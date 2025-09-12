#ifndef __SCREEN_BASE_H__
#define __SCREEN_BASE_H__   

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

//用以表示每个UI的界面,并为每个UI提供绘制和事件处理的接口
class Screen_Base
{
public:

    explicit Screen_Base(TFT_eSPI& tft, Button& button);

    virtual void Draw_Static() = 0;

    virtual void Draw_Update() = 0;

    virtual void Handle_Button() = 0;

    virtual ~Screen_Base();




    Screen_Base* prev_screen = nullptr;//上一个界面结点指针
    
    Screen_Base* target_screen = nullptr;//需要跳转的指定页面结点指针

protected://UI管理器不可以访问
    TFT_eSPI& tft;  // 所有页面共享一个屏幕
    Button& button;  // 所有页面共享一个按键对象

};


#endif

