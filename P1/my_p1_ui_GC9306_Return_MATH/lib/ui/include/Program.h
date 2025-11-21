#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

// 关联界面
#include "Drag_Teach.h"
#include "Blockly_Runner.h"
#include "Quick_Move.h"
#include "Connection.h"
#include "Firmware.h"
#include "Calibration.h"

class Program : public Screen_Base
{
public:
    Program(TFT_eSPI &tft, Button &button);

    ~Program();

    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_UI();
    void UltraArm_P1_UI();
    void MyCobot_Pro_450_UI();

#ifdef UltraArm_P1

    const char *menuItems[6] = {
        "DragTeach",
        "BlocklyRunner",
        "QuickMove",
        "Connection",
        "Firmware",
        "Calibration"};
    const int menuCount = 6;

    std::map<std::string, std::pair<int, int>> starPos = {
        {"DragTeach",       {10, 75}},
        {"BlocklyRunner",   {10, 100}},
        {"QuickMove",       {10, 125}},
        {"Connection",      {10, 150}},
        {"Firmware",        {10, 175}},
        {"Calibration",     {10, 200}}};

#endif

#ifdef MyCobot_Pro_450

    const char *menuItems[5] = {
        "DragTeach",
        "BlocklyRunner",
        "QuickMove",
        "Firmware",
        "Calibration"};
    const int menuCount = 5;

    std::map<std::string, std::pair<int, int>> starPos = {
        {"DragTeach",       {10, 75}},
        {"BlocklyRunner",   {10, 100}},
        {"QuickMove",       {10, 125}},
        {"Firmware",        {10, 150}},
        {"Calibration",     {10, 175}}};

#endif

    int selectedIndex = 0; // 当前选中

    // 位置
    const int starX = 10;      // 星号起始X,坐标Y沿用lineHeight
    const int textX = 30;      // 所有文字的起始坐标X
    const int startY = 63;     // 所有文字起始的坐标Y
    const int lineHeight = 25; // 标题以外每段文件的间距
};

#endif
