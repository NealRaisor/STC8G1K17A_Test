#include "System_config.h"

u8 xdata Deal_Buf[COM1_RX_Len];

u16 ms_Recorder = 0; // 毫秒计数
bit Flag_1s     = 0; // 1s标志

void main(void)
{
    u8 num, i;
    System_config();
    delay_ms(2000);
    printf("System init OK!\r\n");

    while (1) {
        num = Read_RX_BUF(&COM1, RX1_Buffer, Deal_Buf);
        if (num > 0) {
            printf("num:%bd\r\n", num);
            for (i = 0; i < num; ++i) {
                UART_SendByte(Deal_Buf[i]);
            }
        }
    }
}
