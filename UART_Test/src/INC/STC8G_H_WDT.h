#ifndef __STC8G_H_WDT_H
#define __STC8G_H_WDT_H

#include "Config.h"

//========================================================================
//                        Register bit define
//========================================================================

// sfr WDT_CONTR = 0xC1; //Watch-Dog-Timer Control register
//     7     6     5      4       3      2   1   0     Reset Value
// WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
#define WDT_WDT_FLAG (1 << 7)
#define WDT_EN_WDT   (1 << 5)
#define WDT_CLR_WDT  (1 << 4) /* auto clear	*/
#define WDT_IDLE_WDT (1 << 3) /* WDT counter when Idle	*/

// macro for WDT_CONTR
#define WDT_PS_Set(n) WDT_CONTR = (WDT_CONTR & ~0x07) | (n & 0x07)              /* WDT Scale Set */
#define WDT_Reset(n)  WDT_CONTR = WDT_EN_WDT + WDT_CLR_WDT + WDT_IDLE_WDT + (n) /* Reset WDT */

//========================================================================
//                          Define declare
//========================================================================

#define WDT_IDLE_STOP 0
#define WDT_IDLE_RUN  1

/* WDT Timeout=(12*32768*(SCALE+1))/SYSclk */
/* Fosc     SC_256     nice_SC*/
// 6MHz    16.78ms    SCALE_16
// 12MHz    8.39ms    SCALE_32
// 20MHz    5.03ms    SCALE_64
// 24MHz    4.19ms    SCALE_64
// 35MHz    2.87ms    SCALE_128
#define WDT_SCALE_2   0
#define WDT_SCALE_4   1
#define WDT_SCALE_8   2
#define WDT_SCALE_16  3
#define WDT_SCALE_32  4
#define WDT_SCALE_64  5
#define WDT_SCALE_128 6
#define WDT_SCALE_256 7

typedef struct
{
    u8 WDT_Enable;    // Enable WDT                 ENABLE,DISABLE
    u8 WDT_PS;        // WDT Scale       WDT_SCALE_2,WDT_SCALE_4,WDT_SCALE_8,WDT_SCALE_16,WDT_SCALE_32,WDT_SCALE_64,WDT_SCALE_128,WDT_SCALE_256
    u8 WDT_IDLE_Mode; // IDLE Mode stop counter            WDT_IDLE_STOP,WDT_IDLE_RUN
} WDT_InitTypeDef;

void WDT_Init(WDT_InitTypeDef *WDT);
#define WDT_Clear() (WDT_CONTR |= WDT_CLR_WDT) // feed dog

#endif
