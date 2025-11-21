#include "Blockly_Runner.h"

//logo
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Confirm_logo.h"
#include "Return_logo.h"

extern UI_Manager ui_manager;

Blockly_Runner::Blockly_Runner(TFT_eSPI &tft, Button &button):Screen_Base(tft, button)
{
}


Blockly_Runner::~Blockly_Runner()
{
}



void Blockly_Runner::Draw_Static()
{
    Draw_UI();
}


void Blockly_Runner::Draw_Update()
{
    Update_UI();
}


void Blockly_Runner::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    if(btn == BTN1)
    {
        button.Wait();
    }

    else if(btn == BTN2)
    {
        button.Wait();
    }

    else if(btn == BTN3)
    {
        button.Wait();
    }

    else if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}


void Blockly_Runner::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 Blockly Runner (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("Blockly Runner");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.pushImage(15, 205, 24, 30, down_select_logo);
    tft.pushImage(105, 205, 25, 30, up_select_logo);
    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.drawString("Loading file...", 100, 120);
    vTaskDelay(2000);
    tft.fillRect(50, 100, tft.width(), 80, TFT_BLACK);

    std::map<std::string, std::pair<int, int>>::iterator it;

if(!Blockly_UI.empty())
{
    for(it = Blockly_UI.begin(); it != Blockly_UI.end(); it++)
    {
        const std::string& key = it->first;
        const std::pair<int, int>& pos = it->second;

        tft.setCursor(pos.first, pos.second);
        tft.print(key.c_str());
    }

    //绘制星号
    it = Blockly_UI.begin();
    const std::pair<int, int>& pos = it->second;

    tft.setCursor(pos.first, pos.second);
    tft.print("*");
}

else
{
    tft.drawString("No Blockly Program", 70, 120);
}


    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Blockly_Runner::Update_UI()
{
    
}

bool Blockly_Runner::Load_File()
{
    //需要判断SD卡中是否有文件,目前只实现延时
    return true;
}



void Blockly_Runner::Handle_String_Data(std::vector<uint8_t> &data)
{
    uint8_t len = data[1];
    for(int i = 2; i < len; i++)
    {
        if(data[i] != 0x0A && data[i+1] != 0x0D)
        {
            
        }
    }
}
