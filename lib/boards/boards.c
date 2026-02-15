#include "include/boards.h"
#include "soc/include/mailbox.h"

void decode_board_revision(board_data_t* board);
board_data_t board;

board_name_list_t boards[] = {
    #define X(txt, nr, soc_enum, desc) { \
        .name = #txt, \
        .id = nr, \
        .soc = soc_enum, \
        .description = desc \
    },
    BOARD_LIST(X)
    #undef X
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

bool board_init(board_data_t *board, const char* name) {
    board->baudrate = BAUDRATE;
    board->core_freq_mhz = CORE_FREQ;
    bool found = false;
    if (board != NULL) {
        for (uint8_t i = 0; i < BOARD_COUNT; i++) {
            if (strcmp(name, boards[i].name) == 0) {
                board->used = &boards[i];
                found = true;
                break;
            }
        }
    }
    soc_init(board->used->soc);
    return found;
};

void board_info(board_data_t *board){
    board->firmware_date = get_firmware_revision();
    board->revision_raw_value = get_board_revision();
    decode_board_revision(board);
    soc_info(board->used->soc);
    board->serial = get_board_serial();
    get_mac_address(board->mac_address);
    get_arm_memory(&board->arm_memory_base, &board->arm_memory_size);
    get_gpu_memory(&board->gpu_memory_base, &board->gpu_memory_size);
    board->soc_temperature = get_soc_temperature();
    get_clock_rates(board->clock_rates);
    get_clock_rates_measured(board->clock_rates_measured);
    get_max_clock_rates(board->max_clock_rates);
    get_min_clock_rates(board->min_clock_rates);
}


void decode_board_revision(board_data_t* board) {
    // Controleer of het de nieuwe stijl revisiecode is (Bit 23)
    board->rev_scheme = (board->revision_raw_value >> 23) & 0x1;
    
    if (board->rev_scheme) {
        // Nieuwe stijl
        board->memory_size  = mem_size_table[(board->revision_raw_value >> 20) & 0x7];
        board->manufacturer = manufacturer_table[(board->revision_raw_value >> 16) & 0x7];
        soc_list_t id = (board->revision_raw_value >> 12) & 0xF;
        board->read    = id;
        board->revision_model_type = (board->revision_raw_value >> 4)  & 0xFF;
        board->revision_num = (board->revision_raw_value >> 0)  & 0xF;
    } else {
        // Oude stijl (pre-Pi 2) - velden zijn hier anders
        board->memory_size  = 0; // Niet gecodeerd in oude stijl
        board->revision_model_type   = board->revision_raw_value & 0xFFFFFF; // Hele waarde is model/rev
    }
}
