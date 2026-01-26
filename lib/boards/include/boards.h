#pragma once
#include "../soc/include/mmio.h"
#include "../../general/include/config.h"
#include "../../general/include/stdlib.h"

/*
Revision	Model	        PCB Revision	Memory	        CPU Core            Partnumber  SoC         CORE_FREQ   ARM_FREQ    Notes
Old style revision codes                                                        MIDR reg                GHz         GHz
0002	    B	            1.0	            256 MB	                                                                            (Mfg by Egoman)
0003        B	            1.0	            256 MB	                                                                            (Mfg by Egoman), fuses mod and D14 removed
0004        B	            2.0	            256 MB	                                                                            (Mfg by Sony)
0005        B	            2.0	            256 MB	                                                                            (Mfg by Qisda)
0006        B	            2.0	            256 MB	                                                                            (Mfg by Egoman)
0007        A	            2.0	            256 MB	        ARM1176JZF-S (Arm11)0xB76       BCM2835                             (Mfg by Egoman)
0008        A	            2.0	            256 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)
0009        A	            2.0	            256 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Qisda)
000d	    B	            2.0	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Egoman)
000e	    B	            2.0	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)
000f	    B	            2.0	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Egoman)
0010        B+	            1.2	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)
0011        CM1             1.0	            512 MB	        ARM1176JZF-S        0xB76       BCM2835     0.25        0.70        (Mfg by Sony)
0012        A+	            1.1	            256 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)
0013	    B+	            1.2	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Embest)
0014	    CM1             1.0	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Embest)
0015	    A+	            1.1	            256 MB / 512 MB ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Embest)
New-style revision codes
a01040	    2B	            1.0	            1 GB	        Cortex A7 (Armv7)   0xC07       BCM2836     0.25        0.90        (Mfg by Sony)
a01041	    2B	            1.1	            1 GB	        Cortex A7 (Armv7)   0xC07       BCM2836     0.25        0.90        (Mfg by Sony)
a21041	    2B	            1.1	            1 GB	        Cortex A7 (Armv7)   0xC07       BCM2836     0.25        0.90        (Mfg by Embest)
a02042      2B              1.2             1 GB            Cortex A53 (Armv8)  0xC07       BCM2837                             (Mfg by Sony)
a22042	    2B              1.2	            1 GB	        Cortex A53 (Armv8)  0xC07       BCM2837                             (Mfg by Embest)
900021	    A+	            1.1	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)
900032	    B+	            1.2	            512 MB	        ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)
900061      CM1             1.1             512 MB          ARM1176JZF-S        0xB76       BCM2835                             (Mfg by Sony)      
900092	    Zero	        1.2	            512 MB	        ARM1176JZF-S        0xB76       BCM2835     0.25        0.70        (Mfg by Sony)
920092	    Zero	        1.2	            512 MB	        ARM1176JZF-S        0xB76       BCM2835     0.25        0.70        (Mfg by Embest)
900093      Zero            1.3             512 MB	        ARM1176JZF-S        0xB76       BCM2835     0.25        0.70        (Mfg by Sony)
920093      Zero            1.3             512 MB	        ARM1176JZF-S        0xB76       BCM2835     0.25        0.70        (Mfg by Embest)
902120	    Zero 2 W	    1.0	            512 MB	        Cortex A53          0xD03       RP3A0       0.25        1.00        (Mfg by Sony), System-in-Package (SiP) consisting of a Broadcom BCM2710A1 — silicon die packaged inside the Broadcom BCM2837
9000c1      Zero W          1.1             512 MB	        ARM1176JZF-S        0xB76       BCM2835     0.25        0.70        (Mfg by Sony)
a02082	    3B      	    1.2	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837                             (Mfg by Sony) https://developer.arm.com/documentation/ddi0500/latest/
a020a0	    CM3      	    1.0	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837                 1.20        (Mfg by Sony)
a220a0	    CM3      	    1.0	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837                             (Mfg by Embest)
a02100      CM3+            1.0             1 GB            Cortex A53 (Armv8)  0xD03       BCM2837                             (Mfg by Sony)
a22081	    3B	            1.1	            1 GB	        Armv7 quad          0xC07       BCM2836     0.25        1.20        (Mfg by Embest)
a22082	    3B	            1.2	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837     0.25        1.20        (Mfg by Embest)
a22083	    3B	            1.3	            1 GB	                                        BCM2837B0                           (Mfg by Embest)
a52082	    3B	            1.2	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837     0.25        1.20        (Mfg by Stadium)
a32082	    3B	            1.2	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837     0.25        1.20        (Mfg by Sony Japan)
a020d3	    3B+	            1.3	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837     0.25        1.40        (Mfg by Sony)
a020d4	    3B+	            1.4	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837     0.25        1.40        (Mfg by Sony)
9020e0	    3A+	            1.0	            512 MB	        Cortex A53 (Armv8)  0xD03       BCM2837B0   0.25        1.40        (Mfg by Sony)
9020e1	    3A+	            1.1	            512 MB	        Cortex A53 (Armv8)  0xD03       BCM2837B0   0.25        1.40        (Mfg by Sony)
a02100	    CM3+            1.0	            1 GB	        Cortex A53 (Armv8)  0xD03       BCM2837B0                           (Mfg by Sony)
a03140      CM4             1.0             1 GB            Cortex-A72 (Armv8-A)0xD08       BCM2711                 1.50        (Mfg by Sony)
b03140      CM4             1.0             2 GB            Cortex-A72 (Armv8-A)0xD08       BCM2711                             (Mfg by Sony)
c03140      CM4             1.0             4 GB            Cortex-A72 (Armv8-A)0xD08       BCM2711                             (Mfg by Sony)
d03140      CM4             1.0             8 GB            Cortex-A72 (Armv8-A)0xD08       BCM2711                             (Mfg by Sony)
a03111	    4B      	    1.1	            1 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
b03111	    4B      	    1.1	            2 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
b03112	    4B      	    1.2	            2 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
b03114	    4B      	    1.4	            2 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
b03115	    4B      	    1.5	            2 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
c03111	    4B      	    1.1	            4 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
c03112	    4B      	    1.2	            4 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
c03114	    4B      	    1.4	            4 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
c03115	    4B      	    1.5	            4 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
d03114	    4B      	    1.4	            8 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
d03115	    4B      	    1.5	            8 GB	        Cortex-A72 (Armv8-A)0xD08       BCM2711     0.50        1.50        (Mfg by Sony)
c03130      400             1.0             4 GB            Cortex-A72 (Armv8-A)0xD08       BCM2711                             (Mfg by Sony)
b04170      5               1.0             2 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
c04170      5               1.0             4 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
d04170      5               1.0             8 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
b04171      5               1.1             2 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
c04171      5               1.1             4 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
d04171      5               1.1             8 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
e04171      5               1.1             16 GB           Cortex-A76 (Armv8-A)0xD0B       BCM2712     1.00        2.40        (Mfg by Sony)
b04180      CM5             1.0             2 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
c04180      CM5             1.0             4 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
d04180      CM5             1.0             8 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
e04180      CM5             1.0             16 GB           Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
d04190      500             1.0             8 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
e04190      500             1.0             16 GB           Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
b041a0      CM5 Lite        1.0             2 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
c041a0      CM5 Lite        1.0             4 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
d041a0      CM5 Lite        1.0             8 GB            Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
e041a0      CM5 Lite        1.0             16 GB           Cortex-A76 (Armv8-A)0xD0B       BCM2712                             (Mfg by Sony)
*/

