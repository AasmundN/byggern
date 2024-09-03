#ifndef __UART__
#define __UART__

/**
 * Initialize USART0 with RX and TX enabled
 * @param unsigned int value written to baud registers
 */
void UART_init(unsigned int baudrate);

/**
 * Transmit char on USART0, waits for TX ready
 * @param unsigned char char to be sent
 */
void UART_transmit(unsigned char ch);

/**
 * Read USART0 data register
 * @return DDR0 value
 */
unsigned char UART_receive();

/**
 * Register callback for the RX complete interrupt
 * Callback must read data register
 * @see UART_transmit()
 * @param pointer to callback
 */
void UART_rxc_register_cb(void (*cb)());

#endif // __UART__
