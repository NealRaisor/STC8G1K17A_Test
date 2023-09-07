#include "ENS160_I2C.h"

static u8 xdata Buf[2];

//========================================================================
// 常驻函数(8 位机用指针传参, 32位机返回具体数值)
//========================================================================

//==================================================================================================//
//                                          初始化函数
//==================================================================================================//
/*  Mode参数说明
 *  ENS160_OPMODE_DEEP_SLEEP = 0x00
 *  ENS160_OPMODE_IDLE = 0x01
 *  ENS160_OPMODE_STANDBY = 0x02
 *  ENS160_OPMODE_RESET = 0xF0
 */
void ENS160_OPMODE_Set(u8 Mode)
{
    I2C_WriteNBytes(ENS160_ADDR, ENS160_OPMODE, &Mode, 1);
}

// INT中断口配置函数
void ENS160_CONFIG_INTERRUPT_Set(u8 Config)
{
    // 保留有效的 01x3x56x 位
    Config |= 0x02;
    I2C_WriteNBytes(ENS160_ADDR, ENS160_CONFIG, &Config, 1);
}

void ENS160_Init()
{
    ENS160_OPMODE_Set(ENS160_OPMODE_STANDBY);
    delay_ms(20);
    ENS160_CONFIG_INTERRUPT_Set(0);
    delay_ms(20);
}

//==================================================================================================//
//                                     读取主要数据函数
//==================================================================================================//

void ENS160_Get_DATA_AQI(u8 *AQI)
{
    I2C_ReadNBytes(ENS160_ADDR, ENS160_DATA_AQI, AQI, 1);

    *AQI = *AQI & 0x07; // 仅保留有效的低3位
    if (*AQI > 5) *AQI = 5;
}

void ENS160_Get_DATA_TVOC(u16 *TVOC)
{
    I2C_ReadNBytes(ENS160_ADDR, ENS160_DATA_TVOC, Buf, 2);

    *TVOC = Buf[1]; // add clear var
    *TVOC = (*TVOC << 8) | Buf[0];
}

void ENS160_Get_DATA_ECO2(u16 *ECO2)
{
    I2C_ReadNBytes(ENS160_ADDR, ENS160_DATA_ECO2, Buf, 2);

    *ECO2 = Buf[1]; // add clear var
    *ECO2 = (*ECO2 << 8) | Buf[0];
}
//============================= 次要函数 ===========================================

// 只有在IDLE模式下才能正常读取状态
void ENS160_Get_DEVICE_STATUS(u8 *Dev_Status)
{
    I2C_ReadNBytes(ENS160_ADDR, ENS160_DEVICE_STATUS, Dev_Status, 1);
}

//=========================================================================================
// 以下函数为可选函数
//=========================================================================================
// 附带命令函数部分
#if ENS160_COMMAND_ENABLE == 1

// 设置命令
//   命令: ENS160_COMMAND_NOP
//        ENS160_COMMAND_GET_APP_VERSION
//        ENS160_COMMAND_CLR_GPR
//   返回：0：成功，1：失败
//   失败原因：命令错误
u8 ENS160_COMMAND_Set(u8 Command)
{
    if ((Command == ENS160_COMMAND_NOP) ||
        (Command == ENS160_COMMAND_GET_APP_VERSION) ||
        (Command == ENS160_COMMAND_CLR_GPR)) {
        I2C_WriteNBytes(ENS160_ADDR, ENS160_COMMAND, &Command, 1);
        return 0;
    }
    return 1; // 命令错误
}
#endif

// 温湿度模块函数部分，次要数据
#if ENS160_DATA_T_RH_ENABLE == 1

void ENS160_TEMP_IN_Set(u16 *Temp)
{
    u8 Temp_IN[2];
    u16 Tmp    = ((u16)(*Temp + 273)) << 6;
    Temp_IN[0] = Tmp & 0xFF;
    Temp_IN[1] = (Tmp & 0xFF00) >> 8;
    I2C_WriteNBytes(ENS160_ADDR, ENS160_TEMP_IN, Temp_IN, 2);
}

