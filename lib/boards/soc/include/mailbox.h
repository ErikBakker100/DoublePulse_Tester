#pragma once
#include <stdint.h>
#include "../../include/boards.h"

// Mailbox Video Core operations structure
typedef struct {
    uint32_t (*read)(uint8_t mailbox, uint8_t core);
    void (*write)(uint8_t mailbox, uint8_t core, uint32_t val);
    uint32_t (*get_firmware_version)(void);
    uint32_t (*get_board_model)(void);
    uint32_t (*get_board_revision)(void);
    mac_addr (*get_mac_address)(void);
    board_serial (*get_board_serial)(void);
    void (*get_arm_memory)(uint32_t *base, uint32_t *size);
    uint32_t (*get_soc_temperature)(void);
    void (*get_clock_rates)(uint32_t [sizeof(clock_id)]);
    void (*get_clock_rates_measured)(uint32_t [sizeof(clock_id)]);
    void (*get_max_clock_rates)(uint32_t [sizeof(clock_id)]);
    void (*get_min_clock_rates)(uint32_t [sizeof(clock_id)]); 
} mailbox_vc_ops_t;
extern const mailbox_vc_ops_t *mailbox_vc;

typedef struct {
    uint32_t (*read)(uint8_t mailbox, uint8_t core);
    void (*write)(uint8_t mailbox, uint8_t core, uint32_t val);
} mailboxes_ops_t;
extern const mailboxes_ops_t *mailbox;

extern volatile uint32_t mailbox_buffer[8];