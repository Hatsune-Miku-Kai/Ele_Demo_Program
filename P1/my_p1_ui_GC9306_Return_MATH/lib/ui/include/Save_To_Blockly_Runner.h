#ifndef __SAVE_TO_BLOCKLY_RUNNER_H__
#define __SAVE_TO_BLOCKLY_RUNNER_H__

#include "Screen_Base.h"
#include "UI_Manager.h"


class Save_To_Blockly_Runner:public Screen_Base
{
public:
    Save_To_Blockly_Runner(TFT_eSPI &tft, Button &button);
    ~Save_To_Blockly_Runner();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:
    void Draw_UI();
    void Update_UI();
    void Save_To_Blockly();

};



#endif

