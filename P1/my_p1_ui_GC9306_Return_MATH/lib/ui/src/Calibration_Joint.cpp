#include "Calibration_Joint.h"
#include "SPI.hpp"

//logo
#include "Start_logo.h"
#include "Stop_logo.h"
#include "Cannot_Save_logo.h"
#include "Can_Save_logo.h"
#include "Return_logo.h"
#include "Cannot_Pause_logo.h"
#include "Cannot_Stop_logo.h"


extern UI_Manager ui_manager;

uint8_t Joint_Index;

Calibration_Joint::Calibration_Joint(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::SCREEN_ID_CALIBRATION_JOINT;
}

Calibration_Joint::~Calibration_Joint()
{
}

void Calibration_Joint::Draw_Static()
{
    Screen_Base* screen = ui_manager.GetRegisteredScreenByID(ScreenID::SCREEN_ID_AUTOMATIC_CALIBRATION);
    if(screen != nullptr)
    {
        Automatic_Calibration* temp = static_cast<Automatic_Calibration*>(screen);
        Joint_Index = temp->selectedIndex + 1;
    }
    Draw_UI(Joint_Index);

}

void Calibration_Joint::Draw_Update()
{
    // 绘制动态界面
}

void Calibration_Joint::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN1 && Calibration_Status == 0)
    {
        button.Wait();
        tft.fillRect(15, 205, 30, 32, TFT_BLACK);
        tft.fillRect(105, 205, 30, 30, TFT_BLACK);

        tft.pushImage(15, 205, 30, 32, cannot_pause_logo);
        tft.pushImage(105, 205, 30, 30, stop_logo);


        tft.fillRect(0, 50, 400, 20, TFT_BLACK);
        tft.setCursor(30, 65);
        tft.setTextColor(TFT_YELLOW);
        tft.setFreeFont(&FreeSansBold9pt7b);
        String msg = "J" + String(Joint_Index) + " Calibrating" + "...";
        tft.print(msg.c_str());
        Calibration_Status = 1;
    }

    else if(btn == BTN2 && Calibration_Status == 1)
    {
        button.Wait();
        tft.fillRect(0, 50, 400, 20, TFT_BLACK);

        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.setCursor(30, 65);
        tft.setTextColor(TFT_YELLOW);
        tft.print("Update the Zero position?");


        tft.fillRect(15, 205, 30, 32, TFT_BLACK);
        tft.fillRect(105, 205, 30, 32, TFT_BLACK);
        tft.fillRect(190, 208, 30, 32, TFT_BLACK);

        tft.pushImage(190, 205, 30, 30, can_save_logo);
        Calibration_Status = 2;
    }

    else if(btn == BTN3 && Calibration_Status == 2)
    {
        button.Wait();

        String msg = "J" + String(Joint_Index) + " Updated!";
        tft.fillRect(0, 50, 400, 20, TFT_BLACK);
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.setCursor(50, 65);
        tft.setTextColor(TFT_GREEN);
        tft.print(msg.c_str());
        tft.fillRect(190, 208, 30, 30, TFT_BLACK);
        Calibration_Status = 2;
    }

    else if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}

void Calibration_Joint::Draw_UI(uint8_t index)
{
    String msg = "Ready to calibrate J" + String(index) + ".";
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("Automatic");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(30, 65);
    tft.print(msg.c_str());

    tft.setTextColor(TFT_WHITE);
    tft.setCursor(45, 90);
    tft.print("Current   :");

    tft.setCursor(45, 120);
    tft.print("NegLimit :");

    tft.setCursor(45, 150);
    tft.print("PosLimit :");

    tft.setCursor(45, 180);
    tft.print("Offset     :");

    tft.pushImage(15, 205, 30, 30, start_logo);
    tft.pushImage(105, 205, 30, 30, cannot_stop_logo);
    tft.pushImage(190, 208, 30, 30, cannot_save_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Calibration_Joint::Update_UI()
{
    // 更新界面
}
