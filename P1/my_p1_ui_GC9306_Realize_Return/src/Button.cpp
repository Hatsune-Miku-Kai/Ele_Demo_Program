#include "Button.h"
#include <driver/adc.h>
#include <Arduino.h>

void Button::Init()
{
    // 初始化ADC1通道4和通道5
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12); // btn3 btn4
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_12); // btn1 btn2
}

uint8_t Button::Get_Button_Status()
{
    uint8_t status = 0;
    // 读取 ADC4
    int adc4 = 0;
    for (int i = 0; i < 100; i++)
    {
        adc4 += adc1_get_raw(ADC1_CHANNEL_4);
    //     delay(2);
    }
    adc4 /= 100; // 简单平均
    float voltage4 = adc4 * 3.3f / 4095.0f;

    if (voltage4 < 1.0f)
        status = BTN3_4;
    else if (voltage4 < 2.2f)
        status = BTN4;
    else if (voltage4 < 2.6f)
        status = BTN3;

    // 读取 ADC5
    int adc5 = 0;
    for (int i = 0; i < 100; i++)
    {
        adc5 += adc1_get_raw(ADC1_CHANNEL_5);
    //     delay(2);
    }
    adc5 /= 100;
    float voltage5 = adc5 * 3.3f / 4095.0f;

    if (voltage5 < 1.0f)
        status = BTN1_2;
    else if (voltage5 < 2.2f)
        status = BTN2;
    else if (voltage5 < 2.6f)
        status = BTN1;

    return status;
    delay(10);
}

void Button::Wait()
{
    while (Get_Button_Status() != 0)
    {
        delay(10); // 等待松手
    }
}
