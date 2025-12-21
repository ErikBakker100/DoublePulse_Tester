#pragma once
#include <stdint.h>

//
// System Timer, base address at mmio_base + 0x003000
//
typedef struct {
    volatile uint32_t CS;                   // 0x00 System Timer Control/Status
    volatile uint32_t CLO;                  // 0x04 System Timer Counter Lower 32 bits
    volatile uint32_t CHI;                  // 0x08 System Timer Counter Higher 32 bits
    volatile uint32_t C[4];                 // 0x0C-0x18 System Timer Compare 0-3
} sys_timer_regs_t;
extern volatile sys_timer_regs_t *TMR;

//
// Interrupt, base address at mmio_base + 0x00B200
//
typedef struct {
    volatile uint32_t IRQ_BASIC_PENDING;    // 0x00
    volatile uint32_t IRQ_PENDING[2];       // 0x04 - 0x08
    volatile uint32_t FIQ_CONTROL;          // 0x0C
    volatile uint32_t ENABLE_IRQS[2];       // 0x10 - 0x14
    volatile uint32_t ENABLE_IRQS_BASIC;    // 0x18    
    volatile uint32_t DISABLE_IRQS[2];      // 0x1C - 0x20
    volatile uint32_t DISABLE_IRQS_BASIC;   // 0x24   
} int_regs_t;
extern volatile int_regs_t *INT;

#define IRQ_ARM_TIMER           (1 << 0);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_MAILBOX         (1 << 1);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_DOORBELL_0      (1 << 2);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_DOORBELL_1      (1 << 3);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_GPU0_HALTED     (1 << 4);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_GPU1_HALTED     (1 << 5);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_ILLEGAL_TYPE0   (1 << 6);   // Used in the ENABLE_IRQS_BASIC register
#define IRQ_ARM_ILLEGAL_TYPE1   (1 << 7);   // Used in the ENABLE_IRQS_BASIC register

/*
#define TIMER_IRQ_PENDING       (1 << 0);   // Used in the IRQ_PENDING register
#define MAILBOX_IRQ_PENDING     (1 << 1);   // Used in the IRQ_PENDING register
#define DOORBELL0_IRQ_PENDING   (1 << 2);   // Used in the IRQ_PENDING register
#define DOORBELL1_IRQ_PENDING   (1 << 3);   // Used in the IRQ_PENDING register
#define GPU0_HALTED_IRQ_PENDING (1 << 4);   // Used in the IRQ_PENDING register
#define GPU1_HALTED_IRQ_PENDING (1 << 5);   // Used in the IRQ_PENDING register
*/

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
} arm_timer_regs_t;
extern volatile arm_timer_regs_t *ARM_TMR;



//
// 3 x Clock Manager General Purpose Clocks Control, base address at mmio_base + 0x101070
//
typedef struct {
    volatile uint32_t CM_GPCTL;             // 0x00 General Purpose Clock Control
    volatile uint32_t CM_GPDIV;             // 0x04 General Purpose Clock Divisor
} gp_clk_regs_t;

typedef struct {
    gp_clk_regs_t GP[3];               // 0x00 - 0x14
} gp_clks_regs_t;
extern volatile gp_clks_regs_t *GPIO_CLKS;

//
// General Purpose I/O (GPIO), base address at mmio_base + 0x200000
//
typedef struct {
    /* 0x00–0x14: Function Select Registers */
    volatile uint32_t FSEL[6];     // 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14
    uint32_t reserved0;            // 0x18
    volatile uint32_t SET[2];      // 0x1C–0x20: Pin Output Set
    uint32_t reserved1;            // 0x24
    volatile uint32_t CLR[2];      // 0x28–0x2C: Pin Output Clear
    uint32_t reserved2;            // 0x30
    volatile uint32_t LEV[2];      // 0x34–0x38: Pin Level
    uint32_t reserved3;            // 0x3C
    volatile uint32_t EDS[2];      // 0x40–0x44: Event Detect Status
    uint32_t reserved4;            // 0x48
    volatile uint32_t REN[2];      // 0x4C–0x50: Rising Edge Detect Enable
    uint32_t reserved5;            // 0x54
    volatile uint32_t FEN[2];      // 0x58–0x5C: Falling Edge Detect Enable
    uint32_t reserved6;            // 0x60
    volatile uint32_t HEN[2];      // 0x64–0x68: High Detect Enable
    uint32_t reserved7;            // 0x6C
    volatile uint32_t LEN[2];      // 0x70–0x74: Low Detect Enable
    uint32_t reserved8;            // 0x78
    volatile uint32_t AREN[2];     // 0x7C–0x80: Async Rising Edge Detect
    uint32_t reserved9;            // 0x84
    volatile uint32_t AFEN[2];     // 0x88–0x8C: Async Falling Edge Detect
    uint32_t reserved10;           // 0x90
    volatile uint32_t PUD;         // 0x94–0x9C: Pull-up/down + Clocks
    volatile uint32_t PUDCLK[2];   // 0x98, 0x9C
} gpio_regs_t;
extern volatile gpio_regs_t *GPIO;

