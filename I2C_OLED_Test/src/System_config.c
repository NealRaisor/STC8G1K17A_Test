#include "System_config.h"

#define Uart1_BaudRate 115200ul

void System_config(void)
{
    // p33 SDA; p32 SCL; p31 TxD; p30 RxD
    u32 j;
    u8 GPIO_Pin_X;
    delay_ms(10); // wait system io port stable

    EAXSFR(); // enable extend register
    //====================================================================================
    // UART Port  Mode -- IO_WeakPP + H_Speed
    GPIO_Pin_X = (GPIO_Pin_0 | GPIO_Pin_1); // set P3.0, P3.1, P3.2, P3.3 mode
    P3_MODE_IO_PU(GPIO_Pin_X);              // P3.0, P3.1, P3.2, P3.3 IO low_pullup
    P3_SPEED_HIGH(GPIO_Pin_X);              // P3.0, P3.1, P3.2, P3.3 high_speed
    // I2C Port Mode -- PP_Out + H_Speed
    GPIO_Pin_X = (GPIO_Pin_2 | GPIO_Pin_3); // set P3.2, P3.3 mode
    P3_MODE_OUT_PP(GPIO_Pin_X);             // Out_pullup
    P3_SPEED_HIGH(GPIO_Pin_X);              // P3.2, P3.3 high_speed
    // Button Port Mode -- HIZ + PP
    GPIO_Pin_X = GPIO_Pin_5;       // set P5.4, P5.5 mode
    P5_MODE_IN_HIZ(GPIO_Pin_X);    // P5.4, P5.5 hiz mode
    P5_PULL_UP_ENABLE(GPIO_Pin_X); // P5.4, P5.5 pullup

    //====================================================================================
    I2C_Mode_Set(I2C_Mode_Master); // set master mode
    I2C_SetSpeed(I2C_Speed_400K);  // set I2C speed
    I2CMSST = 0x00;                // reset i2c contral register
    I2C_WDTA_EN(DISABLE);          // I2c auto send data and Recv ACK
    // interrupt config
    I2C_Master_Inturrupt(DISABLE); // disable I2C interrupt
    I2C_Priority(Priority_1);      // set I2C interrupt priority
    // ready start
    I2C_SW(I2C_P32_P33);  // STC8G1KXXA Port select: I2C_P32_P33: 0 ; I2C_P54_P55: 1
    I2C_Function(ENABLE); // enable I2C

    //====================================================================================
    Timer0_Stop(); // stop Timer0
    // config
    Timer0_Mode_Set(TIM_16BitAutoReload);    // Work mode
    Timer0_CLK_Select(TIM_CLOCK_1T);         // External counting or frequency division
    Timer0_CLK_Output(DISABLE);              // Output pin clk enable
    T0_Load(65536UL - (MAIN_Fosc / 1000UL)); // 1ms load Timer0 init value
    // interrupt config
    Timer0_Priority(Priority_3); // l to h: Priority_0,Priority_1,Priority_2,Priority_3
    Timer0_Interrupt(ENABLE);    // Set interruptENABLE/DISABLE;
    // ready start
    Timer0_Run(); // Run Timer

    //====================================================================================
    UART_Mode_Set(UART_8bit_BRTx);        // COM port mode config
    j = (MAIN_Fosc / 4) / Uart1_BaudRate; // Compute variable baudrate
    if (j >= 65536UL) return;
    j = 65536UL - j;
    // Configure BRT
    Timer1_Stop(); // Stop Timer1
    // config baudrate Timer
    Timer1_Mode_Set(TIM_16BitAutoReload); // Work mode Timer1_16bitAutoReload;
    Timer1_CLK_Select(TIM_CLOCK_1T);      // External counting or frequency division
    T1_Load(j);                           // 1ms load Timer1 init value
    Timer1_Interrupt(DISABLE);            // Timer1 Interrupt Disable
    Timer1_CLK_Output(DISABLE);           // Output pin clk enable
    UART1_BRT_Use_Timer1();               // Select BRT Use Timer1 and used as a timer
    // interrupt config
    UART1_Priority(Priority_2); // From low to high: Priority_0,Priority_1,Priority_2,Priority_3
    UART1_Interrupt(ENABLE);    // uart interrupt: ENABLE/DISABLE;
    // select port
    UART1_SW(UART1_SW_P30_P31); // Port select: UART1_SW_P30_P31
    UART1_RxEnable(ENABLE);     // enable Recv

    Timer1_Run(); // Start Timer1
    //====================================================================================
    EA = 1; // enable global interrupt
}