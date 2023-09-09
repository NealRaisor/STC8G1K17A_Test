#ifndef __ENS160_I2C_H
#define __ENS160_I2C_H

#include "STC8G_H_I2C.h"
#include "STC8G_H_Delay.h"

//========================================================================
// i2c Write Addresses
//========================================================================
/*
ENS160是一个I2C从设备，
如果上电时MISO/ADDR线保持低电平，则具有固定的7位地址0x52;
如果MISO/ADDR线保持高电平，则具有固定的7位地址0x53。
*/
#define ENS160_ADDR 0xA6 // IIC写地址
//========================================================================
// 功能开关宏定义
#define ENS160_COMMAND_ENABLE   DISABLE // 使能命令相关函数

#define ENS160_DATA_T_RH_ENABLE DISABLE // 使能温湿度相关函数
#define ENS160_DATA_MISR_ENABLE DISABLE // 使能MISR相关函数

#define ENS160_GPR_WRITE_ENABLE DISABLE // 使能GPR写相关函数
#define ENS160_GPR_READ_ENABLE  DISABLE // 使能GPR读相关函数

//========================================================================
// ENS160 Register Addresses
//========================================================================
#define ENS160_PART_ID 0x00
/* Part_ID寄存器
 * Part_ID Register Address (Default = 0x0160)
 * Read Only
 * H_byte = 0x01, L_byte = 0x60
 */

//========================================================================
#define ENS160_OPMODE 0x10
/* 主要操作寄存器
 * Run_Mode Register Address (Default = 0x00)
 * Read/Write
 * 0x00 = Deep Sleep Mode
 * 0x01 = Idle Mode
 * 0x02 = Standby Mode
 * 0xf0 = Reset
 */

#define ENS160_OPMODE_DEEP_SLEEP 0x00
#define ENS160_OPMODE_IDLE       0x01
#define ENS160_OPMODE_STANDBY    0x02
#define ENS160_OPMODE_RESET      0xF0

void ENS160_OPMODE_Set(u8 Mode);
void ENS160_Init(); // 初始化函数

//========================================================================
#define ENS160_CONFIG 0x11
/**
 * @brief Interrupt Config Register Address (Default = 0x00)
 * Read/Write
 * Undef_Bit = 7|4|2;
 * Bit 0    0 = Interrupt Pin Disabled, 1 = Interrupt Pin Enabled
 * Bit 1    DATA_XXX New DATA Ready Interrupt Enable
 * Bit 3    Generic Read_Only Register Update Interrupt Enable
 * Bit 5    Interrupt Pin Drive Mode;   0 = Open Drain, 1 = Push Pull
 * Bit 6    Interrupt Pin Active Level; 0 = Low,        1 = High
 */
void ENS160_CONFIG_INTERRUPT_Set(u8 Config); // 用函数传参，限定参数类型

// //========================================================================
#define ENS160_DEVICE_STATUS 0x20
/* 主要操作寄存器
 * Device_Status Register Address (Default = 0x00)
 * Read Only
 * Undef_Bit = 4|5;

 * Bit 0    GPR_READX 有新数据 0 = No New DATA, 1 = New DATA Ready
 （新数据首次读取后自动清零） (DEVICE_STATUS & 0x01)
 * Bit 1    DATA_X    有新数据 0 = No New DATA, 1 = New DATA Ready
 （新数据首次读取后自动清零） (DEVICE_STATUS & 0x02)>>1
 * Bit 2:3  有效标志位 0=正常运行，1=传感器正在热身，2=初始启动阶段，3=输出无效
  (DEVICE_STATUS & 0x0C)>>2
 * Bit 6    错误标志位 0 = No Error, 1 = Error （错误信息在GPR_READ中）
  (DEVICE_STATUS & 0x40)>>6

 * 不可读写
 * Bit 7   Statas   1 = Run
 */

