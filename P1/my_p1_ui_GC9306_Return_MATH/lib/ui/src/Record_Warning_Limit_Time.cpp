#include "Record_Warning_Limit_Time.h"

extern UI_Manager ui_manager;

// logo
#include "Return_logo.h"
#include "confirm_logo.h"

Record_Warning_Limit_Time::Record_Warning_Limit_Time(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Record_Warning_Limit_Time;
}

Record_Warning_Limit_Time::~Record_Warning_Limit_Time()
{
}

void Record_Warning_Limit_Time::Draw_Static()
{
    MyCobot_Pro_450_UI();
}

void Record_Warning_Limit_Time::Draw_Update()
{
    MyCobot_Pro_450_Update();
}

void Record_Warning_Limit_Time::Handle_Button()
{
#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_Select();
#endif
}

void Record_Warning_Limit_Time::MyCobot_Pro_450_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);
    tft.print("Record");

    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Warning:", 110, 70);
    tft.drawString("Recording time limit", 50, 100);
    tft.drawString("reached.", 30, 130);
    tft.drawString("Please choose", 130, 130);
    tft.drawString("to save or return!", 70, 160);

    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.pushImage(190, 208, 30, 30, can_save_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Record_Warning_Limit_Time::MyCobot_Pro_450_Select()
{
    uint8_t btn = button.Get_Button_Status();

    if (btn == BTN3)
    {
        button.Wait();
        Save *save = new Save(tft, button);
        ui_manager.RegisterScreen(save);
        ui_manager.Change_UI(save, true);
    }

    else if (btn == BTN4)
    {
        button.Wait();
        Record_Warning_Less_Time *record_warning_less_time = new Record_Warning_Less_Time(tft, button);
        ui_manager.RegisterScreen(record_warning_less_time);
        ui_manager.Change_UI(record_warning_less_time, true);
    }
}

void Record_Warning_Limit_Time::MyCobot_Pro_450_Update()
{
}