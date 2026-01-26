#include "include/mailbox_BCM2835.h"
#include "../../include/mailbox.h"

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

/* Data Memory Barrier */
static inline void dmb(void) {
    #if defined(__arm__) && (__ARM_ARCH <= 6)
        // ARMv6 (Pi 1 / Zero)
        __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory");
    #else
        // ARMv7 en ARMv8 (Pi 2, 3, 4, 5 in zowel 32-bit als 64-bit)
        __asm__ __volatile__ ("dmb sy" : : : "memory");
    #endif
}

//
// Write data to a specific mailbox channel.
//
void bcm2835_mailbox_write(uint8_t channel, uint8_t core, uint32_t val) {
/* not available on the BCM2835 */
}

//
// Read data from a specific mailbox channel.
//
uint32_t bcm2835_mailbox_read(uint8_t mailbox, uint8_t core) {
/* not available on the BCM2835 */
    return 0;
}

//
// Write data to the videocore mailbox.
//
void bcm2835_mailbox_vc_write(uint32_t addres28, uint8_t channel) {
    while (MAILBOX_VC_2835->STATUS & MAIL_FULL); // Wait till the mailbox is empty
    dmb();
    MAILBOX_VC_2835->WRITE = (addres28 | (channel & 0xF)); // Combine data (28:0) with channel (3:0 bits)
}

//
// Read data from the videocore mailbox.
//
uint32_t bcm2835_mailbox_vc_read(uint8_t channel) {
    for (;;) {                              // Loop until we receive something from the requested channel
		while ((MAILBOX_VC_2835->STATUS & MAIL_EMPTY) != 0);  // Wait for data
		volatile uint32_t address = MAILBOX_VC_2835->READ; // Read the data
        dmb();                              // Make sure that the CPU will not read from its Cache, but out of RAM.
		uint8_t readChannel = address & 0xF;
		if (readChannel == channel)         // Return if it's for the requested channel
			return address & 0xFFFFFFF0;
	}
}

uint32_t bcm2835_get_firmware_version() {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Total size of the buffer in bytes
    mailbox_buffer[1] = MAIL_PROC_REQ;      // Request code (0 = request)
    mailbox_buffer[2] = 0x00000001;         // Tag: Get firmware revision
    mailbox_buffer[3] = 4;                  // Size of the value buffer
    mailbox_buffer[4] = 0;                  // Indicator (0)
    mailbox_buffer[5] = 0;                  // Data
    mailbox_buffer[6] = 0;                  // padding
    mailbox_buffer[7] = 0;                  // End tag (0x0)
    // Send the channel number and the ADDRESS of the mailbox_buffer
    // Add 0x40000000 to the ADDRESS not to use the L2 cache of the GPU
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[5];
    }
    return 0xFFFFFFFF; // Fault
}

uint32_t bcm2835_get_board_model() {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Total size of the buffer in bytes
    mailbox_buffer[1] = 0;                  // Request code (0 = request)
    mailbox_buffer[2] = 0x00010001;         // Tag: Get board model
    mailbox_buffer[3] = 4;                  // Size of the value buffer
    mailbox_buffer[4] = 0;                  // Indicator (0)
    mailbox_buffer[5] = 0;                  // Data
    mailbox_buffer[6] = 0;                  // padding
    mailbox_buffer[7] = 0;                  // End tag (0x0)
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[5];
    }
    return 0xFFFFFFFF; // Fault
}

uint32_t bcm2835_get_board_revision() {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Total size of the buffer in bytes
    mailbox_buffer[1] = MAIL_PROC_REQ;      // Request code (0 = request)
    mailbox_buffer[2] = 0x00010002;         // Tag: Get board revision
    mailbox_buffer[3] = 4;                  // Size of the value buffer
    mailbox_buffer[4] = 0;                  // Indicator (0)
    mailbox_buffer[5] = 0;                  // Data
    mailbox_buffer[6] = 0;                  // padding
    mailbox_buffer[7] = 0;                  // End tag (0x0)
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[5];
    }
    return 0xFFFFFFFF; // Fault
}

