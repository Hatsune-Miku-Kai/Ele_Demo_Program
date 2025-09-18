#include "Error.h"

Error::Error(TFT_eSPI &display) : tft(display)
{
}

Error::~Error()
{
}


void Error::Draw_UI(const String& text)
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("UltraArm P1");

    // 右上角红圈对勾
    tft.fillCircle(tft.width() - 20, 20, 10, TFT_RED);
    // tft.setTextColor(TFT_BLACK); 
    // tft.setCursor(tft.width() - 25, 25);
    // tft.print("V");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    //显示界面的红圈
    tft.fillCircle(30, 90, 10, TFT_RED);
    tft.setCursor(50, 98);
    tft.printf("Error");

    tft.setCursor(70, 140);
    tft.printf("%s", text.c_str());
}



