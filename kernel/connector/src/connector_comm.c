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

#include "connector_dev.h"
#include "k_connector_ioctl.h"
#include "k_connector_comm.h"
#include "tick.h"


k_s32 connector_priv_ioctl(struct connector_driver_dev* dev, k_u32 cmd, void* args)
{
    k_s32 ret = -1;
    if (!dev) {
        rt_kprintf("%s error, dev null\n", __func__);
        return ret;
    }

    switch (cmd) {
    case KD_IOC_CONNECTOR_S_POWER: {
        k_s32 power_on;
        if (dev->connector_func.connector_power == NULL) {
            rt_kprintf("%s (%s)connector_power is null\n", __func__, dev->connector_name);
            return -1;
        }

        if (sizeof(k_s32) != lwp_get_from_user(&power_on, args, sizeof(k_s32))) {
            rt_kprintf("%s:%d lwp_get_from_user err\n", __func__, __LINE__);
            return -1;
        }

        ret = dev->connector_func.connector_power(dev, power_on);
        break;
    }
    case KD_IOC_CONNECTOR_S_INIT: {
        k_connector_info connector_info;
        if (dev->connector_func.connector_init == NULL) {
            rt_kprintf("%s (%s)connector_init is null\n", __func__, dev->connector_name);
            return -1;
        }

        if (sizeof(connector_info) != lwp_get_from_user(&connector_info, args, sizeof(connector_info))) {
            rt_kprintf("%s:%d lwp_get_from_user err\n", __func__, __LINE__);
            return -1;
        }

        ret = dev->connector_func.connector_init(dev, &connector_info);
        break;
    }
    case KD_IOC_CONNECTOR_G_ID: {
        k_u32 chip_id = 0;
        if (dev->connector_func.connector_get_chip_id == NULL) {
            rt_kprintf("%s (%s)connector_get_chip_id is null\n", __func__, dev->connector_name);
            return -1;
        }
        ret = dev->connector_func.connector_get_chip_id(dev, &chip_id);
        if (ret) {
            rt_kprintf("%s (%s)connector_get_chip_id err\n", __func__, dev->connector_name);
            return -1;
        }
        if (sizeof(chip_id) != lwp_put_to_user(args, &chip_id, sizeof(chip_id))) {
            rt_kprintf("%s:%d lwp_put_to_user err\n", __func__, __LINE__);
            return -1;
        }
        break;
    }
    case KD_IOC_CONNECTOR_G_NEG_DATA: {
        k_connector_negotiated_data negotiated_data;
        if (dev->connector_func.connector_get_negotiated_data == NULL) {
            rt_kprintf("%s (%s)connector_get_negotiated_data is null\n", __func__, dev->connector_name);
            return -1;
        }
        ret = dev->connector_func.connector_get_negotiated_data(dev, &negotiated_data);
        if (ret == -1) {
            rt_kprintf("%s (%s)connector_get_negotiated_data err\n", __func__, dev->connector_name);
            return -1;
        }
        if (sizeof(negotiated_data) != lwp_put_to_user(args, &negotiated_data, sizeof(negotiated_data))) {
            rt_kprintf("%s:%d lwp_put_to_user err\n", __func__, __LINE__);
            return -1;
        }
        break;
    }
    case KD_IOC_CONNECTOR_S_MIRROR: {
        k_connector_mirror mirror;
        if (dev->connector_func.connector_set_mirror == NULL) {
            rt_kprintf("%s (%s)connector_set_mirror is null\n", __func__, dev->connector_name);
            return -1;
        }

        if (sizeof(mirror) != lwp_get_from_user(&mirror, args, sizeof(mirror))) {
            rt_kprintf("%s:%d lwp_get_from_user err\n", __func__, __LINE__);
            return -1;
        }

        ret = dev->connector_func.connector_set_mirror(dev, &mirror);
        break;
    }
#if defined (CONFIG_MPP_ENABLE_DSI_DEBUGGER)
    case KD_IOC_CONNECTOR_S_DEBUGGER_SETTING: {
        uint32_t setting_size;
        k_connector_debugger_setting *setting = NULL;

        if (dev->connector_func.connector_debugger_apply_setting == NULL) {
            rt_kprintf("%s (%s)connector_debugger_apply_setting is null\n", __func__, dev->connector_name);
            return -1;
        }

        // first get setting size
        if (sizeof(setting_size) != lwp_get_from_user(&setting_size, args, sizeof(setting_size))) {
            rt_kprintf("%s:%d lwp_get_from_user err\n", __func__, __LINE__);
            return -2;
        }

        if(NULL == (setting = rt_malloc_align(setting_size, RT_CPU_CACHE_LINE_SZ))) {
            rt_kprintf("%s:%d malloc failed\n", __func__, __LINE__);
            return -3;
        }

        if (setting_size != lwp_get_from_user(setting, args, setting_size)) {
            rt_kprintf("%s:%d lwp_get_from_user err\n", __func__, __LINE__);

            rt_free_align(setting);
            return -4;
        }

        ret = dev->connector_func.connector_debugger_apply_setting(dev, setting);

        rt_free_align(setting);
    } break;
#endif // CONFIG_MPP_ENABLE_DSI_DEBUGGER
    default:
        break;
    }
    return ret;
}

