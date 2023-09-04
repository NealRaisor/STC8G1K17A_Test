#include "STC8G_H_Timer.h"
#include "STC8G_H_UART.h"

// extern u8 AHT21_Delay;
// extern u16 KEY_Recorder;
extern u16 ms_Recorder;
extern bit Flag_1s; // 1s

#ifdef Timer0

void Timer0_ISR_Handler(void) interrupt TMR0_VECTOR
{
    // TODO
    if (++ms_Recorder > 60000) { // %60000ms
        ms_Recorder = 0;
    }
    // 1s
    if (ms_Recorder % 1000 == 0) {
        Flag_1s = 1;
    }
    // Recv Timer
    if (COM1.RX_TimeOut > 0) {
        --COM1.RX_TimeOut;
    }
    // // AHT21延时

    // if (AHT21_Delay > 0) {
    //     --AHT21_Delay;
    // }

    // if (KEY0 == 0) {
    //     ++KEY_Recorder;
    // }
}
#endif

#ifdef Timer1

void Timer1_ISR_Handler(void) interrupt TMR1_VECTOR
{
    // TODO
    return;
}
#endif
