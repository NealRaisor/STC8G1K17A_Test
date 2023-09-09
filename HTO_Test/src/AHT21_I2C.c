#include "AHT21_I2C.h"
#include "STC8G_H_Delay.h"

#define AHT21_SOFT_RESET_CMD 0xBA // Soft Reset CMD
#define AHT21_DATA_LEN       6    // 数据长度

// 仅本文件使用
static u8 xdata AHT21_Data_Buf[AHT21_DATA_LEN]; // AHT21 data buf
static u32 xdata Tmp   = 0;                     // temp save var
static bit Flag_UnRead = 0;                     // AHT21 read data flag bit 0: Read over 1: Didn't read

// 异步延时，定时器0中减少，本文件中设置值以及读取值
u8 data AHT21_Delay = 0; // dalay time var, Added in this document and reduced in timer interrupt function.

//==================================================================================================//
//                                          初始化函数
//==================================================================================================//
// Get Status Byte
static u8 AHT21_Get_Status(void)
{
    u8 status = 0;
    I2C_ReadNBytes(AHT21_ADDR, I2C_NO_MEM_ADDR, &status, 1);
    return status;
}

// Init Function
void AHT21_Init(void)
{
    u8 status = 0;

    status = AHT21_SOFT_RESET_CMD;
    I2C_WriteNBytes(AHT21_ADDR, I2C_NO_MEM_ADDR, &status, 1); // 发送重启命令
    delay_ms(20);                                             // 阻塞延时20ms

    status = AHT21_Get_Status(); // 判断校准位
    if (AHT21_STATUS_CAL_OVER != (status & AHT21_STATUS_CAL_OVER)) {
        AHT21_Data_Buf[0] = 0xBE;
        AHT21_Data_Buf[1] = 0x08;
        AHT21_Data_Buf[2] = 0x00;
        I2C_WriteNBytes(AHT21_ADDR, I2C_NO_MEM_ADDR, AHT21_Data_Buf, 3);
        AHT21_Delay = 20;
    }
}

//==================================================================================================//
//                                          主要函数
//==================================================================================================//

// Start measure function
void AHT21_Measure(void)
{
    // 判断是否正在测量或未读取
    if ((AHT21_Delay > 0) || (Flag_UnRead == 1)) {
        return;
    }
    // 测量命令
    AHT21_Data_Buf[0] = 0xAC;
    AHT21_Data_Buf[1] = 0x33;
    AHT21_Data_Buf[2] = 0x00;
    // 发送命令
    I2C_WriteNBytes(AHT21_ADDR, I2C_NO_MEM_ADDR, AHT21_Data_Buf, 3);
    AHT21_Delay = 80; // 延时
    Flag_UnRead = 1;  // 设置标志位
}

// Read data Function
void AHT21_Read_Data(AHT21_DATA *R_T)
{
    // 判断是否正在测量
    if (AHT21_Delay > 0 || (Flag_UnRead == 0)) {
        return;
    }
    // 读取数据
    I2C_ReadNBytes(AHT21_ADDR, I2C_NO_MEM_ADDR, AHT21_Data_Buf, AHT21_DATA_LEN);
    // 判断忙位
    if (AHT21_Data_Buf[0] & AHT21_STATUS_BUSY) {
        return;
    }
    // 清除标志位
    Flag_UnRead = 0;

    // 计算湿度
    Tmp = AHT21_Data_Buf[1];
    Tmp <<= 8;
    Tmp |= AHT21_Data_Buf[2];
    Tmp <<= 4;
    Tmp |= (AHT21_Data_Buf[3] >> 4);
    // 湿度 = (RH * 100)/2^20
    R_T->Humi = (u8)((25 * Tmp) >> 18);

    // 计算温度
    Tmp = (AHT21_Data_Buf[3] & 0x0F);
    Tmp <<= 8;
    Tmp |= AHT21_Data_Buf[4];
    Tmp <<= 8;
    Tmp |= AHT21_Data_Buf[5];
    // 温度 = (T * 200)/2^20 - 50
    R_T->Temp = (i8)((25 * Tmp) >> 17) - 50;
}