#include "STC8G_H_UART.h"

//========================================================================
//                           Define Declaration
//========================================================================
// 共享数据
COMx_Define COM1 = {0};
u8 xdata TX1_Buffer[COM1_TX_Len]; // send buffer
u8 xdata RX1_Buffer[COM1_RX_Len]; // receive buffer

// 仅本文件使用
static u8 data cnt = 0, i = 0;

//========================================================================
/// @brief Read once receive data buffer
/// @param COMx     COM1,COM2,COM3,COM4
/// @param RX_BUF   Receive buffer
/// @param DEAL_BUF Deal buffer
/// @return Length of data read this time
u8 Read_RX_BUF(COMx_Define *COMx, u8 *RX_BUF, u8 *DEAL_BUF)
{
    if ((COMx->RX_Cnt == 0) || (COMx->RX_TimeOut > 0)) {
        return 0;
    }
    // One Read
    i = 0, cnt = COMx->RX_Cnt; // Ensure data integrity at one time.
    do {
        DEAL_BUF[i] = RX_BUF[i];
        ++i;
    } while (--cnt);
    COMx->RX_Cnt = 0; // clear Buf, wait New Data.
    return i;
}

//========================================================================
/// @brief Send a byte of data from the serial port.
/// @param Byte One byte of data to be sent
void UART_SendByte(u8 Byte)
{
    TX1_Buffer[COM1.TX_write] = Byte;
    if (++COM1.TX_write >= COM1_TX_Len) {
        COM1.TX_write = 0;
    }

    if (COM1.B_TX_busy == 0) {
        COM1.B_TX_busy = 1;
        TI             = 1;
    }
}

//========================================================================
/// @brief Send a string from a serial port.
/// @param Str  String pointer
void UART_SendStr(u8 *Str)
{
    while (*Str) {
        UART_SendByte(*Str);
        ++Str;
    }
}

//========================================================================
/// @brief printf redirect call function
/// @param ch   one char
/// @return this char
char putchar(char ch)
{
    UART_SendByte((u8)ch);
    return ch;
}