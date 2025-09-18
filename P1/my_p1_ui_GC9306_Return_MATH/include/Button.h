#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// 按键定义
#define BTN1   1
#define BTN2   2
#define BTN3   3
#define BTN4   4
#define BTN1_2 5    // BTN1 + BTN2
#define BTN3_4 6    // BTN3 + BTN4

// 按键板类
class Button
{
public:
    void Init();               // 初始化
    uint8_t Get_Button_Status(); // 读取按键状态
    void Wait();
};

#endif 
