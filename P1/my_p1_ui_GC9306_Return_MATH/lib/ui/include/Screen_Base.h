#ifndef __SCREEN_BASE_H__
#define __SCREEN_BASE_H__   

// #define UltraArm_P1
#define MyCobot_Pro_450

//所有界面都需要不断地去获取错误状态进而判断是否需要显示错误界面
#define ERROR_DATA_START 24
#define ERROR_DATA_LEN 32

#define IP_POWER_ON_START 80

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Button.h"
#include <map>
#include <string>
#include "SPI.hpp"
#include "Cmd.h"

//Global_Data
#include "Global_Data.h"

// using namespace std;

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
        ScreenID_Jog_Select_Move,
        ScreenID_Coords
    };

    explicit Screen_Base(TFT_eSPI& tft, Button& button);
    
    virtual ~Screen_Base();

    virtual void Draw_Static() = 0;

    virtual void Draw_Update() = 0;

    virtual void Handle_Button() = 0;

    // virtual void Handle_Data(std::vector<uint8_t>& data) = 0;//第一版不加,但是后面一定要加
    void Get_Robot_State_Data(std::vector<uint8_t> &data, std::map<std::string, uint16_t> &robot_states);//从数据中提取机械臂状态

    void Handle_Robot_States_Data(std::map<std::string, uint16_t>& robot_states);//给所有子类提供处理机械臂状态的接口

    ScreenID id; //每个界面都有一个唯一ID,用于识别和管理不同的界面,只要不赋重复值就不会冲突

    Screen_Base* prev_screen = nullptr;//上一个界面结点指针
    
//保留,看看以后用不用
    //Screen_Base* target_screen = nullptr;//需要跳转的指定页面结点指针

protected://UI管理器不可以访问
    TFT_eSPI& tft;  // 所有页面共享一个屏幕
    Button& button;  // 所有页面共享一个按键对象

};


#endif

