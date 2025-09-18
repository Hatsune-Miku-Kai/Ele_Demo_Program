#include "Connect.h"




Connect::Connect(TFT_eSPI &display) : tft(display)
{

}

Connect::~Connect()
{

}

void Connect::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("Connection");



    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 80);
    tft.print("WiFi");
    tft.setCursor(10, 120);
    tft.print("USB");
    tft.setCursor(10, 160);
    tft.print("BlueTooth");

}