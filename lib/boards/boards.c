#include "include/boards.h"
#include "soc/include/mailbox.h"

void decode_board_revision(board_data_t* board);
board_data_t board;

typedef struct {
    uint8_t id;                             // used to find soc during board_init via mailbox_vc
    const char *description;
} board_name_list_t;

const board_name_list_t boards[] = {
    {0x00,    "Raspberry Pi Model A v1."},
    {0x01,    "Raspberry Pi Model B v1."},
    {0x02,    "Raspberry Pi Model A+ v1."},
    {0x03,    "Raspberry Pi Model B+ v1."},
    {0x04,    "Raspberry Pi 2B v1."},
    {0x05,    "Raspberry Pi Alpha v1."},
    {0x06,    "Raspberry Pi Compute Module 1 v1."},
    {0x08,    "Raspberry Pi 3B v1."},
    {0x09,    "Raspberry Pi Zero v1."},
    {0x0A,    "Raspberry Pi Compute Module 3 v1."},
    {0x0B,    "Raspberry Pi 3B+ v1."},
    {0x0C,    "Raspberry Pi Zero W v1."},
    {0x0D,    "Raspberry Pi 3B+ v1."},
    {0x0E,    "Raspberry Pi 3A+ v1."},
    {0x10,    "Raspberry Pi Compute Module 3+ v1."},
    {0x11,    "Raspberry Pi 4B v1."},
    {0x12,    "Raspberry Pi Zero 2 W v1."},
    {0x13,    "Raspberry Pi 400 v1."},
    {0x14,    "Raspberry Pi Compute Module 4 v1."},
    {0x15,    "Raspberry Pi Compute Module 4S v1."},
    {0x17,    "Raspberry Pi 5 v1."},
    {0x18,    "Raspberry Pi Compute Module 5 v1."},
    {0x19,    "Raspberry Pi 500 v1."},
    {0x1A,    "Raspberry Pi Compute Module 5 Lite v1."},
    {0x1B,    "Unknown board"}
};

const char* mem_size_table[] = {
    "256 MB",
    "512 MB",
    "1 GB",
    "2 GB",
    "4 GB",
    "8 GB",
    "16 GB",
    "32 GB"
};

const char* manufacturer_table[] = {
    "Sony UK",
    "Egoman",
    "Embest",
    "Sony Japan",
    "Embest",
    "Stadium",
    "unknown",
    "unknown"
};

bool board_init(board_data_t *board) {
    bool found = false;
    soc_init();
    if (board != NULL) {
        board->baudrate = BAUDRATE;
        board->firmware_date = get_firmware_revision();
        board->revision_raw_value = get_board_revision();
        decode_board_revision(board);
        board->serial = get_board_serial();
        get_mac_address(board->mac_address);
        get_arm_memory(&board->arm_memory_base, &board->arm_memory_size);
        get_gpu_memory(&board->gpu_memory_base, &board->gpu_memory_size);
        board->soc_temperature = get_soc_temperature();
        get_clock_rates(board->clock_rates);
        get_clock_rates_measured(board->clock_rates_measured);
        get_max_clock_rates(board->max_clock_rates);
        get_min_clock_rates(board->min_clock_rates);
        for (uint8_t i = 0; i < ARRAY_SIZE(boards); i++) {
            if (boards[i].id == board->revision_model_type) {
                board->description = boards[i].description;
                found = true;
                break;
            }
        }
    }
    return found;
};

void decode_board_revision(board_data_t* board) {
    // Controleer of het de nieuwe stijl revisiecode is (Bit 23)
    board->rev_scheme = (board->revision_raw_value >> 23) & 0x1;
    
    if (board->rev_scheme) {
        // Nieuwe stijl
        board->memory_size  = mem_size_table[(board->revision_raw_value >> 20) & 0x7];
        board->manufacturer = manufacturer_table[(board->revision_raw_value >> 16) & 0x7];
//        soc_list_t id = (board->revision_raw_value >> 12) & 0xF;
        board->revision_model_type = (board->revision_raw_value >> 4)  & 0xFF;
        board->revision_num = (board->revision_raw_value >> 0)  & 0xF;
    } else {
        // Oude stijl (pre-Pi 2) - velden zijn hier anders
        board->memory_size  = 0; // Niet gecodeerd in oude stijl
        board->revision_model_type   = board->revision_raw_value & 0xFFFFFF; // Hele waarde is model/rev
    }
}
