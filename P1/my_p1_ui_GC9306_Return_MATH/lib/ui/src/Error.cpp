#include "Error.h"

extern UI_Manager ui_manager;

// logo
#include "Return_logo.h"


//Global_Data
#include "Global_Data.h"

int count = 0;

Error::Error(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Error;
}

Error::~Error()
{
}

void Error::Draw_Static()
{
    Draw_UI();
}

void Error::Draw_Update()
{
    Handle_Error_Data(Global_Data::Data);
    // Update_UI(Error_Status);
}

void Error::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Home();
    }
}

void Error::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("MyCobot Pro 450");

    // 右上角红色圆圈
    tft.fillCircle(tft.width() - 30, 20, 10, TFT_RED);

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    // 中间红色圆圈
    tft.fillCircle(60, 60, 10, TFT_RED);

    tft.setCursor(73, 67);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.print("Error");

    while(Global_Data::Robot_States["Power Code"] == 0 || Global_Data::Robot_States["Robot Crashed"] == 2)
    {
        tft.setCursor(95, 125);
        tft.setTextColor(TFT_WHITE);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.print("Power Code:");
        tft.printf(" %d", Global_Data::Robot_States["Power Code"]);
        Get_Robot_State_Data(Global_Data::Data, Global_Data::Robot_States);
    }

}

void Error::Update_UI(std::map<std::string, uint16_t>& Error_Status)
{
    Get_Robot_State_Data(Global_Data::Data, Global_Data::Robot_States);
    for(std::map<std::string, uint16_t>::iterator it = Global_Data::Robot_States.begin(); it != Global_Data::Robot_States.end(); it++)
    {
        if(it->second == 0 && it->first != "Robot Crashed" && it->first != "Robot Moving")
        {
            ui_manager.Go_Home();
        }
    }
//角度超限错误
    for(std::map<std::string, uint16_t>::iterator it = Error_Status.begin(); it != Error_Status.end() && count < 9; it++)
    {
        if(it->second != 0 && it->first != "Robot Crashed" && it->first != "Robot Moving")
        {
            Over_Limit_Error_Msg += it->first + ", ";
        }
        count++;
    }

    if (!Over_Limit_Error_Msg.empty())
    {
        Over_Limit_Error_Msg.erase(Over_Limit_Error_Msg.size() - 2);
        Over_Limit_Error_Msg += " Over Limit";
        tft.setCursor(0, 30);
        tft.setTextColor(TFT_WHITE);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.print(Over_Limit_Error_Msg.c_str());
    }

//电机错误
    for(std::map<std::string, uint16_t>::iterator it = Error_Status.begin(); it != Error_Status.end() && count < 15; it++)
    {
        if(it->second != 0)
        {
            Motor_Error_Msg += it->first + ", ";
        }
        count++;
    }

    if (!Motor_Error_Msg.empty())
    {
        Motor_Error_Msg.erase(Motor_Error_Msg.size() - 2);
        Motor_Error_Msg += " Error";
        tft.setCursor(0, 50);
        tft.setTextColor(TFT_WHITE);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.print(Motor_Error_Msg.c_str());
    }

//软件报错
    for(std::map<std::string, uint16_t>::iterator it = Error_Status.begin(); it != Error_Status.end() && count < 21; it++)
    {
        if(it->second != 0)
        {
            Motor_Error_Msg += it->first + ", ";
        }
        count++;
    }
    
    if (!Software_Error_Msg.empty())
    {
        Software_Error_Msg.erase(Software_Error_Msg.size() - 2);
        Software_Error_Msg += " Error";
        tft.setCursor(0, 70);
        tft.setTextColor(TFT_WHITE);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.print(Software_Error_Msg.c_str());
    }

        Software_Error_Msg = "";
        Over_Limit_Error_Msg = "";
        Motor_Error_Msg = "";
}

void Error::Handle_Error_Data(std::vector<uint8_t> &data)
{
    uint16_t Error_Case = 0;
    // int idx = 0;
    // for (int i = ERROR_DATA_START; i < ERROR_DATA_LEN - 24; i++)
    // {
    //     Error_Status[idx++].second = data[i];
    // }

    // uint16_t len = ERROR_DATA_START + ERROR_DATA_LEN - 24;

    // for (int i = ERROR_DATA_START; i < len; i + 2)
    // {
    //     Error_Status[idx++].second = data[i] << 8 | data[i + 1];
    // }
}


// bool Error::Is_Error_Active()
// {
//    for(int i = 0; i < Error_Status.size(); i++)
//    {
//        if(Error_Status[i].second != 0)
//        {
//            return true;
//        }
//    }
//    return false;
// }