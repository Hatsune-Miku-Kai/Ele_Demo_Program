#include "Home.h"

extern UI_Manager ui_manager;


Home::Home(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    Serial.println("Home created");
}


Home::~Home()
{
    Serial.println("Home destroyed");
}


void Home::Draw_Static()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("UltraArm P1");

    // 右上角绿色对勾
    tft.fillCircle(tft.width() - 20, 20, 10, TFT_GREEN);

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


    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // 背景黑色，防止重影
    tft.setCursor(10, 90);
    tft.printf("X:        mm");

    tft.setCursor(10, 115);
    tft.printf("Y:        mm");

    tft.setCursor(10, 140);
    tft.printf("Z:        mm");

    tft.setCursor(10, 165);
    tft.printf("Theta:");


    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(rightX, 90);
    tft.printf("J1:         Degrees");
    tft.setCursor(rightX, 115);
    tft.printf("J2:         Degrees");
    tft.setCursor(rightX, 140);
    tft.printf("J3:         Degrees");
    tft.setCursor(rightX, 165);
    tft.printf("J4:         Degrees");



    // 底部分割线
    tft.drawLine(0, 190, tft.width(), 190, TFT_WHITE);
}


void Home::Draw_Update()
{
    UpdateCoord(0, 0, 0, 0);
    UpdateAngle(0, 0, 0, 0);
    // delay(100);
}


void Home::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN1)
    {
        Program* program = new Program(tft, button);
        button.Wait();
        ui_manager.Change_UI(program, true);
    }


}


void Home::UpdateCoord(float x, float y, float z, float o)
{
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // 背景黑色，防止重影
    tft.setCursor(20, 90);
    tft.printf("%6.1f", x);

    tft.setCursor(20, 115);
    tft.printf("%6.1f", y);

    tft.setCursor(20, 140);
    tft.printf("%6.1f", z);

    tft.setCursor(60, 165);
    tft.printf("%6.1f", o);
    // tft.printf("°");
}


void Home::UpdateAngle(float j1, float j2, float j3, float j4)
{
    int rightX = 170;
    int clearX = 180;
    tft.setFreeFont(&FreeSans9pt7b);

    tft.fillRect(clearX, 70, 10, 30, TFT_BLACK);
    tft.fillRect(clearX, 95, 10, 30, TFT_BLACK);
    tft.fillRect(clearX, 120, 10, 30, TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(rightX, 90);
    tft.printf("%6.1f", j1);
    tft.setCursor(rightX, 115);
    tft.printf("%6.1f", j2);
    tft.setCursor(rightX, 140);
    tft.printf("%6.1f", j3);
    tft.setCursor(rightX, 165);
    tft.printf("%6.1f", j4);
}