#define ENS160_DEVICE_STATUS_NEW_GPR_READX(x) (x & 0x01)
#define ENS160_DEVICE_STATUS_NEW_DATA_X(x)    ((x & 0x02) >> 1)
#define ENS160_DEVICE_STATUS_VALID(x)         ((x & 0x0C) >> 2)
#define ENS160_DEVICE_STATUS_ERROR(x)         ((x & 0x40) >> 6)

#define ENS160_DEVICE_STATUS_VALID_NORMAL     0 // 正常运行
#define ENS160_DEVICE_STATUS_VALID_WARMUP     1 // 传感器正在热身
#define ENS160_DEVICE_STATUS_VALID_INITIAL    2 // 初始启动阶段
#define ENS160_DEVICE_STATUS_VALID_INVALID    3 // 输出无效

void ENS160_Get_DEVICE_STATUS(u8 *Device_Status); // 正常工作时无反应

//========================================================================
#define ENS160_DATA_AQI 0x21
/** 主要操作寄存器
 * @brief DATA_AQI Register Address (Default = 0x01)
 * Read Only
 * Undef_Bit 3:7;
 * Bit 0:2  Air Quality Index (AQI[1-5])
        (DATA_AQI & 0x07)
      1 = 优秀, 2 = 良好, 3 = 中度, 4 = 较差, 5 = 非常差
 */

#define ENS160_AQI_VERY_GOOD 1
#define ENS160_AQI_GOOD      2
#define ENS160_AQI_MODERATE  3
#define ENS160_AQI_POOR      4
#define ENS160_AQI_VERY_POOR 5

void ENS160_Get_DATA_AQI(u8 *DATA_AQI);

//========================================================================
#define ENS160_DATA_TVOC 0x22
/** 主要操作寄存器
 * @brief DATA_TVOC Register Address (Default = 0x0000)
 * Read Only

 * u8 Buf[2] = [TVOC_L, TVOC_H]
 * u16 TVOC = Buf[0] | (Buf[1]<<8)

 * 以ppb为单位报告计算出的TVOC浓度值。
 */

void ENS160_Get_DATA_TVOC(u16 *TVOC);

//========================================================================
#define ENS160_DATA_ECO2 0x24
/** 主要操作寄存器
 * @brief DATA_ECO2 Register Address (Default = 0x0000)
 * Read Only

 * u8 Buf[2] = [ECO2_L, ECO2_H]
 * u16 ECO2 = Buf[0] | (Buf[1]<<8)

 * 以ppm为单位报告计算出的eCO2浓度值。
 */

void ENS160_Get_DATA_ECO2(u16 *ECO2);

//========================================================================

#if ENS160_COMMAND_ENABLE == 1

#define ENS160_COMMAND 0x12
/**
 * @brief Command Register Address (Default = 0x00)
 * Read/Write
 * 0x00 = Command_Nop (No Operation)
 * 0x0E = Command_Get_AppVersion (Get Application Version)
    Return Info Addr (GPR_READ4 = Major, GPR_READ5 = Minor, GPR_READ6 = Patch)
 * 0xCC = Command_ClrGPR (Clear GPR)
 */

#define ENS160_COMMAND_NOP             0x00
#define ENS160_COMMAND_GET_APP_VERSION 0x0E
#define ENS160_COMMAND_CLR_GPR         0xCC
u8 ENS160_COMMAND_Set(u8 Command); // 用函数传参，限定参数类型

#endif

//========================================================================

#if ENS160_DATA_T_RH_ENABLE == 1

#define ENS160_TEMP_IN 0x13
/**
 * @brief Temp_IN Register Address (Default = 0x0000)
 * Read/Write

 * u16 Tmp = （摄氏度 + 273）<< 6;
 * u8  Temp_IN[2] = [Tmp & 0xFF, (Tmp & 0xFF00)>>8];

 * 温度补偿值寄存器，用于存储温度补偿值。
 */

void ENS160_TEMP_IN_Set(u16 *Temp); // 用函数传参，限定参数类型
//========================================================================

