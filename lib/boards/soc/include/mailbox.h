#pragma once
#include <stdint.h>
#include "../../include/boards.h"
#include <stdbool.h>
#include "../BCM2835/include/BCM2835.h"

// Mailbox Video Core operations structure
typedef struct {
    uint32_t (*read)(uint8_t mailbox);
    void (*write)(uint8_t mailbox, uint32_t val);
} mailbox_vc_ops_t;
extern const mailbox_vc_ops_t *mailbox_vc;

typedef struct {
    uint32_t (*read)(uint8_t mailbox, uint8_t core);
    void (*write)(uint8_t mailbox, uint8_t core, uint32_t val);
} mailboxes_ops_t;
extern const mailboxes_ops_t *mailbox;

extern volatile uint32_t mailbox_buffer[16];

void bcm283x_mailbox_vc_write(volatile bcm2835_mailbox_vc_regs_t *regs, uint8_t channel, uint32_t addres28);
uint32_t bcm283x_mailbox_vc_read(volatile bcm2835_mailbox_vc_regs_t *regs, uint8_t channel);
uint32_t get_firmware_revision();
uint32_t get_board_revision();
void get_mac_address(uint8_t* mac);
uint64_t get_board_serial();
void get_arm_memory(uint32_t *base, uint32_t *size);
void get_gpu_memory(uint32_t *base, uint32_t *size);
void get_clock_rates(uint32_t *id);
void get_clock_rates_measured(uint32_t *id);
void get_max_clock_rates(uint32_t *id);
void get_min_clock_rates(uint32_t *id);
uint32_t get_soc_temperature();

