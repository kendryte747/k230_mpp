#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "k_connector_comm.h"
#include "k_connector_ioctl.h"
#include "k_type.h"
#include "k_vo_comm.h"
#include "mpi_connector_api.h"

#define pr_debug(...) // printf(__VA_ARGS__)
#define pr_info(...) // printf(__VA_ARGS__)
#define pr_warn(...) // printf(__VA_ARGS__)
#define pr_err(...) printf(__VA_ARGS__)

#define KD_MPI_CONNECTOR_SETTING_PATH ("/sdcard/display_debugger_config.txt")

static void print_phy_attr(const k_connectori_phy_attr* phy_attr) {
    printf("PHY Attributes:\n");
    printf("  n: %u\n", phy_attr->n);
    printf("  m: %u\n", phy_attr->m);
    printf("  voc: %u\n", phy_attr->voc);
    printf("  hs_freq: %u\n", phy_attr->hs_freq);
}

static void print_resolution(const k_vo_display_resolution* resolution) {
    printf("Display Resolution:\n");
    printf("  pclk: %u\n", resolution->pclk);
    printf("  phyclk: %u\n", resolution->phyclk);
    printf("  htotal: %u\n", resolution->htotal);
    printf("  hdisplay: %u\n", resolution->hdisplay);
    printf("  hsync_len: %u\n", resolution->hsync_len);
    printf("  hback_porch: %u\n", resolution->hback_porch);
    printf("  hfront_porch: %u\n", resolution->hfront_porch);
    printf("  vtotal: %u\n", resolution->vtotal);
    printf("  vdisplay: %u\n", resolution->vdisplay);
    printf("  vsync_len: %u\n", resolution->vsync_len);
    printf("  vback_porch: %u\n", resolution->vback_porch);
    printf("  vfront_porch: %u\n", resolution->vfront_porch);
}

static void print_connector_info(const k_connector_info* connector) {
    printf("Connector Info:\n");
    printf("  Connector Name: %s\n", connector->connector_name);
    printf("  Screen Test Mode: %u\n", connector->screen_test_mode);
    printf("  DSI Test Mode: %u\n", connector->dsi_test_mode);
    printf("  Background Color: 0x%08X\n", connector->bg_color);
    printf("  Interrupt Line: %u\n", (1 << connector->intr_line));
    printf("  Pixel Clock Divider: %u\n", connector->pixclk_div);

    // Print LAN number (assuming it is an enum)
    switch (connector->lan_num) {
        case K_DSI_1LAN:
            printf("  LAN Number: 1\n");
            break;
        case K_DSI_2LAN:
            printf("  LAN Number: 2\n");
            break;
        case K_DSI_4LAN:
            printf("  LAN Number: 4\n");
            break;
        default:
            printf("  LAN Number: Unknown\n");
    }

    // Print work mode (assuming it is an enum)
    switch (connector->work_mode) {
        case K_BURST_MODE:
            printf("  Work Mode: Burst Mode\n");
            break;
        case K_NON_BURST_MODE_WITH_SYNC_EVENT:
            printf("  Work Mode: Sync Event Mode\n");
            break;
        case K_NON_BURST_MODE_WITH_PULSES:
            printf("  Work Mode: Pulses Mode\n");
            break;
        default:
            printf("  Work Mode: Unknown\n");
    }

    // Print cmd_mode (assuming it is an enum)
    switch (connector->cmd_mode) {
        case K_VO_LP_MODE:
            printf("  Command Mode: K_VO_LP_MODE\n");
            break;
        case K_VO_HS_MODE:
            printf("  Command Mode: K_VO_HS_MODE\n");
            break;
        default:
            printf("  Command Mode: Unknown\n");
    }

    // Print PHY attributes
    print_phy_attr(&connector->phy_attr);

    // Print resolution
    print_resolution(&connector->resolution);

    // Print connector type (assuming it is an enum)
    printf("Connector Type: %d\n", connector->type);
}

