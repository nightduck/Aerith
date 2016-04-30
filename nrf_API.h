// Define Dummy Byte for reading back a value
#define SPIM_DUMMY_BYTE			0x00

// NRF Control Bytes for Reading from Selcted Registers.
// The NRF24 write register command is 000A AAAA, where
// A AAAA is the 5-bit register address from the register
// map in section 9.1 of the datasheet.

#define NRF_READ_CONFIG			0x00
#define NRF_READ_EN_AA			0x01
#define NRF_READ_EN_RXADDR		0x02
#define NRF_READ_SETUP_AW		0x03
#define NRF_READ_SETUP_RETR		0x04
#define NRF_READ_RF_CH			0x05
#define NRF_READ_RF_SETUP		0x06
#define NRF_READ_STATUS			0x07
#define NRF_READ_RX_ADDR_P0		0x0A
#define NRF_READ_RX_ADDR_P1		0x0B
#define NRF_READ_RX_ADDR_P2		0x0C
#define NRF_READ_RX_ADDR_P3		0x0D
#define NRF_READ_RX_ADDR_P4		0x0E
#define NRF_READ_RX_ADDR_P5		0x0F
#define NRF_READ_TX_ADDR		0x10
#define NRF_READ_PW_P0			0x11
#define NRF_READ_PW_P1			0x12
#define NRF_READ_PW_P2			0x13
#define NRF_READ_PW_P3			0x14
#define NRF_READ_PW_P4			0x15
#define NRF_READ_PW_P5			0x16
#define NRF_READ_DYNPD			0x1C
#define NRF_READ_FEATURE		0x1D

// NRF Control Bytes for Writing to Selcted Registers.
// The NRF24 write register command is 001A AAAA, where
// A AAAA is the 5-bit register address from the register
// map in section 9.1 of the datasheet.

#define NRF_WRITE_CONFIG		0x20
#define NRF_WRITE_EN_AA			0x21
#define NRF_WRITE_EN_RXADDR		0x22
#define NRF_WRITE_SETUP_AW		0x23
#define NRF_WRITE_SETUP_RETR	0x24
#define NRF_WRITE_RF_CH			0x25
#define NRF_WRITE_RF_SETUP		0x26
#define NRF_WRITE_STATUS		0x27
#define NRF_WRITE_RX_ADDR_P0	0x2A
#define NRF_WRITE_RX_ADDR_P1	0x2B
#define NRF_WRITE_RX_ADDR_P2	0x2C
#define NRF_WRITE_RX_ADDR_P3	0x2D
#define NRF_WRITE_RX_ADDR_P4	0x2E
#define NRF_WRITE_RX_ADDR_P5	0x2F
#define NRF_WRITE_TX_ADDR		0x30
#define NRF_WRITE_PW_P0			0x31
#define NRF_WRITE_PW_P1			0x32
#define NRF_WRITE_PW_P2			0x33
#define NRF_WRITE_PW_P3			0x34
#define NRF_WRITE_PW_P4			0x35
#define NRF_WRITE_PW_P5			0x36
#define NRF_WRITE_DYNPD			0x3C
#define NRF_WRITE_FEATURE		0x3D

// Other control words
#define NRF_WRITE_PAYLOAD		0xA0
#define NRF_READ_PAYLOAD		0x61
#define IRQ 25
#define CSN 22
#define LOW 0
#define HIGH 1
