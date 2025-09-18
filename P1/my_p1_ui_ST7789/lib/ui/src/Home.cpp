#include "Home.h"


Home::Home(TFT_eSPI &display) : tft(display)
{
}

Home::~Home()
{
}

void Home::drawStaticUI()
{
    // 背景
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("UltraArm P1");

    // 右上角绿色对勾
    tft.fillCircle(tft.width() - 20, 20, 10, TFT_GREEN);
    // tft.setTextColor(TFT_BLACK);
    // tft.setCursor(tft.width() - 25, 25);
    // tft.print("V");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 左列 Coord
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 65);
    tft.print("Coord");

    // 右列 Angle
    int rightX = 150;
    tft.setCursor(rightX, 65);
    tft.print("Angle");
}

void Home::Draw_UI()
{
    drawStaticUI();
    UpdateCoord(-120.0, -120.0, -120.0, -120.0);
    UpdateAngle(-120.0, 40.0, 20.0, -120.0);
}

void Home::UpdateCoord(float x, float y, float z, float o)
{
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // 背景黑色，防止重影
    tft.setCursor(10, 90);
    tft.printf("X:%6.1fmm", x);

    tft.setCursor(10, 115);
    tft.printf("Y:%6.1fmm", y);

    tft.setCursor(10, 140);
    tft.printf("Z:%6.1fmm", z);

    tft.setCursor(10, 165);
    tft.printf("Theta:%6.1f", o);
    // tft.printf("°");
}

void Home::UpdateAngle(float j1, float j2, float j3, float j4)
{
    int rightX = 150;
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(rightX, 90);
    tft.printf("J1:%6.1f Degrees", j1);
    tft.setCursor(rightX, 115);
    tft.printf("J2:%6.1f Degrees", j2);
    tft.setCursor(rightX, 140);
    tft.printf("J3:%6.1f Degrees", j3);
    tft.setCursor(rightX, 165);
    tft.printf("J4:%6.1f Degrees", j4);
}
