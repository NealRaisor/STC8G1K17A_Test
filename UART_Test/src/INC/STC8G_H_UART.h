#ifndef __STC8G_H_UART_H
#define __STC8G_H_UART_H

#include "Config.h"

//========================================================================
//                        UART Mode Settable Item
//========================================================================
#define UART_ShiftRight  0
#define UART_8bit_BRTx   (1 << 6)
#define UART_9bit        (2 << 6)
#define UART_9bit_BRTx   (3 << 6)
#define UART_Mode_Set(n) (SCON = (SCON & 0x3f) | n)

//===========================================================================//
//                         UART1 constant define
//===========================================================================//
#define COM1_TX_Len            64
#define COM1_RX_Len            32
#define TimeOutSet1            3

#define UART1_BRT_Use_Timer1() TMOD &= ~0x40, AUXR &= ~0x01
#define UART1_RxEnable(n)      (n == 0 ? (REN = 0) : (REN = 1))

//==========================================================================//
//                      UART Info Struct Define
//==========================================================================//
typedef struct
{
    u8 TX_send;
    u8 TX_write;
    u8 B_TX_busy;

    u8 RX_Cnt;     // Counter
    u8 RX_TimeOut; // TimeOut Recorder
} COMx_Define;

#define DATA_HEAD_LEN 4    // Dataframe head len
#define START_CODE_0  0xEF // Dataframe 0 start key
#define START_CODE_1  0x10 // Dataframe 0 start key
#define DATA_LEN_IDX  2    // Data length index in Dataframe head
#define DATA_CKS_IDX  3    // Data checksum index in Dataframe head

//==========================================================================//
//                        UART1 Var Define
//==========================================================================//
extern COMx_Define COM1;
extern u8 xdata RX1_Buffer[COM1_RX_Len];
extern u8 xdata TX1_Buffer[COM1_TX_Len];

//==========================================================================//
//                        Function Declare
//==========================================================================//
void UART_SendByte(u8 Byte);
void UART_SendStr(u8 *Str);
u8 Read_RX_BUF(COMx_Define *COMx, u8 *RX_BUF, u8 *DEAL_BUF);
char putchar(char c); // printf redirect function

//==========================================================================//

#endif
