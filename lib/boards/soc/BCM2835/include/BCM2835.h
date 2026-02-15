#pragma once
#include <stdint.h>
#include "DWC_OTG_CORE_IF.h"
#include "../../include/soc.h"
//
// System Timer, base address at mmio_base + 0x003000
//
typedef struct {
    volatile uint32_t CS;                   // 0x00 System Timer Control/Status
    volatile uint32_t CLO;                  // 0x04 System Timer Counter Lower 32 bits
    volatile uint32_t CHI;                  // 0x08 System Timer Counter Higher 32 bits
    volatile uint32_t C[4];                 // 0x0C-0x18 System Timer Compare 0-3
} bcm2835_sys_timer_regs_t;
extern bcm2835_sys_timer_regs_t *SYS_TMR_2835;

//
// DMA Direct Memory Access, base address at mmio_base + 0x007000 - 0x007F00
//
/*  15 DMA Controller Registers
    
    Each DMA channel of a particular type has an identical register map, only the base address of each channel is different.
    There is a global enable register at the top of the Address map that can disable each DMA for powersaving.
    Only three registers in each channel’s register set are directly writeable (CS, CONBLK_AD and DEBUG). The other registers
    (TI, SOURCE_AD, DEST_AD, TXFR_LEN, STRIDE & NEXTCONBK) are automatically loaded from a Control Block data
    structure held in external memory. */

/*  Control Blocks (CB) are 8 words (256 bits) in length and must start at a 256-bit aligned address. The format of the
    different CB data structures in memory, are shown below.
    Each 32-bit word of the Control Block is automatically loaded into the corresponding 32-bit DMA Control Block register at
    the start of a DMA transfer. The descriptions of these registers also define the corresponding bit locations in the CB data
    structure in memory.*/

typedef struct {
    volatile uint32_t TI;                   // 0x00 Transfer Information
    volatile uint32_t SOURCE_AD;            // 0x04 Source Address
    volatile uint32_t DEST_AD;              // 0x08 Destination Address
    volatile uint32_t TXFR_LEN;             // 0x0C Transfer length
    volatile uint32_t STRIDE;               // 0x10 2D Mode Stride
    volatile uint32_t NEXTCONBK;            // 0x14 Next Control Block Address
    uint32_t reserved[2];                   // 0x18-1C Reserved – set to zero
} bcm2835_dma_ctrl_blck_t;

typedef struct {
    volatile uint32_t TI;                   // 0x00 Transfer Information
    volatile uint32_t SOURCE_AD;            // 0x04 Source Address
    volatile uint32_t DEST_AD;              // 0x08 Destination Address
    volatile uint32_t TXFR_LEN;             // 0x0C Transfer length
    uint32_t reserved0;                     // 0x10 Reserved – set to zero.
    volatile uint32_t NEXTCONBK;            // 0x14 Next Control Block Address
    uint32_t reserved1[2];                  // 0x18-1C Reserved – set to zero
} bcm2835_dma_lite_ctrl_blck_t;

typedef struct {
    volatile uint32_t TI;                   // 0x00 Transfer Information
    volatile uint32_t SOURCE_AD;            // 0x04 Source Address
    volatile uint32_t SOURCE_I;             // 0x08 Source Information
    volatile uint32_t DEST_AD;              // 0x0C Transfer length
    volatile uint32_t DEST_I;               // 0x10 Destination Information
    volatile uint32_t TXFR_LEN;             // 0x14 Transfer length
    volatile uint32_t NEXTCONBK;            // 0x18 Next Control Block Address
    uint32_t reserved0;                     // 0x1C Reserved – set to zero
} bcm2835_dma_4_ctrl_blck_t;

