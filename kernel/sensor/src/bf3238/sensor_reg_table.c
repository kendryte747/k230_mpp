/***********************************************************************
//BF3238_MIPI_Raw10_XCLK24M_PCLK74.25M_V1_20210323
//XCLK:24M;  MIPICLK:742.5M  PCLK(RAW10):74.25M
//行长：2200  帧长：1125
//Max fps:30fps
//1928*1088
************************************************************************/
static k_sensor_reg bf3238_mipi_1lane_1920x1080_init[] =
{
#if 1
    {0xf2, 0x01},
    {0xf2, 0x00},
    {0xf3, 0x00},
    {0xf3, 0x00},
    {0xf3, 0x00},
    {0x00, 0x41},
    {0x03, 0x20},//10bit

    {0x06, 0x50},//     add vblank(default 0x13 = 19 ) -> 80 = 0x50

    {0x0b, 0x4c},
    {0x0c, 0x04},
    {0x0F, 0x48},
    {0x15, 0x4b},
    {0x16, 0x63},
    {0x19, 0x4e},
    {0x1b, 0x7a},
    {0x1d, 0x7a},
    {0x25, 0x88},
    {0x26, 0x48},
    {0x27, 0x86},
    {0x28, 0x44},
    {0x2a, 0x7c},
    {0x2B, 0x7a},
    {0x2E, 0x06},
    {0x2F, 0x53},
    {0xe0, 0x00},
    {0xe1, 0xef},
    {0xe2, 0x47},
    {0xe3, 0x43},
    {0xe7, 0x2B},
    {0xe8, 0x69},
    {0xe9, 0x8b},//0x0b},
    {0xea, 0xb7},
    {0xeb, 0x04},
    {0xe4, 0x7a},
    {0x7d, 0x0e},  //MIPI
    {0xc9, 0x80}, //1928*1088
    // {0xcd, 0x88},
    // {0xcf, 0x40},

    {0xca ,0x70},       // hwin
    {0xcb ,0x40},       // v win 

    {0xcc ,0x00},       // h start = 0x8c = 12
    {0xce ,0x00},       // v start 

    {0xcd ,0x80},       //  hstop  1920*1080
    {0xcf ,0x38},       // v stop 

    {0x30, 0x01},
    {0x4d, 0x00},

    {0x59, 0x10},
    {0x5a, 0x10},
    {0x5b, 0x10},
    {0x5c, 0x10},
    {0x5e, 0x22},
    {0x6a, 0x1f},
    {0x6b, 0x04},
    {0x6c, 0x20},
    {0x6f, 0x10},
    {REG_NULL, 0x00},

#else
    {0xf2 ,0x01},
    {0xf2 ,0x00},
    {0xf3 ,0x00},
    {0xf3 ,0x00},
    {0xf3 ,0x00},
    {0x00 ,0x41},
    {0x03 ,0x20},//10bit
    {0x0b ,0x28},
    {0x0c ,0x05},
    {0x0F ,0x48},
    {0x15 ,0x4b},
    {0x16 ,0x63},
    {0x19 ,0x4e},
    {0x1b ,0x7a},
    {0x1d ,0x7a},
    {0x25 ,0x88},
    {0x26 ,0x48},
    {0x27 ,0x86},
    {0x28 ,0x44},
    {0x2a ,0x7c},
    {0x2B ,0x7a},
    {0x2E ,0x06},
    {0x2F ,0x53},
    {0xe0 ,0x00},
    {0xe1 ,0xef},
    {0xe2 ,0x47},
    {0xe3 ,0x43},
    {0xe7 ,0x2B},
    {0xe8 ,0x69},
    {0xe9 ,0x0b},
    {0xea ,0xb7},
    {0xeb ,0x04},
    {0xe4 ,0x7a},
    {0x7d ,0x0e},//MIPI
    {0xc9 ,0x80},
    // {0xcd ,0x88},  //1928*1088
    // {0xcf ,0x40},

    {0xcd ,0x80},  //1920*1080
    {0xcf ,0x38},

    {0x59 ,0x10},
    {0x5a ,0x10},
    {0x5b ,0x10},
    {0x5c ,0x10},
    {0x5e ,0x22},
    {0x6a ,0x1f},
    {0x6b ,0x08},
    {0x6c ,0x20},
    {0x6f ,0x10},
    {REG_NULL, 0x00},
#endif
};

/***********************************************************************
//BF3238_MIPI_Raw10_XCLK24M_PCLK74.25M_V1_20210323
//XCLK:24M;  MIPICLK:742.5M  PCLK(RAW10):74.25M
//行长：2200  帧长：1125
//Max fps:30fps
//1928*1088
************************************************************************/
static k_sensor_reg bf3238_mipi_1lane_1280x960_init[] =
{

    {0xf2, 0x01},
    {0xf2, 0x00},
    {0xf3, 0x00},
    {0xf3, 0x00},
    {0xf3, 0x00},
    {0x00, 0x41},
    {0x03, 0x20},//10bit
    {0x0b, 0x4c},
    {0x0c, 0x04},
    {0x0F, 0x48},
    {0x15, 0x4b},
    {0x16, 0x63},
    {0x19, 0x4e},
    {0x1b, 0x7a},
    {0x1d, 0x7a},
    {0x25, 0x88},
    {0x26, 0x48},
    {0x27, 0x86},
    {0x28, 0x44},
    {0x2a, 0x7c},
    {0x2B, 0x7a},
    {0x2E, 0x06},
    {0x2F, 0x53},
    {0xe0, 0x00},
    {0xe1, 0xef},
    {0xe2, 0x47},
    {0xe3, 0x43},
    {0xe7, 0x2B},
    {0xe8, 0x69},
    {0xe9, 0x0b},
    {0xea, 0xb7},
    {0xeb, 0x04},
    {0xe4, 0x7a},
    {0x7d, 0x0e},  //MIPI
    {0xc9, 0x80}, //1928*1088
    // {0xcd, 0x88},
    // {0xcf, 0x40},
    {0xca, 0x50},
    {0xcb, 0x30},
    {0xcc, 0x00},    
    {0xcd ,0x00},           //act = 1280 x 960
    {0xce ,0x00}, 
    {0xcf ,0xc0},

    {0x30, 0x01},
    {0x4d, 0x00},

    {0x59, 0x10},
    {0x5a, 0x10},
    {0x5b, 0x10},
    {0x5c, 0x10},
    {0x5e, 0x22},
    {0x6a, 0x1f},
    {0x6b, 0x04},
    {0x6c, 0x20},
    {0x6f, 0x10},
    {REG_NULL, 0x00},
};