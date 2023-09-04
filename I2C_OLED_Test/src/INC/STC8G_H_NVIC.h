#ifndef __STC8G_H_NVIC_H
#define __STC8G_H_NVIC_H

#include "config.h"

//========================================================================
//                           edge type define
//========================================================================
#define FALLING_EDGE 1 // falling edge sign
#define RISING_EDGE  2 // rising edge sign

//========================================================================
//                           TimerX interrupt config
//========================================================================
#define Timer0_Interrupt(n) (n == 0 ? (ET0 = 0) : (ET0 = 1))
#define Timer1_Interrupt(n) (n == 0 ? (ET1 = 0) : (ET1 = 1))
#define Timer2_Interrupt(n) IE2 = (IE2 & ~0x04) | (n << 2)
#define Timer3_Interrupt(n) IE2 = (IE2 & ~0x20) | (n << 5)
#define Timer4_Interrupt(n) IE2 = (IE2 & ~0x40) | (n << 6)

//========================================================================
//                             external interrupt config
//========================================================================
#define INT0_Interrupt(n) (n == 0 ? (EX0 = 0) : (EX0 = 1))
#define INT1_Interrupt(n) (n == 0 ? (EX1 = 0) : (EX1 = 1))
#define INT2_Interrupt(n) INTCLKO = (INTCLKO & ~0x10) | (n << 4)
#define INT3_Interrupt(n) INTCLKO = (INTCLKO & ~0x20) | (n << 5)
#define INT4_Interrupt(n) INTCLKO = (INTCLKO & ~0x40) | (n << 6)

//========================================================================
//                              ADC interrupt config
//========================================================================
#define ADC_Interrupt(n) (n == 0 ? (EADC = 0) : (EADC = 1))

//========================================================================
//                              SPI interrupt config
//========================================================================
#define SPI_Interrupt(n) IE2 = (IE2 & ~0x02) | (n << 1)

//========================================================================
//                              I2C interrupt config
//========================================================================
#define I2C_Master_Inturrupt(n) (n == 0 ? (I2CMSCR &= ~0x80) : (I2CMSCR |= 0x80))
#define I2C_Slave_Inturrupt(n)  (I2CSLCR = (I2CSLCR & ~0x78) | n)

//========================================================================
//                              UART  interrupt config
//========================================================================
#define UART1_Interrupt(n) (n == 0 ? (ES = 0) : (ES = 1))
#define UART2_Interrupt(n) IE2 = (IE2 & ~0x01) | (n)
#define UART3_Interrupt(n) IE2 = (IE2 & ~0x08) | (n << 3)
#define UART4_Interrupt(n) IE2 = (IE2 & ~0x10) | (n << 4)

//========================================================================
//                              RTC interrupt config
//========================================================================
#define RTC_Interrupt(n) RTCIEN = (n)

//========================================================================
//                             interrupt priority
//========================================================================
//                      7      6      5      4      3      2      1      0      Reset Value
// sfr IP2   = 0xB5; PPWM2FD PI2C   PCMP    PX4  PPWM0FD  PPWM0  PSPI   PS2     0000,0000
#define PUSB    0x80
#define PPWM2FD 0x80
#define PTKSU   0x80
#define PI2C    0x40
#define PCMP    0x20
#define PX4     0x10
#define PPWM0FD 0x08
#define PPWMB   0x08
#define PPWM0   0x04
#define PPWMA   0x04
#define PSPI    0x02
#define PS2     0x01
//                       7      6      5      4      3      2      1      0     Reset Value
// sfr IP2H  = 0xB6; PPWM2FDH PI2CH PCMPH   PX4H PPWM0FDH PPWM0H PSPIH  PS2H    0000,0000
#define PUSBH    0x80
#define PPWM2FDH 0x80
#define PTKSUH   0x80
#define PI2CH    0x40
#define PCMPH    0x20
#define PX4H     0x10
#define PPWM0FDH 0x08
#define PPWMBH   0x08
#define PPWM0H   0x04
#define PPWMAH   0x04
#define PSPIH    0x02
#define PS2H     0x01

// UART2 Priority config
#define UART2_Priority(n)                       \
    do {                                        \
        if (n == 0) IP2H &= ~PS2H, IP2 &= ~PS2; \
        if (n == 1) IP2H &= ~PS2H, IP2 |= PS2;  \
        if (n == 2) IP2H |= PS2H, IP2 &= ~PS2;  \
        if (n == 3) IP2H |= PS2H, IP2 |= PS2;   \
    } while (0)

