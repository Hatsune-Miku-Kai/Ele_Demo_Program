#include "Program.h"


Program::Program(TFT_eSPI &display) : tft(display) 
{
    board.Init();
}

Program::~Program()
{
}


// 绘制菜单
void Program::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);//标题起始坐标
    tft.print("Program");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 菜单文字 9pt
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;
    for (int i = 0; i < menuCount; i++)
    {
        tft.setCursor(textX, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    // 默认星号 9pt
    tft.setFreeFont(&FreeSans9pt7b);
    tft.setCursor(starX, startY);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");
}

// 移动 * 指示符
void Program::Update_Star()
{
    uint8_t btn = board.Get_Button_Status(); // 读取当前按键

    int8_t dir = 0;
    if (btn == BTN1)
        dir = -1; // 上
    else if (btn == BTN2)
        dir = 1;  // 下
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

    while (board.Get_Button_Status() != 0)
    {
        delay(10);
    }
}
