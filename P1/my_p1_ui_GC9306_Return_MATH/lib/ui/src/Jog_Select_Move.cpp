#include "Jog_Select_Move.h"
#include "SPI.hpp"

//logo
// #include "Add_logo.h"
// #include "Reduce_logo.h"
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Return_logo.h"
#include "Confirm_logo.h"

extern UI_Manager ui_manager;

extern uint8_t Send_Buffer[20];
extern uint8_t Recv_Buffer[20];
extern float parsed[6];
extern float J1;
extern float J2;
extern float J3; 

Jog_Select_Move::Jog_Select_Move(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Jog_Select_Move;
}


Jog_Select_Move::~Jog_Select_Move()
{
}


void Jog_Select_Move::Draw_Static()
{
    Draw_UI();
}


void Jog_Select_Move::Draw_Update()
{
    Update_UI();
    static long time = millis();
    if(millis() - time > 150)
    {
        time = millis();
        SendArray(Send_Buffer, Recv_Buffer);

        UpdateAngle(J1 , J2, J3, 0);
        ultraArmP1.rot = (J1) / 180.0 * PI;//1.75;-0.175, 0.060, 1.569
	    ultraArmP1.low = J2 / 180.0 * PI;// 0.87;
	    ultraArmP1.high = (J3 + 90) / 180.0 * PI;// 2.44;
	    ultraArmP1.end = 0;
	    ultraArmP1.calculateCoords();

        UpdateCoord(ultraArmP1.xmm, ultraArmP1.ymm, ultraArmP1.zmm, ultraArmP1.emm);
    }
}

void Jog_Select_Move::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    if (btn == BTN1)
    {

    }


    if(btn == BTN2)
    {
    }

    if (btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}


void Jog_Select_Move::Update_UI()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键
    int8_t dir = 0;
    if (btn == BTN1)
        dir = 1; // 下
    else if (btn == BTN2)
        dir = -1; // 上
    else
        return;   // 没按键直接返回

    // 擦除旧的 *
    tft.fillRect(0, startY + (selectedIndex - 1) * lineHeight, textX, lineHeight, TFT_BLACK);
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 更新索引
    selectedIndex += dir;
    if (selectedIndex < 0)
        selectedIndex = menuCount - 1;
    if (selectedIndex >= menuCount - 4)
        
        selectedIndex = 0;

    // 绘制新的 *
    tft.setCursor(starX, startY + selectedIndex * lineHeight + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    button.Wait();
}


void Jog_Select_Move::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    selectedIndex = 0;

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);//标题起始坐标
    tft.print("JogMove");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);


    // 左列 Coord
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(65, 75);
    tft.print("Coord");

    // 右列 Angle
    int rightX = 150;
    tft.setCursor(rightX + 55, 75);
    tft.print("Angle");

    //符号
    tft.setCursor(80 + 30, 165);
    tft.printf("o");

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(230 + 40, 90);
    tft.printf("o");

    tft.setCursor(230 + 40, 115);
    tft.printf("o");

    tft.setCursor(230 + 40, 140);
    tft.printf("o");

    tft.setCursor(230 + 40, 165);
    tft.printf("o");


    // 菜单文字 9pt
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;
    for (int i = 0; i < menuCount - 4; i++)
    {
        tft.setCursor(textX, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    y = startY;
    for (int i = menuCount - 4; i < menuCount; i++)
    {
        tft.setCursor(textX + 150, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    // 默认星号 9pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(starX, startY + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    
    // tft.pushImage(15, 220, 30, 6, reduce_logo);
    // tft.pushImage(105, 205, 30, 30, add_logo);
    tft.pushImage(15, 205, 24, 30, down_select_logo);
    tft.pushImage(105, 205, 25, 30, up_select_logo);
    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
} 


void Jog_Select_Move::UpdateCoord(float x, float y, float z, float o)
{
    int start_X = 25 + 30;
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // 背景黑色，防止重影

    tft.fillRect(start_X, 80, 50, 25, TFT_BLACK);
    tft.setCursor(start_X, 100);
    tft.printf("%6.1f", x);

    tft.fillRect(start_X, 105, 50, 25, TFT_BLACK);
    tft.setCursor(start_X, 125);
    tft.printf("%6.1f", y);

    tft.fillRect(start_X, 130, 50, 25, TFT_BLACK);
    tft.setCursor(start_X, 150);
    tft.printf("%6.1f", z);

    tft.fillRect(start_X, 155, 50, 25, TFT_BLACK);
    tft.setCursor(start_X, 175);
    tft.printf("%6.1f", o);
}

void Jog_Select_Move::UpdateAngle(float j1, float j2, float j3, float j4)
{
int rightX = 175 + 35;
    int clearX = 180 + 35;
    tft.setFreeFont(&FreeSansBold9pt7b);

    tft.fillRect(clearX, 80, 10, 30, TFT_BLACK);
    tft.fillRect(clearX, 105, 10, 30, TFT_BLACK);
    tft.fillRect(clearX, 125, 10, 30, TFT_BLACK);

    tft.setTextColor(TFT_WHITE);

    tft.fillRect(rightX, 80, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 100);
    tft.printf("%6.1f", j1);
    
    tft.fillRect(rightX, 105, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 125);
    tft.printf("%6.1f", j2);

    tft.fillRect(rightX, 130, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 150);
    tft.printf("%6.1f", j3);

    tft.fillRect(rightX, 155, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 175);
    tft.printf("%6.1f", j4);
}