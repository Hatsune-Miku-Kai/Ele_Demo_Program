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

    std::map<std::string, std::pair<int, int>> Blockly_UI = //默认没有文件,当用户录制好点位以后才显示
    {
    };


    std::map<std::string, std::pair<int, int>> Start_UI = //星号显示的位置,如果有文件,则显示在文件的位置,默认显示第一个
    {
    };

};



#endif
