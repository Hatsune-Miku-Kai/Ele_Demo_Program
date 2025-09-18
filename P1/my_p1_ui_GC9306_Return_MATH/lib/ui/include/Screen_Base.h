#ifndef __SCREEN_BASE_H__
#define __SCREEN_BASE_H__   

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"

//用以表示每个UI的界面,并为每个UI提供绘制和事件处理的接口
class Screen_Base
{
public:

    enum ScreenID
    {
        ScreenID_Home,
        ScreenID_Program,
        ScreenID_Drag_Teach,
        ScreenID_BlocklyRunner,
        ScreenID_QuickMove,
        ScreenID_Connection,
        ScreenID_Firmware,
        ScreenID_Record,
        ScreenID_Play,
        ScreenID_Save,
        ScreenID_Play_From_RAM_OR_FLASH,
        ScreenID_Error,
        ScreenID_WIFI_Information,
        ScreenID_Save_To_Blockly_Runner,
        ScreenID_IO,
        ScreenID_Connection_Status,
        ScreenID_WLAN_Information,
        ScreenID_WLAN_Program,
        ScreenID_Calibration,
        SCREEN_ID_AUTOMATIC_CALIBRATION,
        SCREEN_ID_CALIBRATION_JOINT,
        ScreenID_Quick_Move,
        ScreenID_Free_Move,
        ScreenID_Jog_Move
    };

    explicit Screen_Base(TFT_eSPI& tft, Button& button);

    virtual void Draw_Static() = 0;

    virtual void Draw_Update() = 0;

    virtual void Handle_Button() = 0;

    virtual ~Screen_Base();

    ScreenID id; //每个界面都有一个ID

    Screen_Base* prev_screen = nullptr;//上一个界面结点指针
    
//保留,看看以后用不用
    //Screen_Base* target_screen = nullptr;//需要跳转的指定页面结点指针

protected://UI管理器不可以访问
    TFT_eSPI& tft;  // 所有页面共享一个屏幕
    Button& button;  // 所有页面共享一个按键对象

};


#endif

