#include "Record_Warning_Less_Time.h"

// logo
#include "confirm_logo.h"
#include "Return_logo.h"


extern UI_Manager ui_manager;

Record_Warning_Less_Time::Record_Warning_Less_Time(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Record_Warning_Less_Time;
}
Record_Warning_Less_Time::~Record_Warning_Less_Time()
{
}

void Record_Warning_Less_Time::Draw_Static()
{
    Draw_UI();
}

void Record_Warning_Less_Time::Draw_Update()
{
    Update_UI();
}

void Record_Warning_Less_Time::Handle_Button()
{
    MyCobot_Pro_450_Select();
}

void Record_Warning_Less_Time::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);
    tft.print("Record");

    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Sure to return without", 45, 90);
    tft.drawString("saving?", 115, 120);

    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Record_Warning_Less_Time::Update_UI()
{

}

void Record_Warning_Less_Time::MyCobot_Pro_450_Select()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN3)
    {
        button.Wait();
        Clear_Record_450();
        ui_manager.total_time = 0;
        ui_manager.Go_To(ScreenID::ScreenID_Drag_Teach);
    }

    if (btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}
