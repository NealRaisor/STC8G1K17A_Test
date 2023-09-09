#ifndef __SSD1315_I2C_H
#define __SSD1315_I2C_H

#include "STC8G_H_Delay.h"
#include "STC8G_H_I2C.h"

#define SSD1315_ADDR 0x78 // SSD1315 device addr

#define SSD1315_CMD  0x00 // SSD1315 cmd
#define SSD1315_DATA 0x40 // SSD1315 data

#define OFF_BYTE     0x00 // SSD1315 off
#define ON_BYTE      0xFF // SSD1315 on

#define SCR_Wide     128 // wide
#define SCR_High     64  // high
#define SCR_Page     8   // height

//=======================  main function =======================//
void OLED_Init(void);
void OLED_Set_Pos(u8 seg, u8 page);
void OLED_Fill(u8 fill_data);
void OLED_ShowText(u8 seg, u8 page, u8 *chr, u8 font_size);

//======================= extend function ======================//
// void OLED_ColorTurn(u8 i);
// void OLED_R180();
// void OLED_R360();
// void OLED_DisPlay_On(void);
// void OLED_DisPlay_Off(void);

#endif
