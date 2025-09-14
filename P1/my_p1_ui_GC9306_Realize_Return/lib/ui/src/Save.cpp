#include "Save.h"

// logo
#include "Confirm_logo.h"
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Return_logo.h"

extern UI_Manager ui_manager;

Save::Save(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Save;
}

Save::~Save()
{
}

void Save::Draw_Static()
{
    Draw_UI();
}

void Save::Draw_Update()
{
    Update_Star();
}

void Save::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN3)
    {
        switch (selectedIndex)
        {
            case 0: // RAM
            {
                Save_To_RAM();
                delay(3000);
                ui_manager.Go_To(Screen_Base::ScreenID_Drag_Teach);
                break;
            }

            case 1: // FLASH
            {
                Save_To_FLASH();
                delay(3000);
                ui_manager.Go_To(Screen_Base::ScreenID_Drag_Teach);
                break;
            }
        }
    }

    else if (btn == BTN4) // 如果不保存,跳回拖动示教界面
    {
        button.Wait();
        ui_manager.Go_To(Screen_Base::ScreenID_Drag_Teach);
    }
}

void Save::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Save");

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

void Save::Update_Star()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键
    Serial.println(btn);
    int8_t dir = 0;
    if (btn == BTN1)
        dir = 1; // 下
    else if (btn == BTN2)
        dir = -1; // 上
    else
        return; // 没按键直接返回

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

    while (button.Get_Button_Status() != 0)
    {
        delay(10);
    }
}

void Save::Save_To_RAM()
{
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.fillRect(0, 50, tft.width(), 80, TFT_BLACK);
    tft.fillRect(0, 95, tft.width(), 80, TFT_BLACK);
    tft.setCursor(110, 125);
    tft.print("Saving...");
    delay(3000);
    tft.fillRect(110, 100, tft.width(), 80, TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(110, 125);
    tft.print("Saved!");
}


void Save::Save_To_FLASH()
{
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.fillRect(0, 50, tft.width(), 80, TFT_BLACK);
    tft.fillRect(0, 95, tft.width(), 80, TFT_BLACK);
    tft.setCursor(110, 125);
    tft.print("Saving...");
    delay(3000);
    tft.fillRect(110, 100, tft.width(), 80, TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(110, 125);
    tft.print("Saved!");
}