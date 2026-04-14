#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAILBOX_VC_OFFSET 0xB880
#define MAILBOX_TIMEOUT   100000            // used to avoid infinite loops in mailbox communication, in case something goes wrong. Value is arbitrary, but should be large enough to not cause false positives.

#define GET_FIRMWARE_REVISION   0x00000001
#define GET_BOARD_REVISION      0x00010002
#define GET_MAC_ADDRESS         0x00010003
#define GET_BOARD_SERIAL        0x00010004
#define GET_ARM_MEMORY          0x00010005
#define GET_GPU_MEMORY          0x00010006
#define GET_CLOCK_RATE          0x00030002
#define GET_MAX_CLOCK_RATE      0x00030004
#define GET_SOC_TEMPERATURE     0x00030006
#define GET_MIN_CLOCK_RATE      0x00030007
#define GET_CLOCK_RATE_MEASURED 0x00030047

// Clock IDs, used to index clock rate arrays in board_t
typedef enum {
INVALID_id = 0,                             // 0x0 
EMMC_id    = 1,                             // 0x1 
UART_id,                                    // 0x2
ARM_id,                                     // 0x3
CORE_id,                                    // 0x4
V3D_id,                                     // 0x5
H264_id,                                    // 0x6
ISP_id,                                     // 0x7
SDRAM_id,                                   // 0x8
PIXEL_id,                                   // 0x9
PWM_id,                                     // 0xa
// below not relevant for this project, but included for completeness
//HEVC_id,                                  // 0xb
//EMMC2_id,                                 // 0xc
//M2MC_id,                                  // 0xd
//PIXEL_BVB_id,                             // 0xe
CLOCK_SIZE                                  // 0xf
} clock_id_t;

//
// Videocore Mailbox, base address at mmio_base + 0x00B880
// The primary means of communication between the ARM and the VideoCore firmware running on the GPU
//
typedef struct {
    volatile uint32_t READ;                 // 0x00 (Mailbox 0: GPU -> ARM)
    uint32_t reserved[3];                   // 0x04, 0x08, 0x0C
    volatile uint32_t PEEK;                 // 0x10
    volatile uint32_t SENDER;               // 0x14
    volatile uint32_t STATUS;               // 0x18
    volatile uint32_t CONFIG;               // 0x1C
    volatile uint32_t WRITE;                // 0x20 (Mailbox 1: ARM -> GPU)
#define MAIL_PROC_REQ   0x00000000          // process request
#define MAIL_FULL       0x80000000          // This bit is set in the status register if there is no space to write into the mailbox
#define MAIL_EMPTY      0x40000000          // This bit is set in the status register if there is nothing to read from the mailbox
#define MAIL_RESP_OK    0x80000000          // request successful
#define MAIL_RESP_ERR   0x80000001          // error parsing request buffer (partial response)
} mailbox_vc_regs_t;
extern volatile mailbox_vc_regs_t *MAILBOX_VC;

extern volatile uint32_t mailbox_buffer[16];

uint32_t get_firmware_revision();
uint32_t get_board_revision();
void get_mac_address(uint8_t* mac);
uint64_t get_board_serial();
void get_arm_memory(uint32_t *base, uint32_t *size);
void get_gpu_memory(uint32_t *base, uint32_t *size);
uint32_t get_clock_rate(clock_id_t id);
void get_clock_rates(uint32_t *id);
void get_clock_rates_measured(uint32_t *id);
void get_max_clock_rates(uint32_t *id);
void get_min_clock_rates(uint32_t *id);
uint32_t get_soc_temperature();
