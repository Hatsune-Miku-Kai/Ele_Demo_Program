#include "Play.h"

extern UI_Manager ui_manager;

extern const GFXfont FreeSansBold9pt7b;

Play::Play(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Play;
}

Play::~Play()
{
}

void Play::Draw_Static()
{
    Draw_UI();
}

void Play::Draw_Update()
{
    Update_Star();
}

void Play::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN3)
    {
        switch(selectedIndex)
        {
            case 0://RAM
            {
                Play_From_RAM_OR_FLASH* play_from_ram_or_flash = new Play_From_RAM_OR_FLASH(tft, button);
                button.Wait();
                ui_manager.Change_UI(play_from_ram_or_flash, true);
                break;
            }

            case 1://Flash
            {
                Play_From_RAM_OR_FLASH* play_from_ram_or_flash = new Play_From_RAM_OR_FLASH(tft, button);
                button.Wait();
                ui_manager.Change_UI(play_from_ram_or_flash, true);
                break;
            }
        }
    }

    else if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}

void Play::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Play");

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

void Play::Update_Star()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键
    int8_t dir = 0;
    if (btn == BTN1)
        dir = -1; // 上
    else if (btn == BTN2)
        dir = 1; // 下
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