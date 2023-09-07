#ifndef __TYPE_DEF_H
#define __TYPE_DEF_H

//========================================================================
//                            Type define
//========================================================================
typedef unsigned char u8;  //  8 bits
typedef unsigned int u16;  // 16 bits
typedef unsigned long u32; // 32 bits

typedef signed char i8;  //  8 bits
typedef signed int i16;  // 16 bits
typedef signed long i32; // 32 bits

typedef unsigned char uint8;  //  8 bits
typedef unsigned int uint16;  // 16 bits
typedef unsigned long uint32; // 32 bits

typedef signed char int8;  //  8 bits
typedef signed int int16;  // 16 bits
typedef signed long int32; // 32 bits

#define UINT8_MAX  0xFF       //  8 bits
#define UINT16_MAX 0xFFFF     // 16 bits
#define UINT32_MAX 0xFFFFFFFF // 32 bits

//===================================================
#define TRUE  1
#define FALSE 0

//===================================================
#define NULL 0
#define LOOP while (1)

//===================================================
#define Priority_0 0 // super low
#define Priority_1 1 // low
#define Priority_2 2 // high
#define Priority_3 3 // best high

#define ENABLE     1
#define DISABLE    0

#define SUCCESS    0
#define FAIL       -1

//===================================================
#define I2C_Mode_Master 1
#define I2C_Mode_Slave  0

//===================================================
#define PIE  0x20 // 1: The comparison result changes from 0 to 1, resulting in rising edge interrupt.
#define NIE  0x10 // 1: The comparison result changes from 1 to 0, resulting in a falling edge interrupt.

#define PWMA 128
#define PWMB 129

#endif