static k_s32 kd_mpi_connector_apply_setting(k_connector_info *info_list, k_connector_debugger_setting *setting) {
    int fd = -1;
    k_s32 ret = 0;
    k_s32 find = 0;

    while(NULL != info_list->connector_name) {
        if(DSI_DEBUGGER_DEVICE == info_list->type) {
            printf("%s, find debugger\n", __func__);

            memcpy(info_list, &setting->info, sizeof(k_connector_info));

            find = 1;
            break;
        }
        info_list++;
    }

    if(0x00 == find) {
        pr_err("%s, not find 'debugger'\n", __func__);
        return -1;
    }

    if(0 > (fd = kd_mpi_connector_open("debugger"))) {
        printf("%s, open display device failed.\n", __func__);

        return -2;
    }

    ret = ioctl(fd, KD_IOC_CONNECTOR_S_DEBUGGER_SETTING, setting);
    if (ret != 0) {
        pr_err("%s, error(%d)\n", __func__, ret);
        return K_ERR_VO_NOT_SUPPORT;
    }

    kd_mpi_connector_close(fd);

    return 0;
}

// Functions to calcaute mipi dsi timing //////////////////////////////////////
static void print_user_config(k_connector_debugger_config *config)
{
    printf("User configure:\n");
    printf(" pclk: %u\n", config->pclk);
    printf(" fps: %u\n", config->fps);
    printf(" lan_num: %u\n", config->lan_num);
    printf(" hdisplay: %u\n", config->hdisplay);
    printf(" hsync_len: %u\n", config->hsync_len);
    printf(" hback_porch: %u\n", config->hback_porch);
    printf(" hfront_porch: %u\n", config->hfront_porch);
    printf(" vdisplay: %u\n", config->vdisplay);
    printf(" vsync_len: %u\n", config->vsync_len);
    printf(" vback_porch: %u\n", config->vback_porch);
    printf(" vfront_porch: %u\n", config->vfront_porch);
}

static k_u32 calc_intr_line(k_u32 vtotal)
{
    k_u32 power;

    if(0 >= vtotal) {
        printf("%s, invalid vtotal.\n", __func__);
        return 1;
    }

    while((1 << power) <= vtotal) {
        power += 1;
    }

    return power -1;
}

k_u32 correct_pclk(k_u32 pclk) {
    const k_u32 DIVISOR = 594000000;  // 594 million
    k_u32 ratio;
    k_u32 corrected_value;

    if (pclk <= 0) {
        printf("%s, pclk must be a positive integer.\n", __func__);
        return 0;
    }

    if (pclk % DIVISOR == 0) {
        return pclk;
    }

    // Calculate the initial ratio
    ratio = DIVISOR / pclk;

    // Adjust the ratio until DIVISOR / ratio % 1000 == 0
    while (1) {
        corrected_value = DIVISOR / ratio;
        if (corrected_value % 1000 == 0) {
            return corrected_value;
        }

        // Increment or decrement the ratio based on its current value
        if (corrected_value < 0) {
            printf("%s, Correction failed; negative corrected value.\n", __func__);
            return 0;
        }

        // Increment the ratio
        ratio++;
    }

    return -1;
}

int calc_m_and_n_impl(k_u32 fout, k_u32 mut, k_u32 clkin, k_u32 *om, k_u32 *on) {
    double ratio = (double)fout / clkin;

    // Iterate over possible n values within the valid range
    for (int n = 1; n <= 16; n++) {
        // Calculate m using the rearranged formula
        double m = mut * (n + 1) * ratio - 2;

        // Check if m is a valid integer and within the specified range
        if ((int)m == m && m > 40 && m < 625) {
            *om = (k_u32)m;
            *on = (k_u32)n;

            return 0;
        }
    }

    printf("Can't generate phy attr with configuration.\n");
    printf("Try to increase or decrease pclk.\n");

    return -1;
}

