#ifndef __UART__
#define __UART__

#define UART_BAUD(fcpu, baud) (fcpu / 16 / baud - 1)

/**
 * Initialize USART0 with RX and TX enabled
 * @param baudrate value written to baud registers
 */
void UART_init(unsigned int baudrate);

/**
 * Transmit char on USART0, waits for TX ready
 * @param ch char to be sent
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
 * @see UART_receive()
 * @param cb pointer to callback
 */
void UART_set_RX_CMPLT_cb(void (*cb)());

#endif // __UART__
