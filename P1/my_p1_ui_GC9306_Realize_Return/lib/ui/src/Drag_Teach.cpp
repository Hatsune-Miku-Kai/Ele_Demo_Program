#include "Drag_Teach.h"

//logo
#include "Confirm_logo.h"
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Return_logo.h"


extern UI_Manager ui_manager;

Drag_Teach::Drag_Teach(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Drag_Teach;
}

Drag_Teach::~Drag_Teach()
{
}

void Drag_Teach::Draw_Static()
{
    Draw_UI();
}

void Drag_Teach::Draw_Update()
{
    Update_Star();
}

void Drag_Teach::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN3)
    {
        switch(selectedIndex)
        {
            case 0://Record
            {
                Record* record = new Record(tft, button);
                ui_manager.RegisterScreen(record);//将页面放入注册列表
                button.Wait();
                ui_manager.Change_UI(record, true);
                break;
            }

            case 1://Play
            {
                Play* play = new Play(tft, button);
                ui_manager.RegisterScreen(play);//将页面放入注册列表
                button.Wait();
                ui_manager.Change_UI(play, true);
                break;
            }
        }
    }

    if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}


void Drag_Teach::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);//标题起始坐标
    tft.print("DragTeach");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

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
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(starX, startY + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    tft.pushImage(15, 205, 24, 30, down_select_logo);
    tft.pushImage(105, 205, 25, 30, up_select_logo);
    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}


void Drag_Teach::Update_Star()
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
    tft.setCursor(starX, startY + selectedIndex * lineHeight + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    button.Wait();
}