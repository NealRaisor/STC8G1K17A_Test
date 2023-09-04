#ifndef __STC8G_H_TIMER_H
#define __STC8G_H_TIMER_H

#include "config.h"

//========================================================================
//                             Timer0 config
//========================================================================
#ifdef Timer0

#define Timer0_CLK_Select(n)                              \
    do {                                                  \
        if (n == 0) AUXR &= ~(1 << 7), TMOD &= ~(1 << 2); \
        if (n == 1) AUXR |= (1 << 7), TMOD &= ~(1 << 2);  \
        if (n == 2) TMOD |= (1 << 2);                     \
    } while (0)

#define Timer0_Mode_Set(n)   (TMOD = (TMOD & ~0x03) | n)
#define Timer0_CLK_Output(n) INTCLKO = (INTCLKO & ~0x01) | (n)
#define Timer0_Run()         TR0 = 1
#define Timer0_Stop()        TR0 = 0
#define T0_Load(n)           TH0 = (n) / 256, TL0 = (n) % 256 

#endif

//========================================================================
//                          Timer1 config
//========================================================================
#ifdef Timer1

#define Timer1_CLK_Select(n)                              \
    do {                                                  \
        if (n == 0) AUXR &= ~(1 << 6), TMOD &= ~(1 << 6); \
        if (n == 1) AUXR |= (1 << 6), TMOD &= ~(1 << 6);  \
        if (n == 2) TMOD |= (1 << 6);                     \
    } while (0)

#define Timer1_Mode_Set(n)   (TMOD = (TMOD & ~0x30) | n << 4)
#define Timer1_CLK_Output(n) INTCLKO = (INTCLKO & ~0x02) | (n << 1)
#define Timer1_Run()         TR1 = 1
#define Timer1_Stop()        TR1 = 0
#define T1_Load(n)           TH1 = (n) / 256, TL1 = (n) % 256

#endif

//========================================================================
//                          setting define
//========================================================================
// Timer0 and Timer1 mode select
#define TIM_16BitAutoReload       0
#define TIM_16Bit                 1
#define TIM_8BitAutoReload        2
#define TIM_16BitAutoReloadNoMask 3

// TimerX clk mode select
#define TIM_CLOCK_12T 0
#define TIM_CLOCK_1T  1
#define TIM_CLOCK_Ext 2

#endif