int calc_m_and_n(k_u32 fout, k_u32 *m, k_u32 *n) {
    k_u32 mut;

    if (fout >= 320000000 && fout <= 1250000000) {
        mut = 1;
    } else if (fout >= 160000000 && fout <= 320000000) {
        mut = 2;
    } else if (fout >= 80000000 && fout <= 160000000) {
        mut = 4;
    } else if (fout >= 40000000 && fout <= 80000000) {
        mut = 8;
    } else {
        printf("Unsupported MIPI Phy Clock: %u\n", fout);
    }

    return calc_m_and_n_impl(fout, mut, 24000000, m, n); // clkin is 24,000,000 by default
}

k_u32 calc_vco(k_u32 fout) {
    if (fout >= 1100000000 && fout <= 1150000000) {
        return 0x01;
    }
    if (fout >= 1150000000 && fout <= 1250000000) {
        return 0x01;
    }
    if (fout >= 630000000 && fout <= 1149000000) {
        return 0x03;
    }
    if (fout >= 420000000 && fout <= 660000000) {
        return 0x07;
    }
    if (fout >= 320000000 && fout <= 440000000) {
        return 0x0F;
    }
    if (fout >= 210000000 && fout <= 330000000) {
        return 0x17;
    }
    if (fout >= 160000000 && fout <= 220000000) {
        return 0x1F;
    }
    if (fout >= 105000000 && fout <= 165000000) {
        return 0x27;
    }
    if (fout >= 80000000 && fout <= 110000000) {
        return 0x2F;
    }
    if (fout >= 52500000 && fout <= 82500000) {
        return 0x37;
    }
    if (fout >= 40000000 && fout <= 55000000) {
        return 0x3F;
    }

    printf("Unsupported MIPI fout: %u\n", fout);

    return 0;
}

typedef struct {
    unsigned long long start;
    unsigned long long end;
    unsigned char hsfreqrange;
} HSFreqRange;

// Create the ranges array with hex values
static const HSFreqRange ranges[] = {
    {2363125000ULL, 2500000000ULL, 0x49},
    {2315625000ULL, 2500000000ULL, 0x48},
    {2268125000ULL, 2500000000ULL, 0x47},
    {2220625000ULL, 2480625000ULL, 0x46},
    {2173125000ULL, 2428125000ULL, 0x45},
    {2125625000ULL, 2375625000ULL, 0x44},
    {2078125000ULL, 2323125000ULL, 0x43},
    {2030625000ULL, 2270625000ULL, 0x42},
    {1983125000ULL, 2218125000ULL, 0x41},
    {1935625000ULL, 2165625000ULL, 0x40},
    {1888125000ULL, 2113125000ULL, 0x0F},
    {1840625000ULL, 2060625000ULL, 0x3F},
    {1793125000ULL, 2008125000ULL, 0x2F},
    {1745625000ULL, 1955625000ULL, 0x1E},
    {1698125000ULL, 1903125000ULL, 0x0E},
    {1650625000ULL, 1850625000ULL, 0x3E},
    {1603125000ULL, 1798125000ULL, 0x2E},
    {1555625000ULL, 1745625000ULL, 0x1D},
    {1508125000ULL, 1693125000ULL, 0x0D},
    {1460625000ULL, 1640625000ULL, 0x3D},
    {1413125000ULL, 1588125000ULL, 0x2C},
    {1365625000ULL, 1535625000ULL, 0x1C},
    {1318125000ULL, 1483125000ULL, 0x0C},
    {1270625000ULL, 1430625000ULL, 0x3C},
    {1223125000ULL, 1378125000ULL, 0x2B},
    {1175625000ULL, 1325625000ULL, 0x1B},
    {1128125000ULL, 1273125000ULL, 0x0B},
    {1080625000ULL, 1220625000ULL, 0x3B},
    {1033125000ULL, 1168125000ULL, 0x2A},
    {985625000ULL, 1115625000ULL, 0x1A},
    {938125000ULL, 1063125000ULL, 0x0A},
    {890625000ULL, 1010625000ULL, 0x3A},
    {843125000ULL, 958125000ULL, 0x29},
    {795625000ULL, 905625000ULL, 0x19},
    {748125000ULL, 853125000ULL, 0x09},
    {700625000ULL, 800625000ULL, 0x39},
    {653125000ULL, 748125000ULL, 0x28},
    {605625000ULL, 695625000ULL, 0x18},
    {558125000ULL, 643125000ULL, 0x07},
    {510625000ULL, 590625000ULL, 0x37},
    {463125000ULL, 538125000ULL, 0x26},
    {415625000ULL, 485625000ULL, 0x16},
    {368125000ULL, 433125000ULL, 0x05},
    {320625000ULL, 380625000ULL, 0x35},
    {296875000ULL, 354375000ULL, 0x25},
    {273125000ULL, 328125000ULL, 0x14},
    {249375000ULL, 301875000ULL, 0x04},
    {225625000ULL, 275625000ULL, 0x33},
    {211375000ULL, 259875000ULL, 0x23},
    {197125000ULL, 244125000ULL, 0x13},
    {182875000ULL, 228375000ULL, 0x03},
    {168625000ULL, 212625000ULL, 0x32},
    {159125000ULL, 202125000ULL, 0x22},
    {149625000ULL, 191625000ULL, 0x12},
    {140125000ULL, 181125000ULL, 0x02},
    {130625000ULL, 170625000ULL, 0x31},
    {121125000ULL, 160125000ULL, 0x21},
    {111625000ULL, 149625000ULL, 0x11},
    {102125000ULL, 139125000ULL, 0x01},
    {92625000ULL, 128625000ULL, 0x30},
    {83125000ULL, 118125000ULL, 0x20},
    {80000000ULL, 107625000ULL, 0x10},
    {80000000ULL, 97125000ULL, 0x00}
};