typedef struct {
    volatile uint32_t CS;                   // 0x000 Control and Status
    volatile uint32_t CONBLK_AD;            // 0x004 Control Block Address
    volatile uint32_t TI;                   // 0x008 CB Word 0 Transfer Information
    volatile uint32_t SOURCE_AD;            // 0x00C CB Word 1 Source Address
    volatile uint32_t DEST_AD;              // 0x010 CB Word 2 Destination Address
    volatile uint32_t TXFR_LEN;             // 0x014 CB Word 3 Transfer length
    volatile uint32_t STRIDE;               // 0x018 CB Word 4 2D Mode Stride
    volatile uint32_t NEXTCONBK;            // 0x01C CB Word 5 Next Control Block Address
    volatile uint32_t DEBUG;                // 0x020 Debug
} bcm2835_dma_ctrl_regs_t;
extern volatile bcm2835_dma_ctrl_regs_t *DMA00_2835;
extern volatile bcm2835_dma_ctrl_regs_t *DMA01_2835;
extern volatile bcm2835_dma_ctrl_regs_t *DMA02_2835;
extern volatile bcm2835_dma_ctrl_regs_t *DMA03_2835;
extern volatile bcm2835_dma_ctrl_regs_t *DMA04_2835;
extern volatile bcm2835_dma_ctrl_regs_t *DMA05_2835;
extern volatile bcm2835_dma_ctrl_regs_t *DMA06_2835;

typedef struct {
    volatile uint32_t CS;                   // 0x000 Control and Status
    volatile uint32_t CONBLK_AD;            // 0x004 Control Block Address
    volatile uint32_t TI;                   // 0x008 CB Word 0 Transfer Information
    volatile uint32_t SOURCE_AD;            // 0x00C CB Word 1 Source Address
    volatile uint32_t DEST_AD;              // 0x010 CB Word 2 Destination Address
    volatile uint32_t TXFR_LEN;             // 0x014 CB Word 3 Transfer length
    uint32_t reserved0;                     // 0x10 Reserved – set to zero
    volatile uint32_t NEXTCONBK;            // 0x01C CB Word 5 Next Control Block Address
    volatile uint32_t DEBUG;                // 0x020 Debug
} bcm2835_dma_lite_ctrl_regs_t;
extern volatile bcm2835_dma_lite_ctrl_regs_t *DMA07_2835;
extern volatile bcm2835_dma_lite_ctrl_regs_t *DMA08_2835;
extern volatile bcm2835_dma_lite_ctrl_regs_t *DMA09_2835;
extern volatile bcm2835_dma_lite_ctrl_regs_t *DMA10_2835;

typedef struct {
    volatile uint32_t CS;                   // 0x000 Control and Status
    volatile uint32_t CONBLK_AD;            // 0x004 Control Block Address
    uint32_t reserved0;                     // 0x008 Reserved – set to zero
    volatile uint32_t DEBUG;                // 0x00C Debug
    volatile uint32_t TI;                   // 0x010 CB Word 0 Transfer Information
    volatile uint32_t SOURCE_AD;            // 0x014 CB Word 1 Source Address [31:0]
    volatile uint32_t SOURCE_ADI;           // 0x018 CB Word 2 Source Address [40:32] and Info
    volatile uint32_t DEST_AD;              // 0x01C CB Word 3 Destination Address[31:0]
    volatile uint32_t DEST_ADI;             // 0x020 CB Word 4 Destination Address[40:32] and Info
    volatile uint32_t TXFR_LEN;             // 0x024 CB Word 5 Transfer length
    volatile uint32_t NEXTCONBK;            // 0x028 CB Word 6 Next Control Block Address
    volatile uint32_t DEBUG2;               // 0x02C Debug
} bcm2835_dma_4_ctrl_regs_t;
extern volatile bcm2835_dma_4_ctrl_regs_t *DMA11_2835;
extern volatile bcm2835_dma_4_ctrl_regs_t *DMA12_2835;
extern volatile bcm2835_dma_4_ctrl_regs_t *DMA13_2835;
extern volatile bcm2835_dma_4_ctrl_regs_t *DMA14_2835;
extern volatile bcm2835_dma_4_ctrl_regs_t *DMA15_2835;
//
// DMA irq status register, base address at mmio_base + 0x007FE0
//
typedef struct {
    volatile uint32_t INT_STATUS;           // 0xFE0 Interrupt status of each DMA channel
} bcm2835_dma_irq_stat_reg_t;
extern volatile bcm2835_dma_irq_stat_reg_t *DMAIRQ_2835;

//
// DMA Enable register, base address at mmio_base + 0x007FF0
//
typedef struct {
    volatile uint32_t ENABLE;               // 0xFF0 Global enable bits for each DMA channel
} bcm2835_dma_enable_reg_t;
extern volatile bcm2835_dma_enable_reg_t *DMAEN_2835;

