#include "SetUp.h"
#include <driver/adc.h>
#include <Arduino.h>

void Board::Init()
{
    // 初始化ADC1通道4和通道5
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12); // btn3 btn4
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_12); // btn1 btn2
}

uint8_t Board::Get_Button_Status()
{
    uint8_t status = 0;
    // 读取 ADC4
    int adc4 = adc1_get_raw(ADC1_CHANNEL_4);
    float voltage4 = adc4 * 3.3f / 4095.0f;

    if (voltage4 < 1.0f) status = BTN3_4;     // Key3+Key4
    else if (voltage4 < 1.8f) status = BTN4;  // Key4
    else if (voltage4 < 2.4f) status = BTN3;  // Key3

    // 读取 ADC5
    int adc5 = adc1_get_raw(ADC1_CHANNEL_5);
    float voltage5 = adc5 * 3.3f / 4095.0f;

    if (voltage5 < 1.0f) status = BTN1_2;     // Key1+Key2
    else if (voltage5 < 1.8f) status = BTN2;  // Key2
    else if (voltage5 < 2.4f) status = BTN1;  // Key1

    if (status != 0)
        return status;

    return 0;
}

