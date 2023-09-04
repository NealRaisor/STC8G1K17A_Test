#ifndef __STC8G_H_I2C_H
#define __STC8G_H_I2C_H

#include "Config.h"

//========================================================================
//                               I2C set
//========================================================================
//============================== Master ====================================
#define I2C_Function(n) (n == 0 ? (I2CCFG &= ~0x80) : (I2CCFG |= 0x80))     /* 1：Enable I2C   0：Disable I2C */
#define I2C_Mode_Set(n) (n == 0 ? (I2CCFG &= ~0x40) : (I2CCFG |= 0x40))     /* 1: Set Master	0: Set Slave */
#define I2C_SetSpeed(n) (I2CCFG = (I2CCFG & ~0x3f) | (n & 0x3f))            // Speed=(Fosc/(2*I2C_Speed)-4)/2
#define I2C_WDTA_EN(n)  (n == 0 ? (I2CMSAUX &= ~0x01) : (I2CMSAUX |= 0x01)) /* 1: Enable Auto Send 0: Disable Auto Send */

#if MAIN_Fosc == 35000000L

#define I2C_Speed_1M   7
#define I2C_Speed_400K 20

#elif MAIN_Fosc == 24000000L

#define I2C_Speed_1M   4
#define I2C_Speed_400K 13
#define I2C_Speed_100K 58

#elif MAIN_Fosc == 20000000L

#define I2C_Speed_1M   3
#define I2C_Speed_400K 10
#define I2C_Speed_100K 48

#elif MAIN_Fosc == 12000000L

#define I2C_Speed_1M   1
#define I2C_Speed_400K 5
#define I2C_Speed_100K 28

#elif MAIN_Fosc == 6000000L

#define I2C_Speed_400K 2
#define I2C_Speed_100K 13

#endif

//============================== Slave ====================================
// #define I2C_ESTAI_EN(n)     (I2CSLCR = (I2CSLCR & ~0x40) | (n << 6))            /* slave enable recv START interrupt */
// #define I2C_ERXI_EN(n)      (I2CSLCR = (I2CSLCR & ~0x20) | (n << 5))            /* slave enable recv 1Byte interrupt*/
// #define I2C_ETXI_EN(n)      (I2CSLCR = (I2CSLCR & ~0x10) | (n << 4))            /* slave enable send 1Byte interrupt */
// #define I2C_ESTOI_EN(n)     (I2CSLCR = (I2CSLCR & ~0x08) | (n << 3))            /* slave enable recv STOP  interrupt */
// #define I2C_MATCH_EN(n)     (n == 0 ? (I2CSLADR |= 0x01) : (I2CSLADR &= ~0x01)) /* slave enable addr compere function, Only same addr */
// #define I2C_SLRET()         (I2CSLCR |= 0x01)                                   /* Reset slave mode */
// #define I2C_Address(n)      (I2CSLADR = (I2CSLADR & 0x01) | (n << 1))           /* slave addr */

// #define I2C_SLAVE_MODE_ADDR 0xA0 /* slave write addr */
// #define I2C_BUF_LENTH       8    /* I2C buffer length */
// #define I2C_ESTAI           0x40 /* slave recv START interrupt */
// #define I2C_ERXI            0x20 /* slave recv 1Byte interrupt */
// #define I2C_ETXI            0x10 /* slave send 1Byte interrupt */
// #define I2C_ESTOI           0x08 /* slave recv STOP interrupt */

// typedef struct {
//     u8 isma; // MEMORY ADDRESS
//     u8 isda; // DEVICE ADDRESS
//     u8 addr; // ADDRESS
// } I2C_IsrTypeDef;

// extern u8 xdata I2C_Buffer[I2C_BUF_LENTH];
// extern bit DisplayFlag;

//========================================================================
//                              define declare
//========================================================================
#define Get_MSBusy_Status() ((0 == (I2CMSST & 0x80)) ? 0 : 1) // get master busy status
#define I2C_NO_MEM_ADDR     0xFF                              // No memory addr

//========================================================================
//                             function declare
//========================================================================
// Base operate function
void I2C_Wait();
void I2C_Start();
void I2C_RecvACK();
void I2C_SendACK();
void I2C_SendNAK();
void I2C_SendByte(u8 Data);
u8 I2C_RecvByte(void);
void I2C_Stop();
// Package function
void I2C_WriteNBytes(u8 dev_addr, u8 Mem_Addr, u8 *buf, u8 len);
void I2C_ReadNBytes(u8 dev_addr, u8 Mem_Addr, u8 *buf, u8 len);

#endif