//
// Interrupt controller, base address at mmio_base + 0x00B200
//
typedef struct {
    volatile uint32_t IRQ_BASIC_PENDING;    // 0x00
    volatile uint32_t IRQ_PENDING[2];       // 0x04 - 0x08
    volatile uint32_t FIQ_CONTROL;          // 0x0C
    volatile uint32_t ENABLE_IRQS[2];       // 0x10 - 0x14
    volatile uint32_t ENABLE_IRQS_BASIC;    // 0x18    
    volatile uint32_t DISABLE_IRQS[2];      // 0x1C - 0x20
    volatile uint32_t DISABLE_IRQS_BASIC;   // 0x24   
#define IRQ_ARM_TIMER           (1 << 0);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_MAILBOX         (1 << 1);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_DOORBELL_0      (1 << 2);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_DOORBELL_1      (1 << 3);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_GPU0_HALTED     (1 << 4);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_GPU1_HALTED     (1 << 5);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_ILLEGAL_TYPE0   (1 << 6);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_ILLEGAL_TYPE1   (1 << 7);   // Used in the ENABLE_IRQS_BASIC register
} bcm2835_ic_regs_t;
extern volatile bcm2835_ic_regs_t *IC_2835;

//
// ARM Timer, base address at mmio_base + 0x00B400
//
typedef struct {
    volatile uint32_t LOAD;                 // 0x00 Load Register
    volatile uint32_t VALUE;                // 0x04 Value Register
    volatile uint32_t CONTROL;              // 0x08 Control Register
    volatile uint32_t IRQ_CLEAR;            // 0x0C IRQ Clear/Acknowledge Register
    volatile uint32_t RAW_IRQ;              // 0x10 Raw IRQ Register
    volatile uint32_t MASKED_IRQ;           // 0x14 Masked IRQ Register
    volatile uint32_t RELOAD;               // 0x18 Reload Register
    volatile uint32_t PRE_DIVIDER;          // 0x1C Pre-divider Register
    volatile uint32_t FREE_RUNNING_CNT;     // 0x20 Free running counter
} bcm2835_arm_timer_regs_t;
extern volatile bcm2835_arm_timer_regs_t *ARM_TMR_2835;

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
} bcm2835_mailbox_vc_regs_t;
extern bcm2835_mailbox_vc_regs_t *MAILBOX_VC_2835;

//
// 3 x Clock Manager General Purpose Clocks Control, base address at mmio_base + 0x101070
//
typedef struct {
    volatile uint32_t CM_GPCTL;             // 0x00 General Purpose Clock Control
    volatile uint32_t CM_GPDIV;             // 0x04 General Purpose Clock Divisor
} bcm2835_gp_clk_regs_t;

typedef struct {
    bcm2835_gp_clk_regs_t GP[3];                    // 0x00 - 0x14
} bcm2835_gp_clks_regs_t;
extern volatile bcm2835_gp_clks_regs_t *GPIO_CLKS_2835;

//
// General Purpose I/O (GPIO), base address at mmio_base + 0x200000
//
typedef struct {
    /* 0x00–0x14: Function Select Registers */
    volatile uint32_t FSEL[6];              // 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14
    uint32_t reserved0;                     // 0x18
    /* 0x1C–0x20: Pin Output Set */
    volatile uint32_t SET[2];               // 0x1C–0x20: Pin Output Set
    uint32_t reserved1;                     // 0x24
    /* 0x28–0x2C: Pin Output Clear */
    volatile uint32_t CLR[2];               // 0x28–0x2C: Pin Output Clear
    uint32_t reserved2;                     // 0x30
    /* 0x34–0x38: Pin Level */
    volatile uint32_t LEV[2];               // 0x34–0x38: Pin Level
    uint32_t reserved3;                     // 0x3C
    /* 0x40–0x44: Event Detect Status */
    volatile uint32_t EDS[2];               // 0x40–0x44: Event Detect Status
    uint32_t reserved4;                     // 0x48
    /* 0x4C–0x50: Rising Edge Detect Enable */
    volatile uint32_t REN[2];               // 0x4C–0x50: Rising Edge Detect Enable
    uint32_t reserved5;                     // 0x54
    /* 0x58–0x5C: Falling Edge Detect Enable */
    volatile uint32_t FEN[2];               // 0x58–0x5C: Falling Edge Detect Enable
    uint32_t reserved6;                     // 0x60
    /* 0x64–0x68: High Detect Enable */
    volatile uint32_t HEN[2];               // 0x64–0x68: High Detect Enable
    uint32_t reserved7;                     // 0x6C
    /* 0x70–0x74: Low Detect Enable */
    volatile uint32_t LEN[2];               // 0x70–0x74: Low Detect Enable
    uint32_t reserved8;                     // 0x78
    /* 0x7C–0x80: Async Rising Edge Detect */
    volatile uint32_t AREN[2];              // 0x7C–0x80: Async Rising Edge Detect
    uint32_t reserved9;                     // 0x84
    /* 0x88–0x8C: Async Falling Edge Detect */
    volatile uint32_t AFEN[2];              // 0x88–0x8C: Async Falling Edge Detect
    uint32_t reserved10;                    // 0x90
    volatile uint32_t PUD;                  // 0x94 Pull-up/down + Clocks
    volatile uint32_t PUDCLK[2];            // 0x98, 0x9C
    uint32_t reserved11[4];                 // 0xA0-0xAC
    volatile uint32_t TEST;                 // 0xB0 Test
} bcm2835_gpio_regs_t;
extern bcm2835_gpio_regs_t *GPIO_2835;

