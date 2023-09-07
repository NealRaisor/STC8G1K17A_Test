#include "STC8G_H_Timer.h"
#include "STC8G_H_UART.h"

// 外部计数声明
// extern u16 KEY_Recorder;

// 外部延时声明
extern u8 AHT21_Delay;

// 外部标志声明
extern bit Flag_ENS160;
extern bit Flag_AHT21;

//==================================================================
#ifdef Timer0

static u16 data ms_Recorder = 0; // 毫秒计数

void Timer0_ISR_Handler(void) interrupt TMR0_VECTOR
{
    // TODO
    if (++ms_Recorder > 60000) ms_Recorder = 0; // 取整

    //============================ 标志设置 ===================================
    // 1s
    if (ms_Recorder % 1000 == 0) Flag_ENS160 = 1;

    // 500ms
    if (ms_Recorder % 500 == 0) Flag_AHT21 = 1;

    //============================ 延时自减 ===================================
    // Recv Timer
    if (COM1.RX_TimeOut > 0) --COM1.RX_TimeOut;

    // AHT21延时
    if (AHT21_Delay > 0) --AHT21_Delay;

    //============================ 持续计数 ===================================
    // if (KEY0 == 0) ++KEY_Recorder;
}
#endif

#ifdef Timer1

void Timer1_ISR_Handler(void) interrupt TMR1_VECTOR
{
    // TODO
    return;
}
#endif
