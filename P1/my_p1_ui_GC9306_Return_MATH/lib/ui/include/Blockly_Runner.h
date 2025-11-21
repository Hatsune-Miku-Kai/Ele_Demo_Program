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
    void Handle_String_Data(std::vector<uint8_t> &data);
    bool Load_File();

    std::map<std::string, std::pair<int, int>> Blockly_UI = //默认没有文件,当用户录制好点位以后才显示
    {
        {"File1", {10, 75}},
        {"File2", {10, 100}},
        {"File3", {10, 125}},
        {"File4", {10, 150}},
        {"File5", {10, 175}},
    };


    char *menuItems[50];//先暂时没有数据,如果收到了数据,就把他填充进这个数组,最大有50个文件


};



#endif
