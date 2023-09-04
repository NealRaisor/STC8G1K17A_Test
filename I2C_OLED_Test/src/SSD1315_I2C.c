#include "SSD1315_I2C.h"
#include "SSD1315_Font.h"

static const u8 code SSD1315_Init_CMD[] = {
    0xAE, // 关闭显示
    0x20, // 设置内存地址模式
    0x02, // 0，水平寻址模式1，垂直寻址模式 2，页寻址模式（默认） 3，无
    0xB0, // 设置页起始地址, b0~b7
    0xC8, // 设置COM扫描方向, 0xc0上下反置 0xc8正常
    0x00, // 设置低列地址, 0x00~0x0f
    0x10, // 设置高列地址, 0x10~0xf0
    0x40, // 设置显示开始行, 0~63
    0x81, // 设置对比度控
    0xFF, // 00~FF, 默认 7F, 对比度设置范
    0xA1, // 设置段重定义, 0xa0左右反置 0xa1正常显示
    0xA8, // 设置多路复用
    0x3F, // 16~64,默认 64, 复用比设置范
    0xD3, // 设置显示偏移
    0x00, // 00~3F, 默认 00, 偏移设置范围
    0xD5, // 设置显示时钟分频/振荡器频
    0x80, // 00~FF, 默认 80, 分频设置范围
    0xD9, // 设置预充电周
    0xF1, // 00~FF, 默认 22, 周期设置范围
    0xDA, // 设置COM引脚硬件配置
    0x12, // 00~FF, 默认 12, 引脚设置范围
    0xDB, // 设置VCOMH
    0x30, // 00~FF, 默认 20, VCOMH设置范围
    0x8D, // 设置电荷
    0x14, // 00~FF, 默认 14, 电荷泵设置范
    0xA4, // 设置显示全局模式, 0xa4正常显示 0xa5全局显示
    0xA6, // 设置正常/反显, 0xa6正常显示 0xa7反显显示
    0xAF, // 打开显示
};

static u8 data offset_c = 0, *tmp_ptr = NULL;
static u8 xdata tmp_buf[3];

// Implement the following necessary basic functions ↓ !!!
// ----> I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, &Data, Data_len);

/*--------main	diplay	base	func---------*/

/// @brief Set Pos
/// @param seg from 0 to 127
/// @param page from 0 to 7
void OLED_Set_Pos(u8 seg, u8 page)
{
    tmp_buf[0] = 0xB0 + page;
    tmp_buf[1] = 0x0F & seg;
    tmp_buf[2] = 0x10 | (seg >> 4);
    I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, tmp_buf, 3);
}

/// @brief Full screen filling Func
/// @param fill_data full data. (0 - 0xFF)
void OLED_Fill(u8 fill_data)
{
    u8 i, n;
    for (i = 0; i < SCR_Page; ++i) {
        OLED_Set_Pos(0, i);
        I2C_Start();                // I2C start
        I2C_SendByte(SSD1315_ADDR); // send addr
        I2C_RecvACK();
        I2C_SendByte(SSD1315_DATA); // send cmd type
        I2C_RecvACK();
        for (n = 0; n < SCR_Wide; ++n) { // I2C send data
            I2C_SendByte(fill_data);
            I2C_RecvACK();
        }
        I2C_Stop(); // send over
    }
}
/// @brief OLED Init Function
/// @param None
void OLED_Init(void)
{
    delay_ms(20);
    I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, SSD1315_Init_CMD, sizeof(SSD1315_Init_CMD));
    OLED_Fill(OFF_BYTE); // clear
}

/// @brief Standard specific font display function
/// @param seg   from 0 to 127
/// @param page  from 0 to 7
/// @param chr   *uint8 string (end with \0)
static void Show_Font0608(u8 seg, u8 page, u8 *chr)
{
    // Limit the display range
    while ((*chr >= ' ') && (*chr <= '~')) {
        if (seg > (SCR_Wide - ASCII_0608_WIDE)) { // \n
            seg = 0;
            ++page;
            OLED_Set_Pos(seg, page); // set display post
        }
        offset_c = (*chr) - ' ';         // Compute index
        tmp_ptr  = ascii_0608[offset_c]; // skip index data
        I2C_WriteNBytes(SSD1315_ADDR, SSD1315_DATA, tmp_ptr, ASCII_0608_WIDE);
        seg += ASCII_0608_WIDE; // dispaly offset
        ++chr;                  // ptr offset
    }
}

/// @brief display string
/// @param seg from 0 to 127
/// @param page from 0 to 7
/// @param chr string ptr (end with \0)
/// @param font_size font size
void OLED_ShowText(u8 seg, u8 page, u8 *chr, u8 font_size)
{
    // Limit the display range.
    if (seg > SCR_Wide || page > SCR_Page)
        return;
    // setting display post
    OLED_Set_Pos(seg, page);
    // 6*8 font
    if (font_size == 8)
        Show_Font0608(seg, page, chr);
}

// /*--------------------- 特殊功能实现 ---------------------------*/
// // 反显显示
// void OLED_ColorTurn(u8 i)
// {
//     if (i == 0) {
//         tmp_buf[0]= 0xA6; // 正常显示
//     }
//     if (i == 1) {
//         tmp_buf[0]= 0xA7; // 反显显示
//     }
//  I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, tmp_buf, 1);
// }

// // 180度
// void OLED_R180(void)
// {
//     tmp_buf[0] = 0xC0;
//     tmp_buf[1] = 0xA0;
//     I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, tmp_buf, 2);
// }

// // 正常显示
// void OLED_R360(void)
// {
//     tmp_buf[0] = 0xC8;
//     tmp_buf[1] = 0xA1;
//     I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, tmp_buf, 2);
// }

// // OLED on
// void OLED_DisPlay_On(void)
// {
//     tmp_buf[0] = 0x8D;
//     tmp_buf[1] = 0x14;
//     tmp_buf[2] = 0xAF;
//     I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, tmp_buf, 3);
// }

// // OLED off
// void OLED_DisPlay_Off(void)
// {
//     tmp_buf[0] = 0x8D;
//     tmp_buf[1] = 0x10;
//     tmp_buf[2] = 0xAE;
//     I2C_WriteNBytes(SSD1315_ADDR, SSD1315_CMD, tmp_buf, 3);
// }