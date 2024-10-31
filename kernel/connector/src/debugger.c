#include "connector_dev.h"
#include "io.h"
#include "drv_gpio.h"
#include "k_type.h"
#include "k_vo_comm.h"
#include "k_connector_comm.h"
#include "rtthread.h"
#include <stdint.h>

static k_connector_debugger_init_seq *s_init_seq = NULL;

// cmd, cmd data size, cmd data
static void connector_debugger_send_init_sequence(void)
{
    connector_send_cmd(s_init_seq->seq, s_init_seq->seq_size, K_TRUE);
}

static void connector_debug_power_reset(k_s32 on)
{
    k_u8 rst_gpio;

    if(0 > (rst_gpio = CONFIG_MPP_DSI_LCD_RESET_PIN)) {
        return;
    }

    kd_pin_mode(rst_gpio, GPIO_DM_OUTPUT);
    if (on) {
        kd_pin_write(rst_gpio, GPIO_PV_HIGH);
    } else {
        kd_pin_write(rst_gpio, GPIO_PV_LOW);
    }
}

static void connector_debug_set_backlight(k_s32 on)
{
    k_u8 backlight_gpio;

    if(0 > (backlight_gpio = CONFIG_MPP_DSI_LCD_BACKLIGHT_PIN)) {
        return;
    }

    kd_pin_mode(backlight_gpio, GPIO_DM_OUTPUT);
    if (on) {
        kd_pin_write(backlight_gpio, GPIO_PV_HIGH);
    } else {
        kd_pin_write(backlight_gpio, GPIO_PV_LOW);
    }
}

static k_s32 connector_debug_power_on(void* ctx, k_s32 on)
{
    k_s32 ret = 0;
    struct connector_driver_dev* dev = ctx;

    k230_display_rst();

    if (on) {
        connector_debug_power_reset(1);
        rt_thread_mdelay(100);
        connector_debug_power_reset(0);
        rt_thread_mdelay(100);
        connector_debug_power_reset(1);

        connector_debug_set_backlight(1);
    } else {
        connector_debug_set_backlight(0);
    }

    return ret;
}

static k_s32 connector_debug_set_phy_freq(k_connectori_phy_attr *phy_attr)
{
    k_vo_mipi_phy_attr mipi_phy_attr;

    memset(&mipi_phy_attr, 0, sizeof(k_vo_mipi_phy_attr));

    mipi_phy_attr.m = phy_attr->m;
    mipi_phy_attr.n = phy_attr->n;
    mipi_phy_attr.hs_freq = phy_attr->hs_freq;
    mipi_phy_attr.voc = phy_attr->voc;
    // mipi_phy_attr.phy_lan_num = K_DSI_4LAN; // no effect here.

    connector_set_phy_freq(&mipi_phy_attr);

    return 0;
}

static k_s32 connector_debug_dsi_resolution_init(k_connector_info *info)
{
    k_vo_dsi_attr attr;
    k_vo_display_resolution resolution;

    memset(&attr, 0, sizeof(k_vo_dsi_attr));

    attr.lan_num = info->lan_num;
    attr.cmd_mode = info->cmd_mode;
    attr.lp_div = 8;
    attr.work_mode = info->work_mode;
    memcpy(&resolution, &info->resolution, sizeof(k_vo_display_resolution));
    memcpy(&attr.resolution, &resolution, sizeof(k_vo_display_resolution));

    connector_set_dsi_attr(&attr);

    // if enable test_mode, user should manual change init sequence.
    connector_debugger_send_init_sequence();

    connector_set_dsi_enable(1);

    if(0x01 == info->dsi_test_mode) {
        connector_set_dsi_test_mode();
    }

    return 0;
}

static k_s32 connector_debug_vo_resolution_init(k_vo_display_resolution *resolution, k_u32 bg_color, k_u32 intr_line)
{
    k_vo_display_resolution vo_resolution;
    k_vo_pub_attr attr;

    memset(&attr, 0, sizeof(k_vo_pub_attr));
    attr.bg_color = bg_color;
    attr.intf_sync = K_VO_OUT_1080P30;
    attr.intf_type = K_VO_INTF_MIPI;
    attr.sync_info = resolution;

    connector_set_vo_init();
    connector_set_vtth_intr(1, intr_line);
    connector_set_vo_param(&attr);
    connector_set_vo_enable();

    return 0;
}

k_s32 connector_debug_init(void *ctx, k_connector_info *info)
{
    k_s32 ret = 0;
    struct connector_driver_dev* dev = ctx;

    if(info->pixclk_div != 0) {
        connector_set_pixclk(info->pixclk_div);
    }

    ret |= connector_debug_set_phy_freq(&info->phy_attr);
    ret |= connector_debug_dsi_resolution_init(info);
    ret |= connector_debug_vo_resolution_init(&info->resolution, info->bg_color, info->intr_line);

    return ret;
}

static k_s32 connector_debug_get_chip_id(void* ctx, k_u32* chip_id)
{
    k_s32 ret = 0;

    return ret;
}

static k_s32 connector_debug_conn_check(void* ctx, k_s32* conn)
{
    k_s32 ret = 0;

    *conn = 1;

    return ret;
}

static k_s32 connector_debug_apply_setting(void *ctx, k_connector_debugger_setting *setting)
{
    k_u32 seq_size = 0;

    if(s_init_seq) {
        rt_free_align(s_init_seq);
        s_init_seq = NULL;
    }

    if(NULL == (s_init_seq = rt_malloc_align(setting->init.seq_size + sizeof(k_connector_debugger_init_seq), RT_CPU_CACHE_LINE_SZ))) {
        rt_kprintf("%s:%d malloc failed\n", __func__, __LINE__);
        return -1;
    }

    seq_size = setting->init.seq_size;
    s_init_seq->seq_size = seq_size;

    rt_memcpy(s_init_seq->seq, setting->init.seq, seq_size);

    return 0;
}

struct connector_driver_dev debugger_connector_dev = {
    .connector_name = "debugger",
    .connector_func = {
        .connector_power = connector_debug_power_on,
        .connector_init = connector_debug_init,
        .connector_get_chip_id = connector_debug_get_chip_id,
        .connector_conn_check = connector_debug_conn_check,
        .connector_debugger_apply_setting = connector_debug_apply_setting,
    },
};
