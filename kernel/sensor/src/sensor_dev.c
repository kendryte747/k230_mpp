/* Copyright (c) 2023, Canaan Bright Sight Co., Ltd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#ifdef RT_USING_POSIX
#include <dfs_posix.h>
#include <dfs_poll.h>
#include <posix_termios.h>
#endif

#include "sensor_dev.h"
#include "k_sensor_comm.h"

#include "k_autoconf_comm.h"

struct sensor_driver_dev g_sensor_drv[3];

static sensor_probe_impl sensor_probes[] = {
#ifdef CONFIG_MPP_ENABLE_SENSOR_GC2093
    sensor_gc2093_probe,
#endif // CONFIG_MPP_ENABLE_SENSOR_GC2093
#ifdef CONFIG_MPP_ENABLE_SENSOR_OV5647
    sensor_ov5647_probe,
#endif // CONFIG_MPP_ENABLE_SENSOR_OV5647
#if defined (CONFIG_MPP_ENABLE_SENSOR_IMX335)
    sensor_imx335_probe,
#endif // CONFIG_MPP_ENABLE_SENSOR_IMX335
    0, // end
};

static k_s32 sensor_dev_open(struct dfs_fd *file)
{
    struct rt_device *device;
	struct sensor_driver_dev *pdriver_dev;
	k_s32 ret = 0;

    //rt_kprintf("%s enter, device(%p)\n", __func__, file->fnode->data);

    device = file->fnode->data;
	if (device == NULL) {
		rt_kprintf("%s: device is null\n", __func__);
		return -ENOMEM;
	}
    pdriver_dev = (struct sensor_driver_dev *)device;

	//rt_kprintf("%s exit\n", __func__);

	return 0;
}

static k_s32 sensor_dev_close(struct dfs_fd *file)
{
    struct rt_device *device;
	struct sensor_driver_dev *pdriver_dev;
	k_s32 ret = 0;

    //rt_kprintf("%s enter, device(%p)\n", __func__, file->fnode->data);

    device = file->fnode->data;
	if (device == NULL) {
		rt_kprintf("%s: device is null\n", __func__);
		return -ENOMEM;
	}
    pdriver_dev = (struct sensor_driver_dev *)device;

	//rt_kprintf("%s exit\n", __func__);

	return 0;
}

static k_s32 sensor_dev_ioctl(struct dfs_fd *file, k_s32 cmd, void *args)
{
    struct rt_device *device;
	struct sensor_driver_dev *pdriver_dev;
	k_s32 ret = 0;

    device = file->fnode->data;
	if (device == NULL) {
		rt_kprintf("%s: device is null\n", __func__);
		return -ENOMEM;
	}

    pdriver_dev = (struct sensor_driver_dev *)device;

	rt_mutex_take(&pdriver_dev->sensor_mutex, RT_WAITING_FOREVER);
	ret = sensor_priv_ioctl(pdriver_dev, cmd, args);
    rt_mutex_release(&pdriver_dev->sensor_mutex);

    return ret;
}

static const struct dfs_file_ops sensor_dev_fops = {
    .open = sensor_dev_open,
    .close = sensor_dev_close,
    .ioctl = sensor_dev_ioctl,
};

k_s32 sensor_drv_dev_init(struct sensor_driver_dev *pdriver_dev)
{
    struct rt_device *device;
    char dev_name[32];
    k_s32 ret = 0;

    RT_ASSERT(pdriver_dev != NULL)

    device = &pdriver_dev->parent;
    rt_snprintf(dev_name, sizeof(dev_name), "sensor_%s", pdriver_dev->sensor_name);

    rt_mutex_init(&pdriver_dev->sensor_mutex, "sensor_mutex", RT_IPC_FLAG_PRIO);

    ret = rt_device_register(device, dev_name, RT_DEVICE_FLAG_RDWR);
    if (ret) {
        rt_kprintf("sensor device register fail\n");
        return ret;
    }

    device->fops = &sensor_dev_fops;
    device->user_data = pdriver_dev;

    return 0;
}

#if !defined (CONFIG_MPP_ENABLE_CSI_DEV_0) && !defined (CONFIG_MPP_ENABLE_CSI_DEV_1) && !(defined (CONFIG_MPP_ENABLE_CSI_DEV_2))
#error "Must Enable one csi dev"
#endif

k_s32 sensor_device_init(void) {
  struct k_sensor_probe_cfg probe_cfg[] = {
#ifdef CONFIG_MPP_ENABLE_CSI_DEV_0
      {
          .csi_num = 0,
          .i2c_name = CONFIG_MPP_CSI_DEV0_I2C_DEV,
          .pwd_gpio = CONFIG_MPP_CSI_DEV0_POWER,
          .reset_gpio = CONFIG_MPP_CSI_DEV0_RESET,
      },
#endif // CONFIG_MPP_ENABLE_CSI_DEV_0
#ifdef CONFIG_MPP_ENABLE_CSI_DEV_1
      {
          .csi_num = 1,
          .i2c_name = CONFIG_MPP_CSI_DEV1_I2C_DEV,
          .pwd_gpio = CONFIG_MPP_CSI_DEV1_POWER,
          .reset_gpio = CONFIG_MPP_CSI_DEV1_RESET,
      },
#endif // CONFIG_MPP_ENABLE_CSI_DEV_1
#ifdef CONFIG_MPP_ENABLE_CSI_DEV_2
      {
          .csi_num = 2,
          .i2c_name = CONFIG_MPP_CSI_DEV2_I2C_DEV,
          .pwd_gpio = CONFIG_MPP_CSI_DEV2_POWER,
          .reset_gpio = CONFIG_MPP_CSI_DEV2_RESET,
      },
#endif // CONFIG_MPP_ENABLE_CSI_DEV_2

    // MAX 3 configure, because we just have 3 csi device
  };

  memset(&g_sensor_drv[0], 0, sizeof(g_sensor_drv));

  for (size_t i = 0; i < (sizeof(probe_cfg) / sizeof(probe_cfg[0])); i++) {
    struct k_sensor_probe_cfg *cfg = &probe_cfg[i];

    struct sensor_driver_dev *dev = &g_sensor_drv[cfg->csi_num];

    for(int j = 0; sensor_probes[j]; j++) {
        if(0x00 == sensor_probes[j](cfg, dev)) {
            sensor_drv_dev_init(dev);
            rt_kprintf("find sensor(%s) on csi%d\n", dev->sensor_name, cfg->csi_num);
            break;
        }
    }

    if(NULL == dev->sensor_func.sensor_init) {
        rt_kprintf("Can't find sensor on csi%d\n", cfg->csi_num);
    }
  }

  return 0;
}