#define ENS160_RH_IN 0x15
/**
 * @brief RH_IN Register Address (Default = 0x0000)
 * Read/Write

 * u16 Tmp = 相对湿度 << 9;
 * u8  RH_IN[2] = [Tmp & 0xFF, (Tmp & 0xFF00)>>8];

 * 湿度补偿值寄存器，用于存储湿度补偿值。
 */

void ENS160_RH_IN_Set(u16 *RH); // 用函数传参，限定参数类型
//========================================================================

#define ENS160_DATA_T 0x30
/** 次要操作寄存器
 * @brief DATA_Temp Register Address (Default = 0x0000)
 * Read Only

 * u8 Buf[2] = [Temp_L, Temp_H]
 * u16 Temp = Buf[0] | (Buf[1]<<8)

 * 摄氏度 = Temp >> 6 - 273
 */

void ENS160_Get_DATA_T(i8 *T);

//========================================================================

#define ENS160_DATA_RH 0x32
/** 次要操作寄存器
 * @brief DATA_RH Register Address (Default = 0x0000)
 * Read Only

 * u8 Buf[2] = [RH_L, RH_H]
 * u16 RH = Buf[0] | (Buf[1]<<8)

 * 相对湿度 = RH >> 9
 */

void ENS160_Get_DATA_RH(u8 *RH);

#endif

//========================================================================

#if ENS160_DATA_MISR_ENABLE == 1

#define ENS160_DATA_MISR 0x38
/** 次要操作寄存器
 * @brief DATA_MISR Register Address (Default = 0x0000)
 * Read Only
 * 用于检测寄存器：[0x20, 0x37]内数据读取CRC校验值。
 * 每次读取这个范围内的寄存器值完，都会自动更新。
 *
 * 读取时该范围数据时，先读取MISR寄存器，同步misr值；
 * 每次传送数据时，都会更新MISR寄存器；
      先读取MISR寄存器，同步misr值；
      再读取数据寄存器，并计算misr值；
      最后再次读取MISR寄存器，比较misr值，若相同，则数据传输正确。
 */

#define ENS160_DATA_MISR_POLY 0x1D
u8 ENS160_Get_DATA_MISR(void); // 读取MISR寄存器
u8 ENS160_MISR_CRC(u8 *Buf, u8 Buf_Len, u8 Begin_MISR);

u8 ENS160_Get_DEVICE_STATUS_MISR(u8 *Dev_Status);
u8 ENS160_Get_DATA_AQI_MISR(u8 *AQI);
u8 ENS160_Get_DATA_TVOC_MISR(u16 *TVOC);
u8 ENS160_Get_DATA_ECO2_MISR(u16 *ECO2);

#if ENS160_DATA_T_RH_ENABLE == 1

u8 ENS160_Get_DATA_T_MISR(i8 *Temp);
u8 ENS160_Get_DATA_RH_MISR(u8 *RH);

#endif

#endif
//========================================================================

#if ENS160_GPR_WRITE_ENABLE == 1

#define ENS160_GPR_WRITE 0x40
/**
 * @brief GPR_WRITE Register(8 Byte) Address (Default = 0x00)
 * Read/(Write && (OP_MODE == 0x01))
 * 用于存储用户自定义数据，可用于存储用户自定义的温度补偿值、湿度补偿值、
 */
u8 ENS160_Get_GPR_WRITE(u8 Offset, u8 *Buf, u8 Buf_Len);
u8 ENS160_Set_GPR_WRITE(u8 Offset, u8 *Buf, u8 Buf_Len);

#endif
//========================================================================

#if ENS160_GPR_READ_ENABLE == 1

#define ENS160_GPR_READ 0x48
/**
 * @brief GPR_READ Register(8 Byte) Address (Default = 0x00)
 * Read Only
 * 用于ENS160向主机传递数据，包括错误信息、版本号等。
 */
void ENS160_Get_GPR_READ(u8 Offset, u8 *Buf, u8 Buf_Len);

#endif
//========================================================================

#endif