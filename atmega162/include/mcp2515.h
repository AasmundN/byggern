#ifndef __MCP2515__
#define __MCP2515__

#include <stdint.h>

/**
 * The following routines implement the MCP2515 SPI interface
 * See the datasheet for detailed documentation
 */

void MCP2515_init();

void MCP2515_read(uint8_t addr, uint8_t data_buffer[], int buffer_size);

void MCP2515_write(uint8_t addr, uint8_t data_buffer[], int buffer_size);

void MCP2515_rts(int buffer_select_bits);

uint8_t MCP2515_read_status();

void MCP2515_bit_mod(uint8_t addr, uint8_t mask, uint8_t data);

void MCP2515_reset();

#endif // __MCP2515__