/* Format: X(naam, soc) */
#define BOARD_LIST(X)           \
    X(RPIA,        BCM2835)     \
    X(RPIA_PLUS,   BCM2835)     \
    X(RPIB,        BCM2835)     \
    X(RPIB_PLUS,   BCM2835)     \
    X(RPICM1,      BCM2835)     \
    X(RPI2B,       BCM2836)     \
    X(RPIZ1,       BCM2835)     \
    X(RPIZ1W,      BCM2835)     \
    X(RPIZ2,       RP3A0)       \
    X(RPIZ2W,      RP3A0)       \
    X(RPI3A,       BCM2837B0)   \
    X(RPI3A_PLUS,  BCM2837B0)   \
    X(RPI3B,       BCM2837)     \
    X(RPI3B_PLUS,  BCM2837)     \
    X(RPICM3,      BCM2837)     \
    X(RPICM3_PLUS, BCM2837)     \
    X(RPICM4,      BCM2711)     \
    X(RPICM4_PLUS, BCM2711)     \
    X(RPI4B,       BCM2711)     \
    X(RPI5,        BCM2712)     \
    X(RPICM5,      BCM2712)     \
    X(RPI500,      BCM2712)

typedef enum {
    #define X(name, soc) name,
    BOARD_LIST(X)
    #undef X
    BOARD_COUNT
} board_name_t;

// MAC address of the board
typedef struct {
   uint8_t bytes[6];
}mac_addr;

// Board serial number
typedef struct {
   uint8_t bytes[8];
}board_serial;

// Clock IDs, used to index clock rate arrays in board_t
typedef enum {
reserved_id,       // 0x0 
EMMC_id,           // 0x1 
UART_id,           // 0x2
ARM_id,            // 0x3
CORE_id,           // 0x4
V3D_id,            // 0x5
H264_id,           // 0x6
ISP_id,            // 0x7
SDRAM_id,          // 0x8
PIXEL_id,          // 0x9
PWM_id,            // 0xa
HEVC_id,           // 0xb
EMMC2_id,          // 0xc
M2MC_id,           // 0xd
PIXEL_BVB_id,      // 0xe

CLOCK_SIZE          // 0xf
}clock_id;

typedef struct {
    board_name_t model;
    char *text;
    soc_t soc;
    int32_t baudrate;                       // set via config.h
    int16_t core_freq_mhz;                  // set via CMakelists.txt
    int16_t arm_freq_mhz;                   // set via CMakelists.txt
    uint32_t firmware_version;              // set during board_init
    uint32_t board_model;                   // set during board_init
    uint32_t board_revision;                // set during board_init
    board_serial serial;                    // set during board_init
    mac_addr MAC;                           // set during board_init
    uint32_t arm_memory_base;               // set during board_init
    uint32_t arm_memory_size;               // set during board_init
    uint32_t soc_temperature;               // set during board_init
    uint32_t clock_rates[sizeof(clock_id)]; // set during board_init
    uint32_t clock_rates_measured[sizeof(clock_id)]; // set during board_init
    uint32_t max_clock_rates[sizeof(clock_id)]; // set during board_init
    uint32_t min_clock_rates[sizeof(clock_id)]; // set during board_init
}board_t;

extern board_t boards[];

typedef struct {
    void (*init)(void);
    void (*info)(void);
} board_ops_t;
extern board_ops_t *board;

extern board_t *board_data;
