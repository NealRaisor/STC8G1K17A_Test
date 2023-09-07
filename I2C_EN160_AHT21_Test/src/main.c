#include "System_config.h"
#include "ENS160_I2C.h"
#include "AHT21_I2C.h"

static void Deal_COM1_RX_Data(void);
static void Show_Aht21_Data(void);
static void Show_Ens160_Data(void);

static u8 xdata Deal_Buf[COM1_RX_Len]; // 信息暂存处，最长可暂存一次满串口数据。
static bit Flag_echo = 1;              // 回显标志

// 异步任务标志
bit Flag_ENS160 = 0; // ENS160任务标志
bit Flag_AHT21  = 0; // AHT21任务标志

void main(void)
{

    System_config();
    delay_ms(2000);
    printf("System init OK!\r\n");

    ENS160_Init(); // ENS160初始化
    AHT21_Init();  // AHT21初始化
    printf("Device init OK!\r\n");

    LOOP
    {
        // 常驻串口数据处理函数
        Deal_COM1_RX_Data();

        // 每1000ms测量一次AHT21数据; 需要一秒执行俩次，一次开始测量，一次读取数据。
        if (Flag_AHT21 == 1) {
            Flag_AHT21 = 0;
            Show_Aht21_Data();
        }

        // 每1000ms测量一次ENS160数据
        if (Flag_ENS160 == 1) {
            Flag_ENS160 = 0;
            Show_Ens160_Data();
        }
    }
}

static void Deal_COM1_RX_Data(void) /* 处理串口数据 */
{
    static u8 data num, i;

    if (Flag_echo == 0) return;                     // 回显关闭
    num = Read_RX_BUF(&COM1, RX1_Buffer, Deal_Buf); // 读取串口数据
    if (num == 0) return;                           // 为空判断

    // 处理串口数据
    printf("num:%bu\r\n", num);
    for (i = 0; i < num; ++i) {
        UART_SendByte(Deal_Buf[i]);
    }
}

static void Show_Aht21_Data(void) // 显示AHT21数据
{
    AHT21_DATA tmp_data;
    tmp_data.Humi = UINT8_MAX;
    tmp_data.Temp = UINT8_MAX;

    // 测量数据
    AHT21_Measure();
    AHT21_Read_Data(&tmp_data);

    // 处理数据，显示
    if ((tmp_data.Humi == UINT8_MAX) || (tmp_data.Temp == UINT8_MAX)) {
        return;
    }

    printf("Humi:%bu\r\n", tmp_data.Humi);
    // sprintf((char *)INFO_BUF, "Temp:%bu", tmp_data.Temp); // 末尾自动添加'\0'
    // OLED_ShowText(66, 2, INFO_BUF, 8);
    printf("Temp:%bd\r\n", tmp_data.Temp);
    // sprintf((char *)INFO_BUF, "Humi:%bd", tmp_data.Humi); // 末尾自动添加'\0'
    // OLED_ShowText(66, 3, INFO_BUF, 8);
}

static void Show_Ens160_Data(void)
{
    u8 tmp_8   = 0;
    u16 tmp_16 = 0;

    // 读取数据
    ENS160_Get_DATA_AQI(&tmp_8);
    printf("AQI:%bu\r\n", tmp_8);
    // sprintf((char *)INFO_BUF, " AQI:%4d", tmp_8); // 末尾自动添加'\0'
    // OLED_ShowText(0, 1, INFO_BUF, 8);

    ENS160_Get_DATA_TVOC(&tmp_16);
    printf("TVOC:%hu\r\n", tmp_16);
    // sprintf((char *)INFO_BUF, "TVOC:%4d", tmp_16); // 末尾自动添加'\0'
    // OLED_ShowText(0, 2, INFO_BUF, 8);

    ENS160_Get_DATA_ECO2(&tmp_16);
    printf("ECO2:%hu\r\n", tmp_16);
    // sprintf((char *)INFO_BUF, "ECO2:%4d", tmp_16); // 末尾自动添加'\0'
    // OLED_ShowText(0, 3, INFO_BUF, 8);
}