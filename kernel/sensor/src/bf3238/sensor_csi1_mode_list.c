static k_sensor_ae_info sensor_csi1_ae_info[] = {
    // 1080P30
    {
        .frame_length = 1125,
        .cur_frame_length = 1125,
        .one_line_exp_time = 0.000028,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 16,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000028,
        .gain_increment = BF3238_MIN_GAIN_STEP,
        .max_integraion_line = 1125 - 8,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000028 * (1125 - 8),
        .min_integraion_time = 0.000028 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain =
            {
                .min = 1.0,
                .max = 15.5,
                .step = (1.0f / 128.0f),
            },
        .d_gain =
            {
                .min = 1.0,
                .max = 15.5,
                .step = (1.0f / 1024.0f),
            },
        .cur_fps = 30,
    },
};

static const k_sensor_mode sensor_csi1_mode_list[] = {
    {
        .index = 0,
        .sensor_type = BF3238_MIPI_CSI1_1920X1080_30FPS_10BIT_LINEAR,
        .size =
            {
                .bounds_width = 1920,
                .bounds_height = 1080,
                .top = 0,
                .left = 0,
                .width = 1920,
                .height = 1080,
            },
        .fps = 30000,
        .hdr_mode = SENSOR_MODE_LINEAR,
        .bit_width = 10,
        .bayer_pattern = BAYER_PAT_BGGR,
        .mipi_info =
            {
                .csi_id = 0,
                .mipi_lanes = 1,
                .data_type = 0x2B,
            },
#if defined(MPP_ENABLE_SENSOR_BF3238_ON_CSI1_USE_CHIP_CLK)
        .reg_list = bf3238_mipi_1lane_1920x1080_init,
        .mclk_setting =
            {
                {
                    .mclk_setting_en = K_TRUE,
                    .setting.id = CONFIG_MPP_CSI_DEV1_MCLK_NUM,
                    .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                    .setting.mclk_div = 24,
                },
                {K_FALSE},
                {K_FALSE},
            },
        .sensor_ae_info = &sensor_csi1_ae_info[0],
#else
        .reg_list = bf3238_mipi_1lane_1920x1080_init,
        .mclk_setting =
            {
                {
                    .mclk_setting_en = K_TRUE,
                    .setting.id = CONFIG_MPP_CSI_DEV1_MCLK_NUM,
                    .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                    .setting.mclk_div = 24,
                },
                {K_FALSE},
                {K_FALSE},
            },
        .sensor_ae_info = &sensor_csi1_ae_info[0],
#endif
    },
    {
        .index = 1,
        .sensor_type = BF3238_MIPI_CSI1_1280X960_30FPS_10BIT_LINEAR,
        .size =
            {
                .bounds_width = 1280,
                .bounds_height = 960,
                .top = 0,
                .left = 0,
                .width = 1280,
                .height = 960,
            },
        .fps = 30000,
        .hdr_mode = SENSOR_MODE_LINEAR,
        .bit_width = 10,
        .bayer_pattern = BAYER_PAT_BGGR,
        .mipi_info =
            {
                .csi_id = 0,
                .mipi_lanes = 1,
                .data_type = 0x2B,
            },
#if defined(MPP_ENABLE_SENSOR_BF3238_ON_CSI1_USE_CHIP_CLK)
        .reg_list = bf3238_mipi_1lane_1280x960_init,
        .mclk_setting =
            {
                {
                    .mclk_setting_en = K_TRUE,
                    .setting.id = CONFIG_MPP_CSI_DEV1_MCLK_NUM,
                    .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                    .setting.mclk_div = 24,
                },
                {K_FALSE},
                {K_FALSE},
            },
        .sensor_ae_info = &sensor_csi1_ae_info[0],
#else
        .reg_list = bf3238_mipi_1lane_1280x960_init,
        .mclk_setting =
            {
                {K_FALSE},
                {K_FALSE},
                {K_FALSE},
            },
        .sensor_ae_info = &sensor_csi1_ae_info[0],
#endif
    },
};

#if defined(CONFIG_MPP_ENABLE_SENSOR_BF3238_ON_CSI1_USE_CHIP_CLK)
_Static_assert((CONFIG_MPP_CSI_DEV1_MCLK_NUM >= 1) &&
                   (CONFIG_MPP_CSI_DEV1_MCLK_NUM <= 3),
               "Invalid CONFIG_MPP_CSI_DEV1_MCLK_NUM");
#endif