mac_addr bcm2835_get_mac_address() {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Total size of the buffer in bytes
    mailbox_buffer[1] = MAIL_PROC_REQ;      // Request code (0 = request)
    mailbox_buffer[2] = 0x00010003;         // Tag: Get MAC address
    mailbox_buffer[3] = 6;                  // Size of the return buffer
    mailbox_buffer[4] = 0;                  // Indicator (0)
    mailbox_buffer[5] = 0;                  // Data
    mailbox_buffer[6] = 0;                  // Data
    mailbox_buffer[7] = 0;                  // End tag (0x0)
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    mac_addr MAC = {0};
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        uint8_t *buffer_ptr = (uint8_t *)&mailbox_buffer[5];
        for (uint8_t i = 0; i < 6; i++) {
            MAC.bytes[i] = buffer_ptr[i];
        }        
        return MAC;
    }
    return MAC; // Fault
}

board_serial bcm2835_get_board_serial() {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Total size of the buffer in bytes
    mailbox_buffer[1] = MAIL_PROC_REQ;      // Request code (0 = request)
    mailbox_buffer[2] = 0x00010004;         // Tag: Get board serial number
    mailbox_buffer[3] = 8;                  // Size of the value buffer
    mailbox_buffer[4] = 0;                  // Indicator (0)
    mailbox_buffer[5] = 0;                  // Data
    mailbox_buffer[6] = 0;                  // Data
    mailbox_buffer[7] = 0;                  // End tag (0x0)
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    board_serial board = {0};
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        uint8_t *buffer_ptr = (uint8_t *)&mailbox_buffer[5];
        for (uint8_t i = 0; i < 8; i++) {
            board.bytes[i] = buffer_ptr[i];
        }        
        return board;
    }
    return board; // Fault
}

void bcm2835_get_arm_memory(uint32_t *base, uint32_t *size) {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Total size of the buffer in bytes
    mailbox_buffer[1] = MAIL_PROC_REQ;      // Request code (0 = request)
    mailbox_buffer[2] = 0x00010005;         // Tag: Get ARM memory
    mailbox_buffer[3] = 8;                  // Size of the value buffer
    mailbox_buffer[4] = 0;                  // Indicator (0)
    mailbox_buffer[5] = 0;                  // Data
    mailbox_buffer[6] = 0;                  // Data
    mailbox_buffer[7] = 0;                  // End tag (0x0)
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    *base = 0;
    *size = 0;
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        *base = mailbox_buffer[5];
        *size = mailbox_buffer[6];
        return;
    }
    return; // Fault 
}

uint32_t bcm2835_get_clock_rate(clock_id id) {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Buffer grootte
    mailbox_buffer[1] = MAIL_PROC_REQ;
    mailbox_buffer[2] = 0x00030002;         // Tag: Get clock rate
    mailbox_buffer[3] = 8;                  // Grootte van de value buffer (ID + Rate)
    mailbox_buffer[4] = 0;                  // Request indicator
    mailbox_buffer[5] = id;                 // INPUT: De ID (3 voor ARM, 4 voor CORE)
    mailbox_buffer[6] = 0;                  // OUTPUT: Hier komt de snelheid in Hz
    mailbox_buffer[7] = 0;                  // End tag
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[6]; // De snelheid in Hz staat in index 6
    }
    return 0;
}

void bcm2835_get_clock_rates(uint32_t *id) {
    for (clock_id nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = bcm2835_get_clock_rate(nr);
    }
}

uint32_t bcm2835_get_clock_rate_measured(clock_id id) {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Buffer grootte
    mailbox_buffer[1] = MAIL_PROC_REQ;
    mailbox_buffer[2] = 0x00030047;         // Tag: Get clock rate measured
    mailbox_buffer[3] = 8;                  // Grootte van de value buffer (ID + Rate)
    mailbox_buffer[4] = 0;                  // Request indicator
    mailbox_buffer[5] = id;                 // INPUT: De ID (3 voor ARM, 4 voor CORE)
    mailbox_buffer[6] = 0;                  // OUTPUT: Hier komt de snelheid in Hz
    mailbox_buffer[7] = 0;                  // End tag
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[6]; // De snelheid in Hz staat in index 6
    }
    return 0;
}

