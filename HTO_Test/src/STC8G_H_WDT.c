#include "STC8G_H_WDT.h"

/// @brief WDT Init Function
/// @param WDT WDT_InitTypeDef ptr, Structure for initializing watchdogs
void WDT_Init(WDT_InitTypeDef *WDT)
{
    // enable WDT
    if (WDT->WDT_Enable == ENABLE)
        WDT_CONTR = WDT_EN_WDT;
    // set SCALE
    WDT_PS_Set(WDT->WDT_PS); //	WDT_SCALE_2 ... WDT_SCALE_256
    // start count
    if (WDT->WDT_IDLE_Mode == WDT_IDLE_STOP)
        WDT_CONTR &= ~0x08; // IDLE Mode Start
    else
        WDT_CONTR |= 0x08; // IDLE Mode Stop
}