void ENS160_RH_IN_Set(u16 *RH)
{
    u8 RH_IN[2];
    u16 Tmp  = *RH << 9;
    RH_IN[0] = Tmp & 0xFF;
    RH_IN[1] = (Tmp & 0xFF00) >> 8;
    I2C_WriteNBytes(ENS160_ADDR, ENS160_RH_IN, RH_IN, 2);
}

void ENS160_Get_DATA_T(i8 *Temp)
{
    u16 Tmp = 0;
    u8 Buf[2];
    I2C_ReadNBytes(ENS160_ADDR, ENS160_DATA_T, Buf, 2);
    Tmp   = (Buf[0] | (Buf[1] << 8));
    *Temp = (i8)((Tmp >> 6) - 273);
}

void ENS160_Get_DATA_RH(u8 *RH)
{
    u16 Tmp = 0;
    u8 Buf[2];
    I2C_ReadNBytes(ENS160_ADDR, ENS160_DATA_RH, Buf, 2);
    Tmp = (Buf[0] | (Buf[1] << 8));
    *RH = (u8)(Tmp >> 9);
}

#endif

//=====================================================================
// 与数据传输校验相关函数
//=====================================================================
#if ENS160_DATA_MISR_ENABLE == 1
// // 先同步MISR值
// u8 misr = ENS160_Get_DATA_MISR();
// // 定义接收缓存
// u8 buf[x];
/*
    读取寄存器 0x20 ~ 0x37 内数据的操作
*/
// //
// if (ENS160_MISR_CRC(buf, x, misr)) {
//   // 数据错误
// }
// // 数据正确

u8 ENS160_Get_DATA_MISR()
{
    u8 MISR = 0;
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DATA_MISR, &MISR, 1);
    return MISR;
}

u8 ENS160_MISR_CRC(u8 *Buf, u8 Len, u8 Begin_MISR)
{
    u8 End_MISR = ENS160_Get_DATA_MISR();
    u8 i;

    for (i = 0; i < Len; i++) {
        u8 misr_xor = ((Begin_MISR << 1) ^ Buf[i]) & 0xFF;
        if (misr_xor & 0x80) {
            Begin_MISR = misr_xor ^ ENS160_DATA_MISR_POLY;
        } else {
            Begin_MISR = misr_xor;
        }
    }

    if (End_MISR == Begin_MISR) {
        return 0;
    }

    return 1;
}

// 启用MISR功能后对应的校验函数。
// 统一返回值为0表示成功，1表示失败。
u8 ENS160_Get_DEVICE_STATUS_MISR(u8 *Dev_Status)
{
    u8 misr = ENS160_Get_DATA_MISR();
    u8 Buf[1];
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DEVICE_STATUS, Buf, 1);
    if (ENS160_MISR_CRC(Buf, 1, misr)) {
        // 数据错误
        return 1;
    }
    *Dev_Status = Buf[0];
    return 0;
}

u8 ENS160_Get_DATA_AQI_MISR(u8 *AQI)
{
    u8 misr = ENS160_Get_DATA_MISR();
    u8 Buf[1];
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DATA_AQI, Buf, 1);
    if (ENS160_MISR_CRC(Buf, 1, misr)) {
        // 数据错误
        return 1;
    }
    *AQI = Buf[0] & 0x07; // 仅保留低3位
    return 0;
}

u8 ENS160_Get_DATA_TVOC_MISR(u16 *TVOC)
{
    u8 misr = ENS160_Get_DATA_MISR();
    u8 Buf[2];
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DATA_TVOC, Buf, 2);
    if (ENS160_MISR_CRC(Buf, 2, misr)) {
        // 数据错误
        return 1;
    }
    *TVOC = Buf[0] | (Buf[1] << 8);
    return 0;
}

