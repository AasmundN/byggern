#include "can.h"
#include "sam.h"
#include <stdarg.h>
#include <stdio.h>

// Import UART from Node 2 starter code, then edit include path accordingly.
// Also, remember to update the makefile
#include "uart.h"

#define F_CPU 84000000
#define BAUDRATE 9600

CanInit_t bit_timing = {
  .phase2 = 3,
  .propag = 3,
  .phase1 = 3,
  .sjw = 0,
  .brp = 51,
  .smp = 0
};

CanMsg send_can =  {
  .id = 1,
  .length = 8,
  .byte = "amobius"
};

CanMsg receive_can;

int main ()
{
  SystemInit ();

  WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

  uart_init (F_CPU, BAUDRATE);
  can_init(bit_timing, 0);

  while (1)
    {
      //while (can_rx(&receive_can));

      //can_printmsg(receive_can);
      can_tx(send_can);
    }
}