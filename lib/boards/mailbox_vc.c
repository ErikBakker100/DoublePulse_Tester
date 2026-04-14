#include "include/mailbox_vc.h"
#include "../general/include/stdlib.h"

volatile mailbox_vc_regs_t *MAILBOX_VC;

//
// Write data to the videocore mailbox.
//
bool mailbox_vc_write(uint8_t channel, uint32_t addres28) {
    uint32_t timeout = MAILBOX_TIMEOUT;
    while (MAILBOX_VC->STATUS & MAIL_FULL){ // Wait till the mailbox is empty
        if (--timeout == 0) return false;   // Timeout, something went wrong
    } 
    dsb();
    MAILBOX_VC->WRITE = (addres28 & 0xFFFFFFF0) | (channel & 0xF); // Combine data (28:0) with channel (3:0 bits)
    return true;
}

//
// Read data from the videocore mailbox.
//
bool mailbox_vc_read(uint8_t channel) {
    uint32_t timeout = MAILBOX_TIMEOUT;
    while (timeout--) {                     // Loop until we receive something from the requested channel
		if (!(MAILBOX_VC->STATUS & MAIL_EMPTY)) {  // Wait for data
            dmb();                              // Make sure that the CPU will not read from its Cache, but out of RAM.
    		volatile uint32_t address = MAILBOX_VC->READ; // Read the data
    		uint8_t readChannel = address & 0xF;
		    if (readChannel == channel)         // Return if it's for the requested channel
			return true;
        }
	}
    return false;
}

volatile uint32_t mailbox_buffer[16] __attribute__((aligned(16)));
#define MAIL_PROC_REQ 0x00000000
#define MAIL_RESP_OK  0x80000000
#define MAIL_TAG_END 0

// used for Video core Mailbox
// For details see https://github.com/raspberrypi/firmware/wiki
// The mailbox interface has 28 bits (MSB) available for the data address and 4 bits (LSB) for the channel
// Request and Response message: 28 bits (MSB) buffer address 4 bits

/*  Channel 8 (Property Tags) buffer, Channel 8: Request from ARM for response by VC
    mailbox_buffer[0] = size in bytes
    mailbox_buffer[1] = request/response
    uint8_t mailbox_buffer[2] = tag
    uint32_t mailbox_buffer[3] = Size of the value buffer
    uint32_t mailbox_buffer[4] = Request/Response indicator
    uint32_t mailbox_buffer[5] = return data
    mailbox_buffer[46] = 0x0 end tag
    channel 8: Property tags (ARM -> VC)
*/

bool mailbox_process(uint32_t tag, uint32_t *data, uint8_t data_len) {
    mailbox_buffer[0] = (6 + data_len) * 4; // Total size in bytes
    mailbox_buffer[1] = MAIL_PROC_REQ;
    mailbox_buffer[2] = tag;                // Tag ID
    mailbox_buffer[3] = data_len * 4;       // Return value size in bytes
    mailbox_buffer[4] = 0;                  // Indicator (written by VC)
    for (uint8_t i = 0; i < data_len; i++) {
        mailbox_buffer[5 + i] = data[i];
    }
    mailbox_buffer[5 + data_len] = MAIL_TAG_END;
    clean_cache(mailbox_buffer, mailbox_buffer[0]);
    dmb();
    mailbox_vc_write(8, (uint32_t)(uintptr_t)mailbox_buffer | 0x40000000); // Channel 8, LSB 4 bits for channel, MSB 28 bits for address
    mailbox_vc_read(8);

    invalidate_cache(mailbox_buffer, mailbox_buffer[0]);
    // 3. Verwerk resultaat (0x80000000 = Success)
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        for (uint8_t i = 0; i < data_len; i++) {
            data[i] = mailbox_buffer[5 + i];
        }
        return true; 
    }
    return false; 
}

uint32_t get_firmware_revision() {
    uint32_t ver = 0;
    if (mailbox_process(GET_FIRMWARE_REVISION, &ver, 1)) {
        return ver;
    }
    return 0;
}