// SPI Priority config
#define SPI_Priority(n)                           \
    do {                                          \
        if (n == 0) IP2H &= ~PSPIH, IP2 &= ~PSPI; \
        if (n == 1) IP2H &= ~PSPIH, IP2 |= PSPI;  \
        if (n == 2) IP2H |= PSPIH, IP2 &= ~PSPI;  \
        if (n == 3) IP2H |= PSPIH, IP2 |= PSPI;   \
    } while (0)
// INT4 Priority config
#define INT4_Priority(n)                        \
    do {                                        \
        if (n == 0) IP2H &= ~PX4H, IP2 &= ~PX4; \
        if (n == 1) IP2H &= ~PX4H, IP2 |= PX4;  \
        if (n == 2) IP2H |= PX4H, IP2 &= ~PX4;  \
        if (n == 3) IP2H |= PX4H, IP2 |= PX4;   \
    } while (0)

// CMP Priority config
#define CMP_Priority(n)                           \
    do {                                          \
        if (n == 0) IP2H &= ~PCMPH, IP2 &= ~PCMP; \
        if (n == 1) IP2H &= ~PCMPH, IP2 |= PCMP;  \
        if (n == 2) IP2H |= PCMPH, IP2 &= ~PCMP;  \
        if (n == 3) IP2H |= PCMPH, IP2 |= PCMP;   \
    } while (0)

// I2C Priority config
#define I2C_Priority(n)                           \
    do {                                          \
        if (n == 0) IP2H &= ~PI2CH, IP2 &= ~PI2C; \
        if (n == 1) IP2H &= ~PI2CH, IP2 |= PI2C;  \
        if (n == 2) IP2H |= PI2CH, IP2 &= ~PI2C;  \
        if (n == 3) IP2H |= PI2CH, IP2 |= PI2C;   \
    } while (0)

// PWM0 Priority config
#define PWM0_Priority(n)                            \
    do {                                            \
        if (n == 0) IP2H &= ~PPWM0H, IP2 &= ~PPWM0; \
        if (n == 1) IP2H &= ~PPWM0H, IP2 |= PPWM0;  \
        if (n == 2) IP2H |= PPWM0H, IP2 &= ~PPWM0;  \
        if (n == 3) IP2H |= PPWM0H, IP2 |= PPWM0;   \
    } while (0)

// PWM0FD Priority config
#define PWM0FD_Priority(n)                              \
    do {                                                \
        if (n == 0) IP2H &= ~PPWM0FDH, IP2 &= ~PPWM0FD; \
        if (n == 1) IP2H &= ~PPWM0FDH, IP2 |= PPWM0FD;  \
        if (n == 2) IP2H |= PPWM0FDH, IP2 &= ~PPWM0FD;  \
        if (n == 3) IP2H |= PPWM0FDH, IP2 |= PPWM0FD;   \
    } while (0)

// PWM2FD Priority config
#define PWM2FD_Priority(n)                              \
    do {                                                \
        if (n == 0) IP2H &= ~PPWM2FDH, IP2 &= ~PPWM2FD; \
        if (n == 1) IP2H &= ~PPWM2FDH, IP2 |= PPWM2FD;  \
        if (n == 2) IP2H |= PPWM2FDH, IP2 &= ~PPWM2FD;  \
        if (n == 3) IP2H |= PPWM2FDH, IP2 |= PPWM2FD;   \
    } while (0)

// PTKSU Priority config
#define PTKSU_Priority(n)                           \
    do {                                            \
        if (n == 0) IP2H &= ~PTKSUH, IP2 &= ~PTKSU; \
        if (n == 1) IP2H &= ~PTKSUH, IP2 |= PTKSU;  \
        if (n == 2) IP2H |= PTKSUH, IP2 &= ~PTKSU;  \
        if (n == 3) IP2H |= PTKSUH, IP2 |= PTKSU;   \
    } while (0)

// USB Priority config
#define USB_Priority(n)                           \
    do {                                          \
        if (n == 0) IP2H &= ~PUSBH, IP2 &= ~PUSB; \
        if (n == 1) IP2H &= ~PUSBH, IP2 |= PUSB;  \
        if (n == 2) IP2H |= PUSBH, IP2 &= ~PUSB;  \
        if (n == 3) IP2H |= PUSBH, IP2 |= PUSB;   \
    } while (0)

// PWMA Priority config
#define PWMA_Priority(n)                            \
    do {                                            \
        if (n == 0) IP2H &= ~PPWMAH, IP2 &= ~PPWMA; \
        if (n == 1) IP2H &= ~PPWMAH, IP2 |= PPWMA;  \
        if (n == 2) IP2H |= PPWMAH, IP2 &= ~PPWMA;  \
        if (n == 3) IP2H |= PPWMAH, IP2 |= PPWMA;   \
    } while (0)

