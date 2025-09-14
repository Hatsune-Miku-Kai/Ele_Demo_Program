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
}

void Blockly_Runner::Update_UI()
{
}

bool Blockly_Runner::Load_File()
{
    //需要判断SD卡中是否有文件,目前只实现延时
    return true;
}