//
// PL011 UART, base address at mmio_base + 0x201000
//
typedef struct {
    volatile uint32_t DR;           // 0x00 Data Register
    volatile uint32_t RSR_ECR;      // 0x04 Receive Status / Error Clear Register
    uint32_t reserved1[4];          // 0x08 - 0x14
    volatile uint32_t FR;           // 0x18 Flag Register
    uint32_t reserved2;             // 0x1C
    volatile uint32_t ILPR;         // 0x20 IrDA Low-Power Register
    volatile uint32_t IBRD;         // 0x24 Integer Baud Rate Register
    volatile uint32_t FBRD;         // 0x28 Fractional Baud Rate Register
    volatile uint32_t LCRH;         // 0x2C Line Control Register
    volatile uint32_t CR;           // 0x30 Control Register
    volatile uint32_t IFLS;         // 0x34 Interrupt FIFO Level Select Register
    volatile uint32_t IMSC;         // 0x38 Interrupt Mask Set/Clear Register
    volatile uint32_t RIS;          // 0x3C Raw Interrupt Status Register
    volatile uint32_t MIS;          // 0x40 Masked Interrupt Status Register
    volatile uint32_t ICR;          // 0x44 Interrupt Clear Register
    volatile uint32_t DMACR;        // 0x48 DMA Control Register
    volatile uint32_t ITCR;         // 0x4C Test Control Register
    volatile uint32_t ITIP;         // 0x50 Integration Test Input Register
    volatile uint32_t ITOP;         // 0x54 Integration Test Output Register
    volatile uint32_t TDR;          // 0x58 Test Data Register
}uart_regs_t;
extern volatile uart_regs_t *UART;

//
// SDHOST Controller (SD Card), base address at mmio_base + 0x202000
//
typedef struct {
}sdhost_regs_t;
extern volatile sdhost_regs_t *SDHOST;

//
// PCM audio interface, base address at mmio_base + 0x203000
//
typedef struct {
}pcm_regs_t;
extern volatile pcm_regs_t *PCM;

//
// SPI Serial Peripheral Interface, base address at mmio_base + 0x204000
//
typedef struct {
    volatile uint32_t CS;          // 0x00 Control and Status
    volatile uint32_t FIFO;        // 0x04 Master TX and RX FIFOs 
    volatile uint32_t CLK;         // 0x08 Clock Divider
    volatile uint32_t DLEN;        // 0x0C Data Length
    volatile uint32_t LTOH;        // 0x10 Lossi TOH
    volatile uint32_t DC;          // 0x14 DMA DREQ Controls
}spi_regs_t;
extern volatile spi_regs_t *SPI;

//
// I2C1, base address at mmio_base + 0x205000
//
typedef struct {
    volatile uint32_t CS;           // 0x00 Control
    volatile uint32_t SR;           // 0x04 Status
    volatile uint32_t DLEN;         // 0x08 Data Length
    volatile uint32_t A;            // 0x0C Slave Address
    volatile uint32_t FIFO;         // 0x10 Data FIFO
    volatile uint32_t DIV;          // 0x14 Clock Divider
    volatile uint32_t DEL;          // 0x18 Data Delay
    volatile uint32_t CLKT;         // 0x1C Clock Stretch Timeout
} i2c1_regs_t;
extern volatile i2c1_regs_t *I2C1;