void bcm2835_get_clock_rates_measured(uint32_t *id) {
    for (clock_id nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = bcm2835_get_clock_rate_measured(nr);
    }
}

uint32_t bcm2835_get_max_clock_rate(clock_id id) {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Buffer grootte
    mailbox_buffer[1] = MAIL_PROC_REQ;
    mailbox_buffer[2] = 0x00030004;         // Tag: Get max clock rate
    mailbox_buffer[3] = 8;                  // Grootte van de value buffer (ID + Rate)
    mailbox_buffer[4] = 0;                  // Request indicator
    mailbox_buffer[5] = id;                 // INPUT: De ID (3 voor ARM, 4 voor CORE)
    mailbox_buffer[6] = 0;                  // OUTPUT: Hier komt de snelheid in Hz
    mailbox_buffer[7] = 0;                  // End tag
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[6]; // De snelheid in Hz staat in index 6
    }
    return 0;
}

void bcm2835_get_max_clock_rates(uint32_t *id) {
    for (clock_id nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = bcm2835_get_max_clock_rate(nr);
    }
}

uint32_t bcm2835_get_min_clock_rate(clock_id id) {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Buffer grootte
    mailbox_buffer[1] = MAIL_PROC_REQ;
    mailbox_buffer[2] = 0x00030007;         // Tag: Get min clock rate
    mailbox_buffer[3] = 8;                  // Grootte van de value buffer (ID + Rate)
    mailbox_buffer[4] = 0;                  // Request indicator
    mailbox_buffer[5] = id;                 // INPUT: De ID (3 voor ARM, 4 voor CORE)
    mailbox_buffer[6] = 0;                  // OUTPUT: Hier komt de snelheid in Hz
    mailbox_buffer[7] = 0;                  // End tag
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[6]; // De snelheid in Hz staat in index 6
    }
    return 0;
}

void bcm2835_get_min_clock_rates(uint32_t *id) {
    for (clock_id nr = EMMC_id; nr < CLOCK_SIZE; nr++) {
        id[nr] = bcm2835_get_min_clock_rate(nr);
    }
}

uint32_t bcm2835_get_soc_temperature(void) {
    mailbox_buffer[0] = sizeof(mailbox_buffer); // Buffer grootte
    mailbox_buffer[1] = MAIL_PROC_REQ;
    mailbox_buffer[2] = 0x00030007;         // Tag: Get temperature
    mailbox_buffer[3] = 8;                  // Grootte van de value buffer (ID + Rate)
    mailbox_buffer[4] = 0;                  // Request indicator
    mailbox_buffer[5] = 0;                 // INPUT: De ID (3 voor ARM, 4 voor CORE)
    mailbox_buffer[6] = 0;                  // OUTPUT: Hier komt de snelheid in Hz
    mailbox_buffer[7] = 0;                  // End tag
    bcm2835_mailbox_vc_write(8, (uint32_t)mailbox_buffer | 0x40000000);
    bcm2835_mailbox_vc_read(8);             // Wait for response 
    if (mailbox_buffer[1] == MAIL_RESP_OK) {
        return mailbox_buffer[6]; // The temperaure is in index 6
    }
    return 0;    
}

const mailbox_vc_ops_t bcm2835_mailbox_vc_ops = {
    .read   = bcm2835_mailbox_read,
    .write  = bcm2835_mailbox_write,
    .get_firmware_version = bcm2835_get_firmware_version,
    .get_board_model = bcm2835_get_board_model,
    .get_board_revision = bcm2835_get_board_revision,
    .get_mac_address = bcm2835_get_mac_address,
    .get_board_serial = bcm2835_get_board_serial,
    .get_arm_memory = bcm2835_get_arm_memory,
    .get_soc_temperature = bcm2835_get_soc_temperature,
    .get_clock_rates = bcm2835_get_clock_rates,
    .get_clock_rates_measured = bcm2835_get_clock_rates_measured,
    .get_max_clock_rates = bcm2835_get_max_clock_rates,
    .get_min_clock_rates = bcm2835_get_min_clock_rates
};

void bcm2835_mailbox_vc_init(void)
{
    mailbox_vc = &bcm2835_mailbox_vc_ops;
}