//
// PL011 UART0, base address at mmio_base + 0x201000
//
typedef struct {
    volatile uint32_t DR;                   // 0x00 Data Register
    volatile uint32_t RSR_ECR;              // 0x04 Receive Status / Error Clear Register
    uint32_t reserved1[4];                  // 0x08 - 0x14
    volatile uint32_t FR;                   // 0x18 Flag Register
    uint32_t reserved2[1];                  // 0x1C
    volatile uint32_t ILPR;                 // 0x20 IrDA Low-Power Register
    volatile uint32_t IBRD;                 // 0x24 Integer Baud Rate Register
    volatile uint32_t FBRD;                 // 0x28 Fractional Baud Rate Register
    volatile uint32_t LCRH;                 // 0x2C Line Control Register
    volatile uint32_t CR;                   // 0x30 Control Register
    volatile uint32_t IFLS;                 // 0x34 Interrupt FIFO Level Select Register
    volatile uint32_t IMSC;                 // 0x38 Interrupt Mask Set/Clear Register
    volatile uint32_t RIS;                  // 0x3C Raw Interrupt Status Register
    volatile uint32_t MIS;                  // 0x40 Masked Interrupt Status Register
    volatile uint32_t ICR;                  // 0x44 Interrupt Clear Register
    volatile uint32_t DMACR;                // 0x48 DMA Control Register
    uint32_t reserved3[13];                 // 0x4C - 7C
    volatile uint32_t ITCR;                 // 0x80 Test Control Register
    volatile uint32_t ITIP;                 // 0x84 Integration Test Input Register
    volatile uint32_t ITOP;                 // 0x88 Integration Test Output Register
    volatile uint32_t TDR;                  // 0x8C Test Data Register
} bcm2835_uart_regs_t;
extern volatile bcm2835_uart_regs_t *UART0_2835;

//
// SDHOST Controller (SD Card), base address at mmio_base + 0x202000
//
typedef struct {
} bcm2835_sdhost_regs_t;
extern volatile bcm2835_sdhost_regs_t *SDHOST_2835;

//
// PCM audio interface, base address at mmio_base + 0x203000
//
typedef struct {
    volatile uint32_t CS_A;                 // 0x00 Control and Status
    volatile uint32_t FIFO_A;               // 0x04 FIFO Data
    volatile uint32_t MODE_A;               // 0x08 Mode
    volatile uint32_t RXC_A;                // 0x0C Receive Configuration
    volatile uint32_t TXC_A;                // 0x10 Transmit Configuration
    volatile uint32_t DREQ_A;               // 0x14 DMA Request Level
    volatile uint32_t INTEN_A;              // 0x18 Interrupt Enables
    volatile uint32_t INTSTC_A;             // 0x1C Interrupt Status & Clear
    volatile uint32_t GRAY;                 // 0x20 Gray Mode Control
} bcm2835_pcm_regs_t;
extern volatile bcm2835_pcm_regs_t *PCM_2835;

//
// SPI Serial Peripheral Interface, base address at mmio_base + 0x204000
//
typedef struct {
    volatile uint32_t CS;                   // 0x00 Control and Status
    volatile uint32_t FIFO;                 // 0x04 Master TX and RX FIFOs 
    volatile uint32_t CLK;                  // 0x08 Clock Divider
    volatile uint32_t DLEN;                 // 0x0C Data Length
    volatile uint32_t LTOH;                 // 0x10 Lossi TOH
    volatile uint32_t DC;                   // 0x14 DMA DREQ Controls
} bcm2835_spi_regs_t;
extern volatile bcm2835_spi_regs_t *SPI0_2835;

