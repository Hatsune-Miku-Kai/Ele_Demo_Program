#ifndef __JOG_SELECT_MOVE_H__
#define __JOG_SELECT_MOVE_H__

#include "robotGeometry.hpp"

#include "Screen_Base.h"
#include "UI_Manager.h"

class Jog_Select_Move : public Screen_Base
{
public:
    Jog_Select_Move(TFT_eSPI& tft, Button& button);
    ~Jog_Select_Move();

    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:
    RobotGeometry ultraArmP1;
    void Draw_UI();
    void Update_UI();
    void UpdateCoord(float x, float y, float z, float o, uint16_t colour);
    void UpdateAngle(float j1, float j2, float j3, float j4, uint16_t colour);
    void Handle_Select_P1(bool IsHighlight);
    void Handle_Select_450(bool IsHighlight);
    void HighlightItem(int index, bool highlight);

    const char *menuItems[8] = {
        "X:             mm",
        "Y:             mm",
        "Z:             mm",
        "R:",
        "J1:         ",
        "J2:         ",
        "J3:         ",
        "J4:         "};
    const int menuCount = 8;

    bool itemSelected[8] = {false};   // 每项的颜色反转状态，初始全 false,表示初始底色均为黑色,文字为白色

    int selectedIndex = 0; // 当前选中

    std::map<std::string, std::pair<int, int>> starPos = {
        {"X:             mm",       {10, 105}},
        {"Y:             mm",       {10, 130}},
        {"Z:             mm",       {10, 155}},
        {"R:",                      {10, 180}},
        {"J1:         ",            {160, 105}},
        {"J2:         ",            {160, 130}},
        {"J3:         ",            {160, 155}},
        {"J4:         ",            {160, 180}}};
    
    // int lastIndex = 0;     // 上一次选中

    const int textX = 30;      // 所有文字的起始坐标X
    const int startY = 100;     // 所有文字起始的坐标Y
    const int lineHeight = 25; // 标题以外每段文件的间距


};





#endif