// PWMB Priority config
#define PWMB_Priority(n)                            \
    do {                                            \
        if (n == 0) IP2H &= ~PPWMBH, IP2 &= ~PPWMB; \
        if (n == 1) IP2H &= ~PPWMBH, IP2 |= PPWMB;  \
        if (n == 2) IP2H |= PPWMBH, IP2 &= ~PPWMB;  \
        if (n == 3) IP2H |= PPWMBH, IP2 |= PPWMB;   \
    } while (0)

//                       7      6      5     4      3      2      1     0      Reset Value
// sfr IP3   = 0xDF;  PPWM4FD PPWM5  PPWM4  PPWM3  PPWM2  PPWM1  PS4   PS3     0000,0000
#define PPWM4FD 0x80
#define PPWM5   0x40
#define PPWM4   0x20
#define PPWM3   0x10
#define PPWM2   0x08
#define PPWM1   0x04
#define PRTC    0x04
#define PS4     0x02
#define PS3     0x01
//                     7        6       5       4       3       2     1     0      Reset Value
// sfr IP3H  = 0xEE; PPWM4FDH PPWM5H  PPWM4H  PPWM3H  PPWM2H  PPWM1H PS4H  PS3H    0000,0000
#define PPWM4FDH 0x80
#define PPWM5H   0x40
#define PPWM4H   0x20
#define PPWM3H   0x10
#define PPWM2H   0x08
#define PPWM1H   0x04
#define PRTCH    0x04
#define PS4H     0x02
#define PS3H     0x01

// RTC Priority config
#define RTC_Priority(n)                           \
    do {                                          \
        if (n == 0) IP3H &= ~PRTCH, IP3 &= ~PRTC; \
        if (n == 1) IP3H &= ~PRTCH, IP3 |= PRTC;  \
        if (n == 2) IP3H |= PRTCH, IP3 &= ~PRTC;  \
        if (n == 3) IP3H |= PRTCH, IP3 |= PRTC;   \
    } while (0)

// PWM1 Priority config
#define PWM1_Priority(n)                            \
    do {                                            \
        if (n == 0) IP3H &= ~PPWM1H, IP3 &= ~PPWM1; \
        if (n == 1) IP3H &= ~PPWM1H, IP3 |= PPWM1;  \
        if (n == 2) IP3H |= PPWM1H, IP3 &= ~PPWM1;  \
        if (n == 3) IP3H |= PPWM1H, IP3 |= PPWM1;   \
    } while (0)

// PWM2 Priority config
#define PWM2_Priority(n)                            \
    do {                                            \
        if (n == 0) IP3H &= ~PPWM2H, IP3 &= ~PPWM2; \
        if (n == 1) IP3H &= ~PPWM2H, IP3 |= PPWM2;  \
        if (n == 2) IP3H |= PPWM2H, IP3 &= ~PPWM2;  \
        if (n == 3) IP3H |= PPWM2H, IP3 |= PPWM2;   \
    } while (0)

// PWM3 Priority config
#define PWM3_Priority(n)                            \
    do {                                            \
        if (n == 0) IP3H &= ~PPWM3H, IP3 &= ~PPWM3; \
        if (n == 1) IP3H &= ~PPWM3H, IP3 |= PPWM3;  \
        if (n == 2) IP3H |= PPWM3H, IP3 &= ~PPWM3;  \
        if (n == 3) IP3H |= PPWM3H, IP3 |= PPWM3;   \
    } while (0)

// PWM4  Priority config
#define PWM4_Priority(n)                            \
    do {                                            \
        if (n == 0) IP3H &= ~PPWM4H, IP3 &= ~PPWM4; \
        if (n == 1) IP3H &= ~PPWM4H, IP3 |= PPWM4;  \
        if (n == 2) IP3H |= PPWM4H, IP3 &= ~PPWM4;  \
        if (n == 3) IP3H |= PPWM4H, IP3 |= PPWM4;   \
    } while (0)

// PWM5 Priority config
#define PWM5_Priority(n)                            \
    do {                                            \
        if (n == 0) IP3H &= ~PPWM5H, IP3 &= ~PPWM5; \
        if (n == 1) IP3H &= ~PPWM5H, IP3 |= PPWM5;  \
        if (n == 2) IP3H |= PPWM5H, IP3 &= ~PPWM5;  \
        if (n == 3) IP3H |= PPWM5H, IP3 |= PPWM5;   \
    } while (0)

