// sensor ae info
static k_sensor_ae_info sensor_csi2_ae_info[] = {
     // list  for external  clk 23.76M
    // 1080P30 
    {
        .frame_length = 1206,
        .cur_frame_length = 1206,
        .one_line_exp_time = 0.000027652,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000027652,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 1206 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000027652 * (1206 - 1),
        .min_integraion_time = 0.000027652 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 30,
    },
    // 1080P60 
    {
        .frame_length = 1218,
        .cur_frame_length = 1218,
        .one_line_exp_time = 0.000013683,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000013683,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 1218 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000013683 * (1218 - 1),
        .min_integraion_time = 0.000013683 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 60,
    },
     // 960P 90fps
    {
        .frame_length = 1072,
        .cur_frame_length = 1072,
        .one_line_exp_time =0.000010369,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000010369,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 1072 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000010369 * (1072 - 1),
        .min_integraion_time = 0.000010369 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 90,
    },
    //720P 90fps  
    {
        .frame_length = 837,
        .cur_frame_length = 837,
        .one_line_exp_time =0.000013273,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000013273,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 837 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000013273 * (837 - 1),
        .min_integraion_time = 0.000013273 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 90,
    },
    // list  for external  clk 24M
    {
        .frame_length = 1218,
        .cur_frame_length = 1218,
        .one_line_exp_time = 0.000027375,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000027375,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 1218 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000027375 * (1218 - 1),
        .min_integraion_time = 0.000027375 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 30,
    },
    // 1080P60 
    {
        .frame_length = 1218,
        .cur_frame_length = 1218,
        .one_line_exp_time = 0.000013688,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000013688,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 1218 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000013688 * (1218 - 1),
        .min_integraion_time = 0.000013688 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 60,
    },
     // 960P 90fps
    {
        .frame_length = 1074,
        .cur_frame_length = 1074,
        .one_line_exp_time =0.00001095,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.00001095,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 1074 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.00001095 * (1074 - 1),
        .min_integraion_time = 0.00001095 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
        .cur_fps = 90,
    },
    //720P 90fps  
    {
        .frame_length = 837,
        .cur_frame_length = 837,
        .one_line_exp_time =0.000013273,
        .gain_accuracy = 1024,
        .min_gain = 1,
        .max_gain = 18,
        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,
        .integration_time_increment = 0.000013273,
        .gain_increment = GC2093_MIN_GAIN_STEP,
        .max_integraion_line = 837 - 1,
        .min_integraion_line = 1,
        .max_integraion_time = 0.000013273 * (837 - 1),
        .min_integraion_time = 0.000013273 * 1,
        .cur_integration_time = 0.0,
        .cur_again = 1.0,
        .cur_dgain = 1.0,
        .a_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/64.0f),
        },
        .d_gain = {
            .min = 1.0,
            .max = 63.984375,
            .step = (1.0f/1024.0f),
        },
    },
};