static inline uint64_t perf_get_times(void)
{
    uint64_t cnt;
    __asm__ __volatile__(
        "rdtime %0"
        : "=r"(cnt));
    return cnt;
}

void connector_delay_us(uint64_t us)
{
    uint64_t delay = (TIMER_CLK_FREQ / 1000000) * us;
    volatile uint64_t cur_time = perf_get_times();
    while (1) {
        if ((perf_get_times() - cur_time) >= delay)
            break;
    }
}

k_u32 connecter_dsi_send_pkg(k_u8* buf, k_u32 cmd_len)
{
    return dwc_lpdt_send_pkg(buf, cmd_len);
}

k_u32 connecter_dsi_read_pkg(k_u8 data_addr)
{
    return dwc_lpdt_read_pkg(data_addr);
}

k_s32 connector_set_phy_freq(k_vo_mipi_phy_attr* phy)
{
    return dwc_mipi_phy_config(phy);
}

k_s32 connector_set_dsi_attr(k_vo_dsi_attr* attr)
{
    return kd_dsi_dsi_attr(attr);
}

k_u32 connector_set_dsi_enable(k_u32 enable)
{
    return dwc_dsi_enable(enable);
}

k_u32 connector_set_dsi_test_mode(void)
{
    return dwc_dst_set_test_mode();
}

k_u32 connector_set_vo_init(void)
{
    return vo_init();
}

k_u32 connector_set_vo_param(k_vo_pub_attr* attr)
{
    return kd_vo_set_dev_param(attr);
}

k_u32 connector_set_vo_attr(k_vo_pub_attr* pub_attr, k_vo_sync_attr* sync_attr)
{
    return kd_vo_set_dev_attr(pub_attr, sync_attr);
}

void connector_set_vo_enable(void)
{
    kd_vo_enable();
}

void connector_set_vtth_intr(k_bool status, k_u32 vpos)
{
    kd_vo_set_vtth_intr(status, vpos);
}

void connector_set_pixclk(k_u32 div)
{
    k230_set_pixclk(div);
}

k_s32 connector_send_cmd(const k_u8 *cmd_seq, size_t cmd_size, k_bool dump)
{
    uint32_t cmd_remain = 0;
    const uint8_t *pcmd = NULL, *pcmd_end = NULL;

    k_connector_cmd_slice *cmd;

    if(NULL == cmd_seq) {
        rt_kprintf("no init sequence set.\n");
        return -1;
    }

    pcmd = cmd_seq;
    pcmd_end = pcmd + cmd_size;

    do {
        cmd = (k_connector_cmd_slice *)pcmd;
        cmd_remain = pcmd_end - pcmd;

        if(cmd->cmd_size > (cmd_remain - sizeof(k_connector_cmd_slice))) {
            rt_kprintf("error cmd sequence. %d > %d\n", cmd->cmd_size, (cmd_remain - sizeof(k_connector_cmd_slice)));
            break;
        }

        if(cmd->cmd_size && ((CMD_TYPE_DCS_WRITE_05 == cmd->cmd_type) || (CMD_TYPE_DCS_WRITE_15 == cmd->cmd_type) || (CMD_TYPE_DCS_WRITE_39 == cmd->cmd_type))) {
            connecter_dsi_send_pkg(cmd->cmd_data, cmd->cmd_size);

            if(dump) {
                rt_kprintf("cmd[0x%02x] -> delay %u, data:", cmd->cmd_type, cmd->cmd_delay);

                for(int i = 0; i < cmd->cmd_size; i++) {
                    rt_kprintf("%02X ", cmd->cmd_data[i]);
                }
                rt_kprintf("\n");
            }

            if(cmd->cmd_delay) {
                uint64_t delay = (uint64_t)(cmd->cmd_delay) * 1000;

                connector_delay_us(delay);
            }
        } else {
            if(cmd->cmd_size) {
                rt_kprintf("unsupport cmd type 0x%02X\n", cmd->cmd_type);
                return -2;
            }
        }

        pcmd += sizeof(k_connector_cmd_slice) + cmd->cmd_size;
    } while(pcmd < pcmd_end);

    return 0;
}