// Function to find the hsfreqrange for the given fout
k_u32 get_hsfreqrange(k_u32 fout) {
    int num_ranges = sizeof(ranges) / sizeof(ranges[0]);

    // Check fout against each range
    for (int i = 0; i < num_ranges; i++) {
        if (ranges[i].start <= fout && fout <= ranges[i].end) {
            return ranges[i].hsfreqrange;
        }
    }

    printf("Unsupported MIPI Clock: %u\n", fout);

    return 0;
}

static int kd_mpi_calc_timings(k_connector_debugger_config *config, k_connector_info *info)
{
    k_u32 htotal, vtotal, intr_line;
    k_u32 pclk, pclk_in, pclk_div;

    k_u32 mipi_clock;
    k_u32 attr_m, attr_n, attr_vco, attr_hs_freq;

    htotal = config->hdisplay + config->hfront_porch + config->hback_porch + config->hsync_len;
    vtotal = config->vdisplay + config->vfront_porch + config->vback_porch + config->vsync_len;

    intr_line = calc_intr_line(vtotal);

    if((0x00 != config->fps) && (0x00 == config->pclk)) {
        pclk_in = htotal * vtotal * config->fps;

        printf("use pclk calcauted by fps, %u.\n", pclk_in);
    }

    if(0x00 != config->pclk) {
        pclk_in = config->pclk;

        printf("use pclk user configure, %u\n", pclk_in);
    }

    pclk = correct_pclk(pclk_in);
    if(pclk != pclk_in) {
        printf("correct pclk_in %u to %u\n", pclk_in, pclk);
    }

    pclk_div = ((594 * 1000 * 1000) / pclk) - 1;
    printf("use pclk %u, div %u, fps %u\n", pclk, pclk_div, pclk / htotal / vtotal);

    mipi_clock = pclk * 3 * 8 / (config->lan_num + 1)/ 2;
    printf("mipi clock %u, phy clock %u\n", mipi_clock * 2, mipi_clock);

    if(0x00 != calc_m_and_n(mipi_clock, &attr_m, &attr_n)) {
        printf("calcaute phy attr failed.\n");
    }

    attr_vco = calc_vco(mipi_clock);
    attr_hs_freq = get_hsfreqrange(mipi_clock);

    info->connector_name = "debugger";
    info->screen_test_mode = 0;
    info->dsi_test_mode = 0;
    info->bg_color = BACKGROUND_BLACK_COLOR;
    info->intr_line = (0x00 == config->intr_line) ? intr_line : config->intr_line;
    info->pixclk_div = pclk_div;
    info->lan_num = config->lan_num;
    info->work_mode = K_BURST_MODE;
    info->cmd_mode = K_VO_LP_MODE;

    info->phy_attr.m = attr_m;
    info->phy_attr.n = attr_n;
    info->phy_attr.voc = attr_vco;
    info->phy_attr.hs_freq = 0x80 | attr_hs_freq;

    info->resolution.pclk = config->pclk / 1000;
    info->resolution.phyclk = mipi_clock * 2;
    info->resolution.htotal = htotal;
    info->resolution.hdisplay = config->hdisplay;
    info->resolution.hsync_len = config->hsync_len;
    info->resolution.hback_porch = config->hback_porch;
    info->resolution.hfront_porch = config->hfront_porch;
    info->resolution.vtotal = vtotal;
    info->resolution.vdisplay = config->vdisplay;
    info->resolution.vsync_len = config->vsync_len;
    info->resolution.vback_porch = config->vback_porch;
    info->resolution.vfront_porch = config->vfront_porch;

    info->type = DSI_DEBUGGER_DEVICE;

    return 0;
}