uint32_t get_board_revision() {
    uint32_t rev = 0;
    if (mailbox_process(GET_BOARD_REVISION, &rev, 1)) {
        return rev & 0x00FFFFFF;
    }
    return 0;
}

void get_mac_address(uint8_t *mac) {
    uint32_t data[2] = {0, 0};
    if (mailbox_process(GET_MAC_ADDRESS, data, 2)) {
        mac[0] = (data[0] >> 0)  & 0xFF;
        mac[1] = (data[0] >> 8)  & 0xFF;
        mac[2] = (data[0] >> 16) & 0xFF;
        mac[3] = (data[0] >> 24) & 0xFF;
        mac[4] = (data[1] >> 0)  & 0xFF;
        mac[5] = (data[1] >> 8)  & 0xFF;
    } else {
        for (int i = 0; i < 6; i++) mac[i] = 0;
    }
}

uint64_t get_board_serial() {
    uint64_t serial = 0;
    mailbox_process(GET_BOARD_SERIAL, (uint32_t *)&serial, 2);
    return serial;
}

void get_arm_memory(uint32_t *base, uint32_t *size) {
    uint32_t data[2] = {0, 0};
    if (mailbox_process(GET_ARM_MEMORY, data, 2)) {
        *base = data[0];                    // Base address
        *size = data[1];                    // Size in bytes    
        return;
    } else {
        *base = 0;
        *size = 0;
    }
}

void get_gpu_memory(uint32_t *base, uint32_t *size) {
    uint32_t data[2] = {0, 0};
    if (mailbox_process(GET_GPU_MEMORY, data, 2)) {
        *base = data[0];                    // Base address
        *size = data[1];                    // Size in bytes    
        return;
    } else {
        *base = 0;
        *size = 0;
    }
}

uint32_t get_clock_rate(clock_id_t id) {
    uint32_t data[2];
    data[0] = id;                           // Clock speed we want to know
    data[1] = 0;                            // Speed will be returned here
    if (mailbox_process(GET_CLOCK_RATE, data, 2)) {
        return data[1];
    }
    return 0;
}

void get_clock_rates(uint32_t *id) {
    for (clock_id_t nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = get_clock_rate(nr);
    }
}

uint32_t get_clock_rate_measured(clock_id_t id) {
    uint32_t data[2];
    data[0] = id;                           // Clock speed we want to know
    data[1] = 0;                            // Speed will be returned here
    if (mailbox_process(GET_CLOCK_RATE_MEASURED, data, 2)) {
        return data[1];
    }
    return 0;
}

void get_clock_rates_measured(uint32_t *id) {
    for (clock_id_t nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = get_clock_rate_measured(nr);
    }
}

uint32_t get_max_clock_rate(clock_id_t id) {
    uint32_t data[2];
    data[0] = id;                           // Clock speed we want to know
    data[1] = 0;                            // Speed will be returned here
    if (mailbox_process(GET_MAX_CLOCK_RATE, data, 2)) {
        return data[1];
    }
    return 0;
}

void get_max_clock_rates(uint32_t *id) {
    for (clock_id_t nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = get_max_clock_rate(nr);
    }
}

uint32_t get_min_clock_rate(clock_id_t id) {
    uint32_t data[2];
    data[0] = id;                           // Clock speed we want to know
    data[1] = 0;                            // Speed will be returned here
    if (mailbox_process(GET_MIN_CLOCK_RATE, data, 2)) {
        return data[1];
    }
    return 0;
}

void get_min_clock_rates(uint32_t *id) {
    for (clock_id_t nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = get_min_clock_rate(nr);
    }
}

uint32_t get_soc_temperature(void) {
    uint32_t val[2] = {0, 0}; // [0] is Sensor ID, [1] wordt de temperatuur
    if (mailbox_process(GET_SOC_TEMPERATURE, val, 2)) {
        return val[1];
    }
    return 0;
}
