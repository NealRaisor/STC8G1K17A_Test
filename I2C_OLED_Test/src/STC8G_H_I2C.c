#include "STC8G_H_I2C.h"

//==================== Standard i2 operation function ====================
void I2C_Wait()
{
    while (!(I2CMSST & 0x40))
        ;
    I2CMSST &= ~0x40;
}

void I2C_Start()
{
    I2CMSCR = 0x01;
    I2C_Wait();
}

void I2C_SendByte(u8 Data)
{
    I2CTXD  = Data;
    I2CMSCR = 0x02;
    I2C_Wait();
}

void I2C_RecvACK()
{
    I2CMSCR = 0x03;
    I2C_Wait();
}

u8 I2C_RecvByte(void)
{
    I2CMSCR = 0x04;
    I2C_Wait();
    return I2CRXD;
}

void I2C_SendACK()
{
    I2CMSST = 0x00;
    I2CMSCR = 0x05;
    I2C_Wait();
}

void I2C_SendNAK()
{
    I2CMSST = 0x01;
    I2CMSCR = 0x05;
    I2C_Wait();
}

void I2C_Stop()
{
    I2CMSCR = 0x06;
    I2C_Wait();
}
//================================= end ===================================

/// @brief Standard i2c write data function
/// @param Dev_Addr device i2c adrr
/// @param Mem_Addr target memory addr
/// @param buf  Data buffer
/// @param len  Length of data to be sent
void I2C_WriteNBytes(u8 Dev_Addr, u8 Mem_Addr, u8 *buf, u8 len)
{
    // I2C standard writing data
    I2C_Start();            // start sign
    I2C_SendByte(Dev_Addr); // send i2c write addr
    I2C_RecvACK();
    if (I2C_NO_MEM_ADDR != Mem_Addr) {
        I2C_SendByte(Mem_Addr); // send memory addr
        I2C_RecvACK();
    }
    // I2C Send data with length len.
    do {
        I2C_SendByte(*buf);
        I2C_RecvACK();
        if (1 != len) // Avoid array out of bounds
            ++buf;
    } while (--len);
    I2C_Stop(); // stop i2c
}

/// @brief Standard i2c read data function
/// @param Dev_Addr  device i2c adrr
/// @param Mem_Addr target memory addr
/// @param buf  data buffer
/// @param len  Length of data to be sent
void I2C_ReadNBytes(u8 Dev_Addr, u8 Mem_Addr, u8 *buf, u8 len)
{
    if (I2C_NO_MEM_ADDR != Mem_Addr) {
        // I2C write mem_addr
        I2C_Start();
        I2C_SendByte(Dev_Addr); // write cmd
        I2C_RecvACK();
        I2C_SendByte(Mem_Addr); // target mem_addr
        I2C_RecvACK();
    }
    // I2C read start
    I2C_Start();
    I2C_SendByte(Dev_Addr | 1); // send read sign
    I2C_RecvACK();
    do {
        (*buf) = I2C_RecvByte();
        if (1 != len) {
            ++buf;
            I2C_SendACK(); // send ACK
        }
    } while (--len);
    I2C_SendNAK(); // last one, Send no ACK, Recv over.
    I2C_Stop();    // stop i2c
}

/*
void I2C_Init(I2C_InitTypeDef *I2Cx)
{
    u8 i;
    for (i = 0; i < I2C_BUF_LENTH; i++)
        I2C_BUF[i] = 0x00;
// master
    if (I2Cx->I2C_Mode == I2C_Mode_Master) {
        I2C_Master();
        I2CMSST = 0x00;
        I2C_SetSpeed(I2Cx->I2C_Speed);
        if (I2Cx->I2C_MS_WDTA == ENABLE)
            I2C_WDTA_EN();
        else
            I2C_WDTA_DIS();
    }
// slave
    else {
        I2C_Slave();
        I2CSLST = 0x00;
        I2C_Address(I2Cx->I2C_SL_ADR);
        if (I2Cx->I2C_SL_MA == ENABLE)
            I2C_MATCH_EN();
        else
            I2C_MATCH_DIS();
    }
    I2C_Function(I2Cx->I2C_Enable);
}
*/