//
// Pulse Width Modulator, base address at mmio_base + 0x20C000
//
typedef struct {
    volatile uint32_t CS;           // 0x00 Control
    volatile uint32_t SR;           // 0x04 Status
    volatile uint32_t DMAC;         // 0x08 DMA Configuration
    uint32_t reserved1[1];
    volatile uint32_t RNG1;         // 0x10 Channel 1 Range
    volatile uint32_t DAT1;         // 0x14 Channel 1 Data
    volatile uint32_t FIF1;         // 0x18 Clock Divider
    uint32_t reserved2[1];
    volatile uint32_t RNG2;         // 0x20 Channel 2 Range
    volatile uint32_t DAT2;         // 0x1C Channel 2 Data 
} pwm_regs_t;
extern volatile pwm_regs_t *PWM;

//
// I2C/SPI SLAVE, base address at mmio_base + 0x214000
//
typedef struct {
    volatile uint32_t DR;           // 0x00 Data
    volatile uint32_t RSR;          // 0x04 Operation status register and error clear register
    volatile uint32_t SLV;          // 0x08 The I2C SPI Address Register holds the I2C slave address value
    volatile uint32_t CR;           // 0x0C The Control register is used to configure the I2C or SPI operation
    volatile uint32_t FR;           // 0x10 Flag register
    volatile uint32_t IFLS;         // 0x14 Interrupt fifo level select register
    volatile uint32_t IMSC;         // 0x18 Channel 2 Range
    volatile uint32_t RIS;          // 0x1C Interupt Mask Set Clear Register
    volatile uint32_t MIS;          // 0x20 Masked Interrupt Status Register
    volatile uint32_t ICR;          // 0x24 Interrupt Clear Register
    volatile uint32_t DMACR;        // 0x28 DMA Control Register
    volatile uint32_t TDR;          // 0x2C FIFO Test Data
    volatile uint32_t GPUSTAT;      // 0x30 GPU Status
    volatile uint32_t HCTRL;        // 0x34 Host Control
    volatile uint32_t DEBUG1;       // 0x38 I2C Debug Register
    volatile uint32_t DEBUG2;       // 0x3C SPI Debug Register
} i2c_spi_slave_regs_t;
extern volatile i2c_spi_slave_regs_t *I2C_SPI_SLAVE;

//
// Mini UART, base address at mmio_base + 0x215000
//
typedef struct {
    volatile uint32_t IRQ;              // 0x00 Auxiliary Interrupt status
    volatile uint32_t ENABLES;          // 0x04 Auxiliary enables
    uint32_t reserved1[14];
    volatile uint32_t MU_IO_REG;        // 0x40 I/O Data
    volatile uint32_t MU_IER_REG;       // 0x44 Interrupt Enable
    volatile uint32_t MU_IIR_REG;       // 0x48 Interrupt Identify/FIFO Enable
    volatile uint32_t MU_LCR_REG;       // 0x4C Line Control
    volatile uint32_t MU_MCR_REG;       // 0x50 Modem Control
    volatile uint32_t MU_LSR_REG;       // 0x54 Line Status
    volatile uint32_t MU_MSR_REG;       // 0x58 Modem Status
    volatile uint32_t MU_SCRATCH;       // 0x5C Scratch
    volatile uint32_t MU_CNTL_REG;      // 0x60 Control
    volatile uint32_t MU_STAT_REG;      // 0x64 Status
    volatile uint32_t MU_BAUD_REG;      // 0x68 Baudrate
} mu_regs_t;
extern volatile mu_regs_t *MU;          // Mini UART base address pointer

//
// SPI1, Universal SPI Master, base address at mmio_base + 0x215000 + 0x80
//
typedef struct {
    volatile uint32_t SPI0_CNTL0_REG;   // 0x80 SPI 1 Control register 0
    volatile uint32_t SPI0_CNTL1_REG;   // 0x84 SPI 1 Control register 1
    volatile uint32_t SPI0_STAT_REG;    // 0x88 SPI 1 Status register
    uint32_t reserved3[1];
    volatile uint32_t SPI0_IO_REG;      // 0x90 SPI 1 Data
    volatile uint32_t SPI0_PEEK_REG;    // 0x94 SPI 1 Peek
} spi1_regs_t;
extern volatile spi1_regs_t *SPI1;      // SPI 1 base address pointer

