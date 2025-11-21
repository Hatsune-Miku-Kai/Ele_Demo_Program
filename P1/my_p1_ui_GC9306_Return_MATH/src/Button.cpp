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
    vTaskDelay(20); // 等待10ms,防抖
    return status;
}


ButtonEvent Button::Get_Press_Type()
{
    static uint8_t last_btn = 0;
    static uint32_t press_start = 0;
    static bool long_triggered = false;

    const TickType_t SHORT_PRESS_TIME = pdMS_TO_TICKS(100);
    const TickType_t LONG_PRESS_TIME  = pdMS_TO_TICKS(800);

    uint8_t btn = Get_Button_Status();
    TickType_t now = xTaskGetTickCount();

    if (btn != 0 && last_btn == 0)
    {
        press_start = now;
        last_btn = btn;
        long_triggered = false;
    }
    else if (btn != 0 && last_btn == btn)
    {
        TickType_t duration = now - press_start;
        if (!long_triggered && duration >= LONG_PRESS_TIME)
        {
            long_triggered = true;
            return { LONG_PRESS_TYPE, btn };
        }
    }
    else if (btn == 0 && last_btn != 0)
    {
        TickType_t duration = now - press_start;
        bool was_long = long_triggered;
        uint8_t released_btn = last_btn;

        last_btn = 0;
        long_triggered = false;

        if (!was_long && duration >= SHORT_PRESS_TIME && duration < LONG_PRESS_TIME)
        {
            return { SHORT_PRESS_TYPE, released_btn };
        }
    }

    return { 0, 0 };
}

void Button::Wait()
{
    while (Get_Button_Status() != 0)
    {
        vTaskDelay(10); // 等待松手
    }
}
