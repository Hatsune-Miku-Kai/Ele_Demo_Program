#include "Save.h"

extern UI_Manager ui_manager;

extern const GFXfont FreeSansBold9pt7b;

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
            case 0://RAM
            {

                break;
            }

            case 1://FLASH
            {
                break;
            }
        }
    }

    else if(btn == BTN4)//如果不保存,跳回拖动示教界面
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
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setCursor(starX, startY);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    // 底部分割线
    tft.drawLine(0, 190, tft.width(), 190, TFT_WHITE);
}

void Save::Update_Star()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键
    Serial.println(btn);
    int8_t dir = 0;
    if (btn == BTN1)
        dir = -1; // 上
    else if (btn == BTN2)
        dir = 1; // 下
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
    tft.setCursor(starX, startY + selectedIndex * lineHeight);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    while (button.Get_Button_Status() != 0)
    {
        delay(10);
    }
}



void Save::Save_To_RAM()
{
    tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);
}

