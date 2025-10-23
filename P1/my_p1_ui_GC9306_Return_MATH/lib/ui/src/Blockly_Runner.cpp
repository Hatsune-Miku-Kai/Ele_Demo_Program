#include "Blockly_Runner.h"



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
    if(button.Get_Button_Status() == BTN1)
    {
        button.Wait();
        ui_manager.Go_To(Screen_Base::ScreenID_Play);
    }
}


void Blockly_Runner::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("Blockly Runner");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

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
    tft.setCursor(0, 200);
    tft.print("No Blockly Program");
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
