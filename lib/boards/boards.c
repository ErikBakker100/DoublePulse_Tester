#include "include/boards.h"
#include "soc/BCM2835/include/BCM2835.h"
#include "soc/BCM2836/include/BCM2836.h"
#include "soc/BCM2837/include/BCM2837.h"
#include "soc/BCM2711/include/BCM2711.h"
#include "soc/BCM2712/include/BCM2712.h"
#include "soc/include/mailbox.h"
#include "soc/include/mmio.h"

board_t *board_data;
board_ops_t *board;

board_t boards[] = {
    /* Gebruik expliciet de veldnaam .soc voor het tweede argument */
    #define X(name, soc) { .model = name, .text = #name, soc},
    BOARD_LIST(X)
    #undef X
};

void board_init(void) {
    board_data = NULL;
    for (board_name_t i = 0; i < BOARD_COUNT; i++) {
        if (strcmp(RPI_DEFINE, boards[i].text) == 0) {
            board_data = &boards[i];
            break;
        }
    }
    if (board_data == NULL) {               // board not found, error
        return;
    }
    mmio_set(board_data->soc);              // Find the right base addresses for board_data->soc = soc_t in mmio.h
    cpu_info(board_data->soc);              // Read the available CPU info from the CPU
    switch (board_data->soc) {
        case BCM2835:
            BCM2835_init(mmio);
            break;
        case BCM2836:
            BCM2836_init(mmio);
            break;
        case BCM2837:
            BCM2837_init(mmio);
            break;
        case RP3A0:
            BCM2837_init(mmio);
            break;
        case BCM2837B0:
            BCM2837_init(mmio);
            break;
        case BCM2711:
            BCM2711_init(mmio);
            break;
        case BCM2712:
            BCM2712_init(mmio);
            break;
        case UNKNOWN:                       // board not found, error
            break;
    }
    board_data->baudrate = BAUDRATE;
    board_data->core_freq_mhz = CORE_FREQ;
};

void board_info(void){
    board_data->firmware_version = mailbox_vc->get_firmware_version();
    board_data->board_model = mailbox_vc->get_board_model();
    board_data->board_revision = mailbox_vc->get_board_revision();
    board_data->serial = mailbox_vc->get_board_serial();
    board_data->MAC = mailbox_vc->get_mac_address();
    mailbox_vc->get_arm_memory(&board_data->arm_memory_base, &board_data->arm_memory_size);
    board_data->soc_temperature = mailbox_vc->get_soc_temperature();
    mailbox_vc->get_clock_rates(board_data->clock_rates);
    mailbox_vc->get_clock_rates_measured(board_data->clock_rates_measured);
    mailbox_vc->get_max_clock_rates(board_data->max_clock_rates);
    mailbox_vc->get_min_clock_rates(board_data->min_clock_rates);
}

static board_ops_t internal_board_ops = {
    .init = board_init,
    .info = board_info
};

board_ops_t *board = &internal_board_ops;
