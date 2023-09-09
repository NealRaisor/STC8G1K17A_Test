#include "System_config.h"
#include "ENS160_I2C.h"
#include "AHT21_I2C.h"
#include "SSD1315_I2C.h"

static void Deal_COM1_RX_Data(void);
static void Show_Aht21_Data(void);
static void Show_Ens160_Data(void);
static void Show_Main_Menu(void);

static u8 xdata Deal_Buf[COM1_RX_Len]; // 信息暂存处，最长可暂存一次满串口数据。
static u8 data tmp_data_8bit, i;
static u16 data tmp_data_16bit;
static AHT21_DATA tmp_aht_data;
static bit Flag_echo = 1; // 回显标志

// 异步任务标志
bit Flag_ENS160 = 0; // ENS160任务标志
bit Flag_AHT21  = 0; // AHT21任务标志

void main(void)
{
    System_config(); // 系统统一配置
    ENS160_Init();   // ENS160初始化
    AHT21_Init();    // AHT21初始化
    OLED_Init();     // OLED初始化

    Show_Main_Menu();
    LOOP
    {
        // 常驻串口数据处理函数
        Deal_COM1_RX_Data();

        // 每800ms测量一次AHT21数据; 需要一秒执行俩次，一次开始测量，一次读取数据。
        if (Flag_AHT21 == 1) {
            Flag_AHT21 = 0;
            Show_Aht21_Data();
        }

        // 每1s测量一次ENS160数据
        if (Flag_ENS160 == 1) {
            Flag_ENS160 = 0;
            Show_Ens160_Data();
        }
    }
}

// 显示主界面
static void Show_Main_Menu(void)
{
    OLED_ShowText(42, 0, "Air Info", 8);
    OLED_ShowText(31, 2, "<-|State|->", 8);

    OLED_ShowText(6, 1, "ENS160", 8);
    OLED_ShowText(0, 3, "ECO2:", 8);
    OLED_ShowText(0, 4, "TVOC:", 8);
    OLED_ShowText(0, 5, " AQI:", 8);

    OLED_ShowText(85, 1, "AHT21", 8);
    OLED_ShowText(78, 4, "Temp:", 8);
    OLED_ShowText(78, 5, "Humi:", 8);
    OLED_ShowText(0, 7, "BLE:NG", 8);
}

static void Deal_COM1_RX_Data(void) /* 处理串口数据, 常驻 */
{
    tmp_data_8bit = Read_RX_BUF(&COM1, RX1_Buffer, Deal_Buf); // 读取串口数据
    if (tmp_data_8bit == 0) return;                           // 为空判断

    if (Flag_echo) { // 回显
        for (i = 0; i < tmp_data_8bit; ++i) {
            UART_SendByte(Deal_Buf[i]);
        }
    }
    // 处理串口数据
}

static void Show_Aht21_Data(void) // 显示AHT21数据
{
    tmp_aht_data.Humi = UINT8_MAX; // 拉高
    tmp_aht_data.Temp = UINT8_MAX;

    AHT21_Measure();                // 测量数据
    AHT21_Read_Data(&tmp_aht_data); // 读取数据

    if ((tmp_aht_data.Humi == UINT8_MAX) || (tmp_aht_data.Temp == UINT8_MAX)) // 有效判断
        return;
    // 信息显示
    sprintf((char *)Deal_Buf, "0"); // 末尾自动添加'\0'
    OLED_ShowText(100, 2, Deal_Buf, 8);

    sprintf((char *)Deal_Buf, "%3bd", tmp_aht_data.Temp); // 末尾自动添加'\0'
    OLED_ShowText(108, 4, Deal_Buf, 8);

    sprintf((char *)Deal_Buf, "%3bu", tmp_aht_data.Humi); // 末尾自动添加'\0'
    OLED_ShowText(108, 5, Deal_Buf, 8);
}

static void Show_Ens160_Data(void)
{
    ENS160_Get_DEVICE_STATUS(&tmp_data_8bit);                                     // 读取状态
    sprintf((char *)Deal_Buf, "%1bu", ENS160_DEVICE_STATUS_VALID(tmp_data_8bit)); // 末尾自动添加'\0'
    OLED_ShowText(22, 2, Deal_Buf, 8);

    ENS160_Get_DATA_ECO2(&tmp_data_16bit);             // 读取ECO2数据
    sprintf((char *)Deal_Buf, "%5hu", tmp_data_16bit); // 末尾自动添加'\0'
    OLED_ShowText(30, 3, Deal_Buf, 8);

    ENS160_Get_DATA_TVOC(&tmp_data_16bit);             // 读取TVOC数据
    sprintf((char *)Deal_Buf, "%5hu", tmp_data_16bit); // 末尾自动添加'\0'
    OLED_ShowText(30, 4, Deal_Buf, 8);

    ENS160_Get_DATA_AQI(&tmp_data_8bit);              // 读取AQI数据
    sprintf((char *)Deal_Buf, "%3bu", tmp_data_8bit); // 末尾自动添加'\0'
    OLED_ShowText(30, 5, Deal_Buf, 8);
}