//
// I2C0, base address at mmio_base + 0x205000
//
typedef struct {
    volatile uint32_t CS;                   // 0x00 Control
    volatile uint32_t SR;                   // 0x04 Status
    volatile uint32_t DLEN;                 // 0x08 Data Length
    volatile uint32_t A;                    // 0x0C Slave Address
    volatile uint32_t FIFO;                 // 0x10 Data FIFO
    volatile uint32_t DIV;                  // 0x14 Clock Divider
    volatile uint32_t DEL;                  // 0x18 Data Delay
    volatile uint32_t CLKT;                 // 0x1C Clock Stretch Timeout
} bcm2835_i2c_regs_t;
extern volatile bcm2835_i2c_regs_t *I2C0_2835;

//
// Pulse Width Modulator, base address at mmio_base + 0x20C000
//
typedef struct {
    volatile uint32_t CTL;                   // 0x00 Control
    volatile uint32_t STA;                   // 0x04 Status
    volatile uint32_t DMAC;                 // 0x08 DMA Configuration
    uint32_t reserved1[1];
    volatile uint32_t RNG1;                 // 0x10 Channel 1 Range
    volatile uint32_t DAT1;                 // 0x14 Channel 1 Data
    volatile uint32_t FIF1;                 // 0x18 Clock Divider
    uint32_t reserved2[1];
    volatile uint32_t RNG2;                 // 0x20 Channel 2 Range
    volatile uint32_t DAT2;                 // 0x1C Channel 2 Data 
} bcm2835_pwm_regs_t;
extern volatile bcm2835_pwm_regs_t *PWM0_2835;

//
// I2C/SPI SLAVE, base address at mmio_base + 0x214000
//
typedef struct {
    volatile uint32_t DR;                   // 0x00 Data
    volatile uint32_t RSR;                  // 0x04 Operation status register and error clear register
    volatile uint32_t SLV;                  // 0x08 The I2C SPI Address Register holds the I2C slave address value
    volatile uint32_t CR;                   // 0x0C The Control register is used to configure the I2C or SPI operation
    volatile uint32_t FR;                   // 0x10 Flag register
    volatile uint32_t IFLS;                 // 0x14 Interrupt fifo level select register
    volatile uint32_t IMSC;                 // 0x18 Channel 2 Range
    volatile uint32_t RIS;                  // 0x1C Interupt Mask Set Clear Register
    volatile uint32_t MIS;                  // 0x20 Masked Interrupt Status Register
    volatile uint32_t ICR;                  // 0x24 Interrupt Clear Register
    volatile uint32_t DMACR;                // 0x28 DMA Control Register
    volatile uint32_t TDR;                  // 0x2C FIFO Test Data
    volatile uint32_t GPUSTAT;              // 0x30 GPU Status
    volatile uint32_t HCTRL;                // 0x34 Host Control
    volatile uint32_t DEBUG1;               // 0x38 I2C Debug Register
    volatile uint32_t DEBUG2;               // 0x3C SPI Debug Register
} bcm2835_i2c_spi_slave_regs_t;
extern volatile bcm2835_i2c_spi_slave_regs_t *I2C_SPI_SLAVE_2835;

//
// AUX registers, base address at mmio_base + 0x215000
//
typedef struct {
    volatile uint32_t IRQ;                  // 0x00 Auxiliary Interrupt status
    volatile uint32_t ENABLES;              // 0x04 Auxiliary enables
} bcm2835_aux_regs_t;
extern bcm2835_aux_regs_t *AUX_2835;

//
// Mini UART, base address at mmio_base + 0x215040
//
typedef struct {
    volatile uint32_t MU_IO;                // 0x40 I/O Data
    volatile uint32_t MU_IER;               // 0x44 Interrupt Enable
    volatile uint32_t MU_IIR;               // 0x48 Interrupt Identify/FIFO Enable
    volatile uint32_t MU_LCR;               // 0x4C Line Control
    volatile uint32_t MU_MCR;               // 0x50 Modem Control
    volatile uint32_t MU_LSR;               // 0x54 Line Status
    volatile uint32_t MU_MSR;               // 0x58 Modem Status
    volatile uint32_t MU_SCRATCH;           // 0x5C Scratch
    volatile uint32_t MU_CNTL;              // 0x60 Control
    volatile uint32_t MU_STAT;              // 0x64 Status
    volatile uint32_t MU_BAUD;              // 0x68 Baudrate
} bcm2835_mu_regs_t;
extern bcm2835_mu_regs_t *MU_2835;              // Mini UART base address pointer

