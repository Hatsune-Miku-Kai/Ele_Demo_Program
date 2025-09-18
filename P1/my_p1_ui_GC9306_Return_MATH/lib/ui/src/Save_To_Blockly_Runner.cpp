#include "Save_To_Blockly_Runner.h"

//logo
#include "Can_Save_logo.h"
#include "Return_logo.h"

extern UI_Manager ui_manager;

Save_To_Blockly_Runner::Save_To_Blockly_Runner(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
}

Save_To_Blockly_Runner::~Save_To_Blockly_Runner()
{
}

void Save_To_Blockly_Runner::Draw_Static()
{
    Draw_UI();
}

void Save_To_Blockly_Runner::Draw_Update()
{
    Update_UI();
}



void Save_To_Blockly_Runner::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    if (btn == BTN3)
    {
        Save_To_Blockly();
        delay(3000);
        ui_manager.Go_To(Screen_Base::ScreenID_Play);
    }

    else if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_To(Screen_Base::ScreenID_Play);
    }
}




void Save_To_Blockly_Runner::Draw_UI()
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

    tft.setCursor(110, 115);
    tft.print("Save Into");

    tft.setCursor(90, 140);
    tft.print("BlocklyRunner?");


    tft.pushImage(190, 208, 30, 30, can_save_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Save_To_Blockly_Runner::Update_UI()
{
}

void Save_To_Blockly_Runner::Save_To_Blockly()
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