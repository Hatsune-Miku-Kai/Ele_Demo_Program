#include "Connection_Status.h"

//logo
#include "Program_logo.h"
#include "Coord_logo.h"
#include "IO_logo.h"
#include "Connect_logo.h"
#include "Not_Connect_logo.h"

extern UI_Manager ui_manager;

Connection_Status::Connection_Status(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    Serial.println("Connection_Status created");
    id = ScreenID::ScreenID_Connection_Status;
}

Connection_Status::~Connection_Status()
{
    Serial.println("Connection_Status destroyed");
}

void Connection_Status::Draw_Static()
{
    Draw_UI();
}

void Connection_Status::Draw_Update()
{
    Update_UI();
}

void Connection_Status::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    if(btn == BTN1)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_Program)))
        {
            Program* program = new Program(tft, button);
            ui_manager.RegisterScreen(program);//将页面放入注册列表
            button.Wait();
            ui_manager.Change_UI(program, true);
        }
    }

    else if(btn == BTN3)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_IO)))
        {
            IO* io = new IO(tft, button);
            ui_manager.RegisterScreen(io);//将页面放入注册列表
            button.Wait();
            ui_manager.Change_UI(io, true);
        }
    }

    else if(btn == BTN2)
    {
        ui_manager.Go_To(ScreenID::ScreenID_Home);
    }
}

void Connection_Status::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("Connection Status");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(20, 80);
    tft.print("WiFi");
    tft.setCursor(20, 120);
    tft.print("USB");
    tft.setCursor(20, 160);
    tft.print("BlueTooth");


    tft.pushImage(220, 65, 30, 18, connect_logo);
    tft.pushImage(220, 105, 30, 18, not_connect_logo);
    tft.pushImage(220, 145, 30, 18, not_connect_logo);

    tft.pushImage(15, 210, 30, 30, program_logo);
    tft.pushImage(105, 208, 30, 30, coord_logo);
    tft.pushImage(190, 210, 30, 30, io_logo);
    tft.pushImage(275, 210, 30, 18, connect_logo);

    tft.fillRect(280, 235, 20, 3, TFT_WHITE);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Connection_Status::Update_UI()
{

}