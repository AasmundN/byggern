#include "can.h"
#include "sam.h"

#define txMailbox 0
#define rxMailbox 1

void CAN_printmsg(CanMsg m)
{
  printf("CanMsg(id:%d, length:%d, data:{", m.id, m.length);

  if (m.length)
    printf("%d", m.byte[0]);

  for (uint8_t i = 1; i < m.length; i++)
    printf(", %d", m.byte[i]);

  printf("})\r\n");
}

void CAN_init(CanInit_t init)
{
  // Disable CAN
  CAN0->CAN_MR &= ~CAN_MR_CANEN;

  // Clear status register by reading it
  __attribute__((unused)) uint32_t ul_status = CAN0->CAN_SR;

  // Disable interrupts on CANH and CANL pins
  PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;

  // Select CAN0 RX and TX in PIOA
  PIOA->PIO_ABSR &= ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);

  // Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral
  // controller can use them
  PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;

  // Enable pull up on CANH and CANL pin
  PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);

  // Enable Clock for CAN0 in PMC
  // DIV = 1 (can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
  PMC->PMC_PCR = PMC_PCR_EN | (0 /*??*/ << PMC_PCR_DIV_Pos) | PMC_PCR_CMD
                 | (ID_CAN0 << PMC_PCR_PID_Pos);
  PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);

  // Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match
  // on all nodes!
  CAN0->CAN_BR = init.reg;

  // Configure mailboxes
  // transmit
  CAN0->CAN_MB[txMailbox].CAN_MID &= ~CAN_MID_MIDE;
  CAN0->CAN_MB[txMailbox].CAN_MMR = CAN_MMR_MOT_MB_TX;

  // receive
  CAN0->CAN_MB[rxMailbox].CAN_MAM = 0; // Accept all messages
  CAN0->CAN_MB[rxMailbox].CAN_MID &= ~CAN_MID_MIDE;
  CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
  CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;

  // Enable CAN
  CAN0->CAN_MR |= CAN_MR_CANEN;
}

void CAN_tx(CanMsg m)
{
  while (!(CAN0->CAN_MB[txMailbox].CAN_MSR & CAN_MSR_MRDY))
    ;

  // Set message ID and use CAN 2.0A protocol
  CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDvA(m.id) & ~CAN_MID_MIDE;

  // Coerce maximum 8 byte length
  m.length = m.length > 8 ? 8 : m.length;

  //  Put message in can data registers
  CAN0->CAN_MB[txMailbox].CAN_MDL = m.dword[0];
  CAN0->CAN_MB[txMailbox].CAN_MDH = m.dword[1];

  // Set message length and mailbox ready to send
  CAN0->CAN_MB[txMailbox].CAN_MCR
      = (m.length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
}

uint8_t CAN_rx(CanMsg *m)
{
  if (!(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY))
    return 1;

  // Get message ID
  m->id = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MID & CAN_MID_MIDvA_Msk)
                    >> CAN_MID_MIDvA_Pos);

  // Get data length
  m->length = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MDLC_Msk)
                        >> CAN_MSR_MDLC_Pos);

  // Get data from CAN mailbox
  m->dword[0] = CAN0->CAN_MB[rxMailbox].CAN_MDL;
  m->dword[1] = CAN0->CAN_MB[rxMailbox].CAN_MDH;

  // Reset for new receive
  CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
  CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;

  return 0;
}