// PWM4 Priority config
#define PWM4FD_Priority(n)                              \
    do {                                                \
        if (n == 0) IP3H &= ~PPWM4FDH, IP3 &= ~PPWM4FD; \
        if (n == 1) IP3H &= ~PPWM4FDH, IP3 |= PPWM4FD;  \
        if (n == 2) IP3H |= PPWM4FDH, IP3 &= ~PPWM4FD;  \
        if (n == 3) IP3H |= PPWM4FDH, IP3 |= PPWM4FD;   \
    } while (0)

// UART3  Priority config
#define UART3_Priority(n)                       \
    do {                                        \
        if (n == 0) IP3H &= ~PS3H, IP3 &= ~PS3; \
        if (n == 1) IP3H &= ~PS3H, IP3 |= PS3;  \
        if (n == 2) IP3H |= PS3H, IP3 &= ~PS3;  \
        if (n == 3) IP3H |= PS3H, IP3 |= PS3;   \
    } while (0)

// UART4 Priority config
#define UART4_Priority(n)                       \
    do {                                        \
        if (n == 0) IP3H &= ~PS4H, IP3 &= ~PS4; \
        if (n == 1) IP3H &= ~PS4H, IP3 |= PS4;  \
        if (n == 2) IP3H |= PS4H, IP3 &= ~PS4;  \
        if (n == 3) IP3H |= PS4H, IP3 |= PS4;   \
    } while (0)

//                     7      6      5      4      3      2      1      0      Reset Value
// sfr IPH   = 0xB7; PPCAH  PLVDH   PADCH   PSH   PT1H    PX1H   PT0H   PX0H    0000,0000
// #define		PPCAH	0x80
#define PLVDH 0x40
#define PADCH 0x20
#define PSH   0x10
#define PT1H  0x08
#define PX1H  0x04
#define PT0H  0x02
#define PX0H  0x01

// INT0 Priority config
#define INT0_Priority(n)                   \
    do {                                   \
        if (n == 0) IPH &= ~PX0H, PX0 = 0; \
        if (n == 1) IPH &= ~PX0H, PX0 = 1; \
        if (n == 2) IPH |= PX0H, PX0 = 0;  \
        if (n == 3) IPH |= PX0H, PX0 = 1;  \
    } while (0)

// INT1 Priority config
#define INT1_Priority(n)                   \
    do {                                   \
        if (n == 0) IPH &= ~PX1H, PX1 = 0; \
        if (n == 1) IPH &= ~PX1H, PX1 = 1; \
        if (n == 2) IPH |= PX1H, PX1 = 0;  \
        if (n == 3) IPH |= PX1H, PX1 = 1;  \
    } while (0)

// Timer0 Priority config
#define Timer0_Priority(n)                 \
    do {                                   \
        if (n == 0) IPH &= ~PT0H, PT0 = 0; \
        if (n == 1) IPH &= ~PT0H, PT0 = 1; \
        if (n == 2) IPH |= PT0H, PT0 = 0;  \
        if (n == 3) IPH |= PT0H, PT0 = 1;  \
    } while (0)

// Timer1 Priority config
#define Timer1_Priority(n)                 \
    do {                                   \
        if (n == 0) IPH &= ~PT1H, PT1 = 0; \
        if (n == 1) IPH &= ~PT1H, PT1 = 1; \
        if (n == 2) IPH |= PT1H, PT1 = 0;  \
        if (n == 3) IPH |= PT1H, PT1 = 1;  \
    } while (0)

// UART1 Priority config
#define UART1_Priority(n)                \
    do {                                 \
        if (n == 0) IPH &= ~PSH, PS = 0; \
        if (n == 1) IPH &= ~PSH, PS = 1; \
        if (n == 2) IPH |= PSH, PS = 0;  \
        if (n == 3) IPH |= PSH, PS = 1;  \
    } while (0)

// ADC Priority config
#define ADC_Priority(n)                      \
    do {                                     \
        if (n == 0) IPH &= ~PADCH, PADC = 0; \
        if (n == 1) IPH &= ~PADCH, PADC = 1; \
        if (n == 2) IPH |= PADCH, PADC = 0;  \
        if (n == 3) IPH |= PADCH, PADC = 1;  \
    } while (0)

// LVD Priority config
#define LVD_Priority(n)                      \
    do {                                     \
        if (n == 0) IPH &= ~PLVDH, PADC = 0; \
        if (n == 1) IPH &= ~PLVDH, PADC = 1; \
        if (n == 2) IPH |= PLVDH, PADC = 0;  \
        if (n == 3) IPH |= PLVDH, PADC = 1;  \
    } while (0)

#endif