//
// SPI0, Universal SPI Master, base address (BA) at mmio_base + 0x215000 + 0x80
// SPI1, base address at mmio_base + 0x215000 + 0xC0
//
typedef struct {
    volatile uint32_t CNTL0;           // 0x00 SPI Control register 0
    volatile uint32_t CNTL1;           // 0x04 SPI Control register 1
    volatile uint32_t STAT;            // 0x08 SPI Status register
    uint32_t reserved0[1];
    volatile uint32_t IO;              // 0x10 SPI Data
    volatile uint32_t PEEK;            // 0x14 SPI Peek
} bcm2835_aux_spi_regs_t;
extern volatile bcm2835_aux_spi_regs_t *SPI1_2835; // SPI 1 base address pointer
extern volatile bcm2835_aux_spi_regs_t *SPI2_2835; // SPI 2 base address pointer

//
// External Mass Media Controller (SD Card), base address at mmio_base + 0x300000
//
typedef struct {
    volatile uint32_t ARG2;                 // 0x00 Argument 2
    volatile uint32_t BLKSIZECNT;           // 0x04 Block Size and Count
    volatile uint32_t ARG1;                 // 0x08 Argument 1
    volatile uint32_t CMDTM;                // 0x0C Command and Transfer Mode
    volatile uint32_t RESP0;                // 0x10 Response 0
    volatile uint32_t RESP1;                // 0x14 Response 1
    volatile uint32_t RESP2;                // 0x18 Response 2
    volatile uint32_t RESP3;                // 0x1C Response 3
    volatile uint32_t DATA;                 // 0x20 Data
    volatile uint32_t STATUS;               // 0x24 Status
    volatile uint32_t CONTROL0;             // 0x28 Control 0
    volatile uint32_t CONTROL1;             // 0x2C Control 1
    volatile uint32_t INTERRUPT;            // 0x30 Interrupt
    volatile uint32_t IRPT_MASK;            // 0x34 Interrupt Mask
    volatile uint32_t IRPT_EN;              // 0x38 Interrupt Enable
    volatile uint32_t CONTROL2;             // 0x3C Control 2
    volatile uint32_t FORCE_IRPT;           // 0x40 Force Interrupt
    volatile uint32_t BOOT_TIMEOUT;         // 0x44 Boot Timeout
    volatile uint32_t DBG_SEL;              // 0x48 Debug Select
    volatile uint32_t EXRDFIFO_CFG;         // 0x4C Extra Read FIFO Configuration
    volatile uint32_t TUNE_STEP;            // 0x50 Tune Step
    volatile uint32_t TUNE_STEPS_STD;       // 0x54 Tune Steps for Standard Speed
    volatile uint32_t TUNE_STEPS_DDR;       // 0x58 Tune Steps for DDR Mode
} bcm2835_emmc_regs_t;
extern volatile bcm2835_emmc_regs_t *EMMC_2835;          // EMMC base address pointer

//
// I2C1, base address at mmio_base + 0x804000
//
extern volatile bcm2835_i2c_regs_t *I2C1_2835;

//
// USB Controller, base address at mmio_base + 0x980000
//
typedef struct {
// For core registers see DWC_OTG_CORE_IF.h
    dwc_otg_core_global_regs_t_p1 core_p1;
    volatile uint32_t USB_MDIO_CNTL;        // 0x080 MDIO interface control
    volatile uint32_t USB_MDIO_GEN;         // 0x084 Data for MDIO interface
    volatile uint32_t USB_VBUS_DRV;         // 0x088 Vbus and other Miscellaneous controls
    uint32_t reserved[29];                  // 0x08C-0x0FC
    dwc_otg_core_global_regs_t_p2 core_p2;
}  bcm2835_usb_regs_t;
extern volatile bcm2835_usb_regs_t *USB_2835;
// Initializes the BCM2836 peripherals base address pointers
void BCM2835_init(const peripheral_t *);
