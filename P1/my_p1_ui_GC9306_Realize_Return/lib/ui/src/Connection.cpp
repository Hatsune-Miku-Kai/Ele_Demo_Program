#include "Connection.h"


Connection::Connection(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Connection;
}

Connection::~Connection()
{
}

void Connection::Draw_Static()
{
    Draw_UI();
}

void Connection::Draw_Update()
{
    Update_Star();
}

void Connection::Handle_Button()
{
    if (button.Get_Button_Status() == BTN3)
    {
        switch (selectedIndex)
        {
        case 0://WLAN
        {

            break;
        }

        case 1://USB
        {

            break;
        }

        case 2://BlueTooth
        {

            break;
        }

        }
    }

}


void Connection::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 40);//标题起始坐标
    tft.print("Connection");

    // 顶部横线
    tft.drawLine(0, 50, tft.width(), 50, TFT_WHITE);

    // 菜单文字 9pt
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;
    for (int i = 0; i < menuCount; i++)
    {
        tft.setCursor(textX, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    // 默认星号 9pt
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setCursor(starX, startY);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}



void Connection::Update_Star()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键

    int8_t dir = 0;
    if (btn == BTN1)
        dir = 1; // 下
    else if (btn == BTN2)
        dir = -1;  // 上
    else
        return;   // 没按键直接返回

    // 擦除旧的 *
    tft.fillRect(0, startY + (selectedIndex - 1) * lineHeight, textX, lineHeight, TFT_BLACK);
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 更新索引
    selectedIndex += dir;
    if (selectedIndex < 0)
        selectedIndex = menuCount - 1;
    if (selectedIndex >= menuCount)
        selectedIndex = 0;

    // 绘制新的 *
    tft.setCursor(starX, startY + selectedIndex * lineHeight);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    button.Wait();
}