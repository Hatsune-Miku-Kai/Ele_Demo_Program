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
    Update_UI();
}

void IO::Handle_Button()
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

    else if(btn == BTN2)
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

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 100);
    tft.print("Input:");

    tft.setCursor(100, 70);
    tft.print("1");

    tft.setCursor(170, 70);
    tft.print("2");

    // 右列 Angle
    tft.setCursor(10, 165);
    tft.print("Ouput:");

    tft.fillCircle(105, 95, 13, TFT_DARKGREY);//Input1状态
    tft.fillCircle(175, 95, 13, TFT_DARKGREY);//Input2状态

    tft.fillCircle(105, 160, 13, TFT_DARKGREY);//Output1状态
    tft.fillCircle(175, 160, 13, TFT_DARKGREY);//Output2状态

    tft.pushImage(15, 210, 30, 30, program_logo);
    tft.pushImage(105, 208, 30, 30, coord_logo);
    tft.pushImage(190, 202, 30, 30, io_logo);
    tft.pushImage(275, 217, 30, 18, connect_logo);

    tft.fillRect(195, 235, 18, 3, TFT_WHITE);
    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

}


void IO::Update_UI()
{
    //目前不检测IO状态,写死显示

    tft.fillCircle(105, 95, 8, TFT_WHITE);//Input1状态
    //tft.fillCircle(175, 95, 8, TFT_WHITE);//Input2状态

    //tft.fillCircle(105, 160, 8, TFT_WHITE);//Output1状态
    tft.fillCircle(175, 160, 8, TFT_WHITE);//Output2状态
}