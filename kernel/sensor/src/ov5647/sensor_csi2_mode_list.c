// sensor ae info
static k_sensor_ae_info sensor_csi2_ae_info[] = {
    /* 1080P@30 */
    {
        .frame_length = 1199,
        .cur_frame_length = 1199,
        .one_line_exp_time = 0.000027808,//0.00003025
        .gain_accuracy = 1024,

        .min_gain = 1.0,
        .max_gain = 8.0,//63.9375,

        .int_time_delay_frame = 2,
        .gain_delay_frame = 2,
        .color_type = SENSOR_COLOR,    //color sensor

        .integration_time_increment = 0.000027808,
        .gain_increment = (1.0f/16.0f),

        .max_long_integraion_line = 1199 - 12,
        .min_long_integraion_line = 2,

        .max_integraion_line = 1199 - 12,
        .min_integraion_line = 2,

        .max_long_integraion_time = 0.000027808 * (1199 - 12),
        .min_long_integraion_time = 0.000027808 * 2,

        .max_integraion_time = 0.000027808 * (1199 - 12),
        .min_integraion_time = 0.000027808 * 2,

        .cur_long_integration_time = 0.0,
        .cur_integration_time = 0.0,

        .cur_long_again = 0.0,
        .cur_long_dgain = 0.0,

        .cur_again = 0.0,
        .cur_dgain = 0.0,

        .a_long_gain.min = 1.0,
        .a_long_gain.max = 8.0,
        .a_long_gain.step = (1.0f/16.0f),

        .a_gain.min = 1.0,
        .a_gain.max = 8.0,
        .a_gain.step = (1.0f/16.0f),

        .d_long_gain.max = 1.0,
        .d_long_gain.min = 1.0,
        .d_long_gain.step = (1.0f/1024.0f),

        .d_gain.max = 1.0,
        .d_gain.min = 1.0,
        .d_gain.step = (1.0f/1024.0f),

        .cur_fps = 30,
    },
};

static const k_sensor_mode sensor_csi2_mode_list[] = {
    {
        .index = 0,
        .sensor_type = OV_OV5647_MIPI_1920X1080_30FPS_10BIT_LINEAR,
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
        .bayer_pattern = BAYER_PAT_GBRG,
        .mipi_info = {
            .csi_id = 0,
            .mipi_lanes = 2,
            .data_type = 0x2B, //RAW10
        },
        .reg_list = ov5647_mipi2lane_1080p_30fps_linear,
        .mclk_setting = {{K_FALSE}, {K_FALSE}, {K_FALSE}},
    },
};