static char* copy_line(char *dest, const char *src, size_t max_len)
{
    size_t i = 0, start = 0, end = 0;

    // Skip leading spaces and tabs
    while (src[start] == ' ' || src[start] == '\t') {
        start++;
    }

    // Copy characters from src to dest until '\n', '\r', '#', or end of string
    while (src[start + i] != '\0' && src[start + i] != '\n' && src[start + i] != '\r' 
           && src[start + i] != '#' && i < max_len - 1) {
        dest[i] = src[start + i];
        i++;
    }

    // Find the position of the last non-space character (trimming trailing spaces)
    end = i;
    while (end > 0 && (dest[end - 1] == ' ' || dest[end - 1] == '\t')) {
        end--;
    }

    // Add null terminator after the last non-space character
    dest[end] = '\0';

    // Warn if the line was too long to fit in the buffer
    if (src[start + i] != '\r' && src[start + i] != '\n' && src[start + i] != '\0' && src[start + i] != '#') {
        printf("Warning: This line is too long, only copied '%s'\n", dest);
    }

    // Skip any remaining characters until end of line (e.g., comments, '\n', or '\r')
    while (src[start + i] != '\0' && src[start + i] != '\n' && src[start + i] != '\r') {
        i++;
    }

    // Skip any trailing newline or carriage return characters
    while (src[start + i] == '\n' || src[start + i] == '\r') {
        i++;
    }

    // Return pointer to the next line or NULL if end of string is reached
    if (src[start + i] != '\0') {
        return (char*)(src + start + i);
    } else {
        return NULL; // End of string reached
    }
}

enum {
    DATA_STAT_INVALID = 0,
    DATA_STAT_CONFIG = 1,
    DATA_STAT_INIT_SEQ = 2,
};

static inline __attribute__((always_inline)) k_u32 shash(const char *s) {
  k_u32 v = 5381;
  if (s) {
    while (*s)
      v = (v << 5) + v + (*s++);
  }
  return v;
}