u8 ENS160_Get_DATA_ECO2_MISR(u16 *ECO2)
{
    u8 misr = ENS160_Get_DATA_MISR();
    u8 Buf[2];
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DATA_ECO2, Buf, 2);
    if (ENS160_MISR_CRC(Buf, 2, misr)) {
        // 数据错误
        return 1;
    }
    *ECO2 = Buf[0] | (Buf[1] << 8);
    return 0;
}

#if (ENS160_DATA_T_RH_ENABLE == 1)

u8 ENS160_Get_DATA_T_MISR(i8 *Temp)
{
    u8 misr = ENS160_Get_DATA_MISR();
    u8 Buf[2];
    u16 Tmp = 0;
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DATA_T, Buf, 2);
    if (ENS160_MISR_CRC(Buf, 2, misr)) {
        // 数据错误
        return 1;
    }
    Tmp   = (Buf[0] | (Buf[1] << 8));
    *Temp = (i8)((Tmp >> 6) - 273);
    return 0;
}

u8 ENS160_Get_DATA_RH_MISR(u8 *RH)
{
    u8 misr = ENS160_Get_DATA_MISR();
    u8 Buf[2];
    u16 Tmp = 0;
    I2C_ReadNBytes((ENS160_ADDR), ENS160_DATA_RH, Buf, 2);
    if (ENS160_MISR_CRC(Buf, 2, misr)) {
        // 数据错误
        return 1;
    }
    Tmp = (Buf[0] | (Buf[1] << 8));
    *RH = (u8)(Tmp >> 9);
    return 0;
}

#endif

#endif

// 通用读写寄存器
#if ENS160_GPR_WRITE_ENABLE == 1

/*  读取命令寄存器
 *  u8 Offset: 读取的寄存器偏移量 (范围: 0 ~ 7)
 *  u8 *Buf: 读取数据存放的缓冲区
 *  u8 Buf_Len: 读取数据的长度 (范围: 1 ~ 8)
 *  返回值: 0表示成功，1表示失败。
 */
void ENS160_Get_GPR_WRITE(u8 Offset, u8 *Buf, u8 Buf_Len)
{
    I2C_ReadNBytes((ENS160_ADDR), (ENS160_GPR_WRITE + Offset), Buf, Buf_Len);
}

/*  写入命令寄存器
 *  u8 Offset: 读取的寄存器偏移量 (范围: 0 ~ 7)
 *  u8 *Buf: 读取数据存放的缓冲区
 *  u8 Buf_Len: 读取数据的长度 (范围: 1 ~ 8)
 *  返回值: 0表示成功，1表示失败。
 */
void ENS160_Set_GPR_WRITE(u8 Offset, u8 *Buf, u8 Buf_Len)
{
    ENS160_OPMODE_Set(ENS160_OPMODE_IDLE);
    I2C_WriteNBytes(ENS160_ADDR, (ENS160_GPR_WRITE + Offset), Buf, Buf_Len);
    ENS160_OPMODE_Set(ENS160_OPMODE_STANDBY);
}
#endif

// 通用读写寄存器
#if ENS160_GPR_READ_ENABLE == 1

/*  GPR_READ Register(8 Byte) Address (Default = 0x00)
 *  u8 Offset: 读取的寄存器偏移量 (范围: 0 ~ 7)
 *  u8 *Buf: 读取数据存放的缓冲区
 *  u8 Buf_Len: 读取数据的长度 (范围: 1 ~ 8)
 *  返回值: 0表示成功，1表示失败。

 *  常用的寄存器偏移量:
 *    Offset = 4, Buf_Len = 3;  get_version_info
 *    Offset = 0, Buf_Len = 2;  get_R1raw (16 bit)
 *    Offset = 6, Buf_Len = 2;  get_R4raw (16 bit)
 *  Rires = 2 << (Riraw >> 11)
 */
void ENS160_Get_GPR_READ(u8 Offset, u8 *Buf, u8 Buf_Len)
{
    I2C_ReadNBytes((ENS160_ADDR), (ENS160_GPR_READ + Offset), Buf, Buf_Len);
}

#endif