#include "IO.h"
#include "Program_logo.h"
#include "Coord_logo.h"
#include "IO_logo.h"
#include "Connect_logo.h"

extern UI_Manager ui_manager;


IO::IO(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    Serial.println("IO created");
    id = ScreenID::ScreenID_IO;
}

IO::~IO()
{
    Serial.println("IO destroyed");
}

void IO::Draw_Static()
{
    Draw_UI();
}

void IO::Draw_Update()
{
    
}

void IO::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN2)
    {
        button.Wait();
        ui_manager.Go_To(ScreenID::ScreenID_Home);
    }

    else if(btn == BTN4)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_Connection_Status)))
        {
            Connection_Status* connection_status = new Connection_Status(tft, button);
            ui_manager.RegisterScreen(connection_status);//将页面放入注册列表
            button.Wait();
            ui_manager.Change_UI(connection_status, true);
        }
    }
}


void IO::Draw_UI()
{

    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("I/O Status");

    tft.fillRect(195, 235, 18, 3, TFT_WHITE);

    tft.pushImage(15, 210, 30, 30, program_logo);
    tft.pushImage(105, 208, 30, 30, coord_logo);
    tft.pushImage(190, 202, 30, 30, io_logo);
    tft.pushImage(275, 217, 30, 18, connect_logo);


    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}