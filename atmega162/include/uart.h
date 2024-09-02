#ifndef __UART__
#define __UART__
  
void UART_init(unsigned int baudrate);

void UART_transmit(unsigned char ch);

void UART_transmit_string(char *string);

unsigned char UART_receive();

void UART_rxc_register_cb(void (*cb)());

#endif // __UART__