static void parse_config(char *line, k_connector_debugger_config *config) {
    k_u32 cmd_hash;
    k_u32 value;

    char *p = NULL, *pend = NULL;
    
    if(NULL == (p = strstr(line, "="))) {
        printf("can't find '='\n");
        return;
    }
    p[0] = '\0';

    cmd_hash = shash(line);
    value = strtoul(p + 1, &pend, 0);

    // printf("parse %s is %d\n", line, value);

    switch(cmd_hash) {
        case 0x7C9C2D6F: /* 'pclk' */
        {
            config->pclk = value;
        } break;
        case 0x0B8873AE: /* 'fps' */
        {
            config->fps = value;
        } break;
        case 0x5A3B3994: /* 'lane_num' */
        {
            if(0x01 == value) {
                config->lan_num = K_DSI_1LAN;
            } else if(0x02 == value) {
                config->lan_num = K_DSI_2LAN;
            } else if(0x04 == value) {
                config->lan_num = K_DSI_4LAN;
            } else {
                printf("Invalid lane num(%u), should be 1, 2 or 4\n", value);
            }
        } break;
        case 0x2FB419E9: /* 'intr_line' */
        {
            config->intr_line = value;
        } break;
        case 0x32ED94E9: /* 'hactive' */
        {
            config->hdisplay = value;
        } break;
        case 0x0B887AE3: /* 'hfp' */
        {
            config->hfront_porch = value;
        } break;
        case 0x0B887A5F: /* 'hbp' */
        {
            config->hback_porch = value;
        } break;
        case 0x0F9A156A: /* 'hsync' */
        {
            config->hsync_len = value;
        } break;
        case 0x689CE777: /* 'vactive' */
        {
            config->vdisplay = value;
        } break;
        case 0x0B88B671: /* 'vfp' */
        {
            config->vfront_porch = value;
        } break;
        case 0x0B88B5ED: /* 'vbp' */
        {
            config->vback_porch = value;
        } break;
        case 0x10976C78: /* 'vsync' */
        {
            config->vsync_len = value;
        } break;
        default: {
            printf("unsupport config '%s'\n", line);
        } break;
    }
}

int parse_and_append_command(const char *line, k_u8 *init_seq, k_u32 *init_seq_size, k_u32 max_seq_size) {
    int parsed_values[256];  // To store the parsed integer values
    int num_values = 0;

    // Parse the line (hex and decimal numbers)
    const char *p = line;
    while (*p) {
        k_u32 value;
        if (sscanf(p, "%x", &value) == 1 || sscanf(p, "%u", &value) == 1) {
            parsed_values[num_values++] = value;
        }
        // Move to the next value
        while (*p && *p != ',' && *p != ' ' && *p != '\t') {
            p++;
        }
        while (*p == ',' || *p == ' ' || *p == '\t') {
            p++;
        }
    }

    if (num_values < 3) {
        // Not enough values to form a valid command slice
        printf("Invalid line: %s\n", line);
        return -1;
    }

    // Extract cmd_type, cmd_delay, and cmd_size
    k_u8 cmd_type = (k_u8)parsed_values[0];
    k_u8 cmd_delay = (k_u8)parsed_values[1];
    k_u8 cmd_size = (k_u8)parsed_values[2];

    // Calculate the total size of the command slice
    k_u32 slice_size = sizeof(k_connector_cmd_slice) + cmd_size;

    // Check if there is enough space left in the init_seq buffer
    if (*init_seq_size + slice_size > max_seq_size) {
        printf("Error: Buffer is too small to accommodate the command slice from line: %s\n", line);
        return -1;  // Failure: not enough space in the buffer
    }

    // Copy the command slice into init_seq
    k_connector_cmd_slice *cmd_slice = (k_connector_cmd_slice *)(init_seq + *init_seq_size);
    cmd_slice->cmd_type = cmd_type;
    cmd_slice->cmd_delay = cmd_delay;
    cmd_slice->cmd_size = cmd_size;

    // Copy the command data (the rest of the parsed values)
    for (int i = 0; i < cmd_size; i++) {
        cmd_slice->cmd_data[i] = (k_u8)parsed_values[3 + i];
    }

    // Update the total size
    *init_seq_size += slice_size;

    return 0;  // Success
}

// hash of '[config]' 0x46CC54D3
// hash of '[init-sequence]' 0xE007B197
static void update_data_stat(char *line, int *stat)
{
    k_u32 cmd_hash;

    cmd_hash = shash(line);

    if(0x46CC54D3 == cmd_hash) {
        *stat = DATA_STAT_CONFIG;
    } else if(0xE007B197 == cmd_hash) {
        *stat = DATA_STAT_INIT_SEQ;
    } else {
        printf("unsupport cmd '%s'\n", line);
    }
}