static const k_sensor_mode sensor_csi2_mode_list[] = {
    {
        .index = 0,
        .sensor_type = GC2093_MIPI_CSI2_1920X1080_30FPS_10BIT_LINEAR,
        .size = {
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
        .bayer_pattern = BAYER_PAT_RGGB,
        .mipi_info = {
            .csi_id = 0,
            .mipi_lanes = 2,
            .data_type = 0x2B,
        },
#if defined(CONFIG_MPP_SENSOR_GC2093_ON_CSI2_USE_CHIP_CLK)
        .reg_list = gc2093_mipi2lane_1080p_30fps_linear,
        .mclk_setting = {
            {
                .mclk_setting_en = K_TRUE,
#if defined(CONFIG_BOARD_K230_CANMV_V3P0)
                .setting.id = SENSOR_MCLK0,
#else
                .setting.id = SENSOR_MCLK2,
#endif
                .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                .setting.mclk_div = 25,
            },
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[0],
#else
        .reg_list = gc2093_mipi2lane_1080p_60fps_mclk_24m_linear,
        .mclk_setting = {
            {K_FALSE},
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[4],
#endif
    },
    {
        .index = 1,
        .sensor_type = GC2093_MIPI_CSI2_1920X1080_60FPS_10BIT_LINEAR,
        .size = {
            .bounds_width = 1920,
            .bounds_height = 1080,
            .top = 0,
            .left = 0,
            .width = 1920,
            .height = 1080,
        },
        .fps = 60000,
        .hdr_mode = SENSOR_MODE_LINEAR,
        .bit_width = 10,
        .bayer_pattern = BAYER_PAT_RGGB,
        .mipi_info = {
            .csi_id = 0,
            .mipi_lanes = 2,
            .data_type = 0x2B,
        },
#if defined(CONFIG_MPP_SENSOR_GC2093_ON_CSI2_USE_CHIP_CLK)
        .reg_list = gc2093_mipi2lane_1080p_60fps_linear,
        .mclk_setting = {
            {
                .mclk_setting_en = K_TRUE,
#if defined(CONFIG_BOARD_K230_CANMV_V3P0)
                .setting.id = SENSOR_MCLK0,
#else
                .setting.id = SENSOR_MCLK2,
#endif
                .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                .setting.mclk_div = 25,
            },
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[5],
#else
        .reg_list = gc2093_mipi2lane_1080p_60fps_mclk_24m_linear,
        .mclk_setting = {
            {K_FALSE},
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[5],
#endif
    },
    {
        .index = 2,
        .sensor_type = GC2093_MIPI_CSI2_1280X960_90FPS_10BIT_LINEAR,
        .size = {
            .bounds_width = 1280,
            .bounds_height = 960,
            .top = 0,
            .left = 0,
            .width = 1280,
            .height = 960,
        },
        .fps = 90000,
        .hdr_mode = SENSOR_MODE_LINEAR,
        .bit_width = 10,
        .bayer_pattern = BAYER_PAT_RGGB,
        .mipi_info = {
            .csi_id = 0,
            .mipi_lanes = 2,
            .data_type = 0x2B,
        },
#if defined(CONFIG_MPP_SENSOR_GC2093_ON_CSI2_USE_CHIP_CLK)
        .reg_list = gc2093_mipi2lane_960p_90fps_linear,
        .mclk_setting = {
            {
                .mclk_setting_en = K_TRUE,
#if defined(CONFIG_BOARD_K230_CANMV_V3P0)
                .setting.id = SENSOR_MCLK0,
#else
                .setting.id = SENSOR_MCLK2,
#endif
                .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                .setting.mclk_div = 25,
            },
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[2],
#else
        .reg_list = gc2093_mipi2lane_960p_90fps_mclk_24m_linear,
        .mclk_setting = {
            {K_FALSE},
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[6],
#endif
    },
    {
        .index = 3,
        .sensor_type = GC2093_MIPI_CSI2_1280X720_90FPS_10BIT_LINEAR,
        .size = {
            .bounds_width = 1280,
            .bounds_height = 720,
            .top = 0,
            .left = 0,
            .width = 1280,
            .height = 720,
        },
        .fps = 90000,
        .hdr_mode = SENSOR_MODE_LINEAR,
        .bit_width = 10,
        .bayer_pattern =BAYER_PAT_RGGB,
        .mipi_info = {
            .csi_id = 0,
            .mipi_lanes = 2,
            .data_type = 0x2B,
        },
#if defined(CONFIG_MPP_SENSOR_GC2093_ON_CSI2_USE_CHIP_CLK)
        .reg_list = gc2093_mipi2lane_720p_90fps_linear,
        .mclk_setting = {
            {
                .mclk_setting_en = K_TRUE,
#if defined(CONFIG_BOARD_K230_CANMV_V3P0)
                .setting.id = SENSOR_MCLK0,
#else
                .setting.id = SENSOR_MCLK2,
#endif
                .setting.mclk_sel = SENSOR_PLL1_CLK_DIV4,
                .setting.mclk_div = 25,
            },
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[3],
#else
        .reg_list = gc2093_mipi2lane_720p_90fps_mclk_24m_linear,
        .mclk_setting = {
            {K_FALSE},
            {K_FALSE},
            {K_FALSE},
        },
        .sensor_ae_info = &sensor_csi2_ae_info[7],
#endif
    },
};
