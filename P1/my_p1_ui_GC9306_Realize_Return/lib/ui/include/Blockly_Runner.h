#ifndef __BLOCKLY_RUNNER_H__
#define __BLOCKLY_RUNNER_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

class Blockly_Runner:public Screen_Base
{
public:
    Blockly_Runner(TFT_eSPI &tft, Button &button);
    ~Blockly_Runner();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;




private:
    void Draw_UI();
    void Update_UI();
    bool Load_File();

};



#endif
