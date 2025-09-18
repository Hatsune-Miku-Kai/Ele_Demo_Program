#include "Jog_Move.h"


//logo
#include "Add_logo.h"
#include "Reduce_logo.h"
#include "Return_logo.h"

extern UI_Manager ui_manager;

Jog_Move::Jog_Move(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Jog_Move;
}


Jog_Move::~Jog_Move()
{
}


void Jog_Move::Draw_Static()
{
    Draw_UI();
}


void Jog_Move::Draw_Update()
{

}

void Jog_Move::Handle_Button()
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


void Jog_Move::Update_UI()
{

}


void Jog_Move::Draw_UI()
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
        tft.setCursor(textX + 100, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    // 默认星号 9pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(starX, startY + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    
    tft.pushImage(15, 205, 24, 30, reduce_logo);
    tft.pushImage(105, 205, 25, 30, add_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
} 