//
// SPI2, base address at mmio_base + 0x215000 + 0xC0
//
typedef struct {
    volatile uint32_t SPI1_CNTL0_REG;   // 0xC0 SPI 2 Control register 0
    volatile uint32_t SPI1_CNTL1_REG;   // 0xC4 SPI 2 Control register 1
    volatile uint32_t SPI1_STAT_REG;    // 0xC8 SPI 2 Status register
    uint32_t reserved5[1];
    volatile uint32_t SPI1_IO_REG;      // 0xD0 SPI 2 Data
    volatile uint32_t SPI1_PEEK_REG;    // 0xD4 SPI 2 Peek
} spi2_regs_t;
extern volatile spi2_regs_t *SPI2;      // SPI 2 base address pointer

//
// External Mass Media Controller (SD Card), base address at mmio_base + 0x300000
//
typedef struct {
    volatile uint32_t ARG2;           // 0x00 Argument 2
    volatile uint32_t BLKSIZECNT;     // 0x04 Block Size and Count
    volatile uint32_t ARG1;           // 0x08 Argument 1
    volatile uint32_t CMDTM;          // 0x0C Command and Transfer Mode
    volatile uint32_t RESP0;          // 0x10 Response 0
    volatile uint32_t RESP1;          // 0x14 Response 1
    volatile uint32_t RESP2;          // 0x18 Response 2
    volatile uint32_t RESP3;          // 0x1C Response 3
    volatile uint32_t DATA;           // 0x20 Data
    volatile uint32_t STATUS;         // 0x24 Status
    volatile uint32_t CONTROL0;       // 0x28 Control 0
    volatile uint32_t CONTROL1;       // 0x2C Control 1
    volatile uint32_t INTERRUPT;      // 0x30 Interrupt
    volatile uint32_t IRPT_MASK;      // 0x34 Interrupt Mask
    volatile uint32_t IRPT_EN;        // 0x38 Interrupt Enable
    volatile uint32_t CONTROL2;       // 0x3C Control 2
    volatile uint32_t FORCE_IRPT;     // 0x40 Force Interrupt
    volatile uint32_t BOOT_TIMEOUT;   // 0x44 Boot Timeout
    volatile uint32_t DBG_SEL;        // 0x48 Debug Select
    volatile uint32_t EXRDFIFO_CFG;   // 0x4C Extra Read FIFO Configuration
    volatile uint32_t TUNE_STEP;      // 0x50 Tune Step
    volatile uint32_t TUNE_STEPS_STD; // 0x54 Tune Steps for Standard Speed
    volatile uint32_t TUNE_STEPS_DDR; // 0x58 Tune Steps for DDR Mode
} emmc_regs_t;
extern volatile emmc_regs_t *EMMC;    // EMMC base address pointer

//
// I2C2, base address at mmio_base + 0x804000
//
typedef struct {
    volatile uint32_t CS;            // 0x00 Control
    volatile uint32_t SR;            // 0x04 Status
    volatile uint32_t DLEN;         // 0x08 Data Length
    volatile uint32_t A;            // 0x0C Slave Address
    volatile uint32_t FIFO;         // 0x10 Data FIFO
    volatile uint32_t DIV;          // 0x14 Clock Divider
    volatile uint32_t DEL;          // 0x18 Data Delay
    volatile uint32_t CLKT;         // 0x1C Clock Stretch Timeout
} i2c2_regs_t;
extern volatile i2c2_regs_t *I2C2;

//
// I2C3, base address at mmio_base + 0x805000
//
typedef struct {
    volatile uint32_t CS;           // 0x00 Control
    volatile uint32_t SR;           // 0x04 Status
    volatile uint32_t DLEN;         // 0x08 Data Length
    volatile uint32_t A;            // 0x0C Slave Address
    volatile uint32_t FIFO;         // 0x10 Data FIFO
    volatile uint32_t DIV;          // 0x14 Clock Divider
    volatile uint32_t DEL;          // 0x18 Data Delay
    volatile uint32_t CLKT;         // 0x1C Clock Stretch Timeout
} i2c3_regs_t;
extern volatile i2c3_regs_t *I2C3;

void BCM2835_Init(uintptr_t mmio_base);
// Initializes the BCM2835 peripherals base address pointers
// Must be called once during system initialization with the correct mmio_base address