void kd_mpi_connector_parse_setting(k_connector_info *info_list) {
#define MAX_SEQ_ARRAY_SIZE (1024 * 2)

    k_u8 *init_seq = NULL;
    k_u32 init_seq_size = 0;

    int fd = -1, ret;
    k_u8 *file_data = NULL;
    k_u32 file_data_size = 0;
    struct stat file_stat;

    char line[256];
    char *next_line = NULL;

    int data_status = DATA_STAT_INVALID;

    k_u32 fps;
    k_connector_debugger_config config;
    k_connector_debugger_setting *setting = NULL;

    // we assume init sequence max 2KiB
    if(NULL == (init_seq = malloc(MAX_SEQ_ARRAY_SIZE))) {
        pr_err("%s, malloc for init sequence failed.\n", __func__);

        return;
    }

    if(0 > (fd = open(KD_MPI_CONNECTOR_SETTING_PATH, O_RDONLY))) {
        pr_err("%s, open file failed.\n", __func__);

        ret = -1;
        goto _failed;
    }

    if(0x00 != fstat(fd, &file_stat)) {
        pr_err("%s, get file stat failed.\n", __func__);

        ret = -2;
        goto _failed;
    }

    file_data_size = file_stat.st_size;
    if(NULL == (file_data = malloc(file_data_size))) {
        pr_err("%s, malloc for read file failed.\n", __func__);

        ret = -3;
        goto _failed;
    }

    if(file_data_size != read(fd, file_data, file_data_size)) {
        pr_err("%s, read file failed.\n", __func__);

        ret = -4;
        goto _failed;
    }
    close(fd);
    fd = -1;

    memset(&config, 0, sizeof(config));

    next_line = (char *)file_data;
    do {
        next_line = copy_line(line, next_line, sizeof(line));

        // printf("get line: '%s'\n", line);

        if('#' == line[0] || '/' == line[0]) {
            // find comment
            continue;
        }

        if('[' == line[0]) {
            update_data_stat(line, &data_status);
            continue;
        }

        if(DATA_STAT_CONFIG == data_status) {
            parse_config(line, &config);
        } else if(DATA_STAT_INIT_SEQ == data_status) {
            parse_and_append_command(line, init_seq, &init_seq_size, MAX_SEQ_ARRAY_SIZE);
        } else {
            printf("unsupport line '%s'\n", line);
        }
    } while(next_line);

    {
        // dump init sequence
        k_connector_cmd_slice *cmd;
        const k_u8 *pcmd = init_seq, *pcmd_end = init_seq + init_seq_size;
        k_u32 cmd_remain = 0;

        printf("cmds:\ntotal size %u\n", init_seq_size);
        do {
            cmd = (k_connector_cmd_slice *)pcmd;
            cmd_remain = pcmd_end - pcmd;

            if(cmd->cmd_size > cmd_remain) {
                printf("error cmd sequence.\n");
                break;
            }

            for(int i = 0; i < sizeof(k_connector_cmd_slice) + cmd->cmd_size; i++) {
                printf("%02X ", pcmd[i]);
            }
            printf("\n");

            pcmd += sizeof(k_connector_cmd_slice) + cmd->cmd_size;
        } while(pcmd < pcmd_end);
    }

    if(NULL == (setting = malloc(sizeof(k_connector_debugger_setting) + init_seq_size))) {
        pr_err("%s, malloc for init sequence failed.\n", __func__);

        ret = -1;
        goto _failed;
    }

    setting->setting_size = sizeof(k_connector_debugger_setting) + init_seq_size;

    setting->init.seq_size = init_seq_size;
    memcpy(setting->init.seq, init_seq, init_seq_size);

    if(0x00 != kd_mpi_calc_timings(&config, &setting->info)) {
        printf("calculate timing failed.\n");

        ret = -1;
        goto _failed;
    }

    print_connector_info(&setting->info);

    ret = kd_mpi_connector_apply_setting(info_list, setting);

_failed:
    if(0 <= fd) {
        close(fd);
        fd = -1;
    }

    if(init_seq) {
        free(init_seq);
        init_seq = NULL;
    }

    if(setting) {
        free(setting);
        setting = NULL;
    }

    if(0x00 != ret) {
        printf("%s, parse setting failed. %d", __func__, ret);
    }

    return;
}
