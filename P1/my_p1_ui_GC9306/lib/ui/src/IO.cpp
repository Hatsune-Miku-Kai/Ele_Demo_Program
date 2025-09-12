#include "IO.h"


IO::IO(TFT_eSPI &display) : tft(display)
{


}


IO::~IO()
{


}


void IO::Draw_UI()
{
    // 背景
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("I/O Status");


    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSans9pt7b);
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

    tft.pushImage(10, 210, 30, 30, program_logo);
    tft.pushImage(50, 210, 30, 30, return_logo);
    tft.pushImage(90, 210, 30, 30, connect_logo);

}