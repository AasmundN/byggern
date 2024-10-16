#ifndef __MCP2515__
#define __MCP2515__

/**
 * The following routines implement the MCP2515 SPI interface
 * See the datasheet for detailed documentation
 */

void MCP2515_init();

void MCP2515_read(char addr, char data_buffer[], int buffer_size);

void MCP2515_write(char addr, char data_buffer[], int buffer_size);

void MCP2515_rts(int buffer_select_bits);

char MCP2515_read_status();

void MCP2515_bit_mod(char addr, char mask, char data);

void MCP2515_reset();

#endif // __MCP2515__
