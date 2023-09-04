#include "STC8G_H_UART.h"

//========================================================================
//					UART1 Interrupt Callback Function
//========================================================================

void UART1_ISR_Handler(void) interrupt UART1_VECTOR
{
    if (RI) {
        RI = 0;
        if (COM1.RX_Cnt >= COM1_RX_Len) return;
        RX1_Buffer[COM1.RX_Cnt++] = SBUF;
        COM1.RX_TimeOut           = TimeOutSet1;
    }

    if (TI) {
        TI = 0;
        if (COM1.TX_send != COM1.TX_write) {
            SBUF = TX1_Buffer[COM1.TX_send++];
            if (COM1.TX_send >= COM1_TX_Len) 
                COM1.TX_send = 0;
        } else {
            COM1.B_TX_busy = 0;
        }
    }
}
