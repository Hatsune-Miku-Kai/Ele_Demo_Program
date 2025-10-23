#include "Screen_Base.h"
#include "Error.h"

class UI_Manager;
extern UI_Manager ui_manager;

Screen_Base::Screen_Base(TFT_eSPI& tft, Button& button) : tft(tft), button(button)
{
    // Serial.println("Screen_Base created");
}


Screen_Base::~Screen_Base() 
{ 
    // Serial.println("Screen_Base destroyed");
}

void Screen_Base::Handle_Robot_States_Data(std::map<std::string, uint16_t>& robot_states)
{
    for(std::map<std::string, uint16_t>::iterator it = robot_states.begin(); it != robot_states.end(); it++)
    {
        if(it->second != 0 && (it->first != "Robot Crashed" || it->first != "Robot Moving"))
        {
           
            Error* error = new Error(tft, button);
            ui_manager.RegisterScreen(error);
            ui_manager.Change_UI(error, false);
        }
    }
}


void Screen_Base::Get_Robot_State_Data(std::vector<uint8_t> &data, std::map<std::string, uint16_t> &robot_states)
{
//获取机械臂状态
//将机械臂的状态更新到唯一的全局变量robot_states中可以给所有界面访问

    std::map<std::string, uint16_t>::iterator it = robot_states.begin();

    robot_states["Power Code"] = data[IP_POWER_ON_START];

    for (int i = ERROR_DATA_START; i < ERROR_DATA_LEN ; i++)
    {
        it->second = data[i];
        it++;
    }

    uint16_t len = ERROR_DATA_START + ERROR_DATA_LEN - 24;

    for (int i = len; i < len + 24; i + 2)
    {
        it->second = data[i] << 8 | data[i + 1];
        it++;
    }
}