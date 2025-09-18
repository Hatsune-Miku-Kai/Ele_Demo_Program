#ifndef __JOG_MOVE_H__
#define __JOG_MOVE_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

class Jog_Move : public Screen_Base
{
public:
    Jog_Move(TFT_eSPI& tft, Button& button);
    ~Jog_Move();

    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:
    void Draw_UI();
    void Update_UI();
    void UpdateCoord(float x, float y, float z, float o);
    void UpdateAngle(float j1, float j2, float j3, float j4);

    const char *menuItems[8] = {
        "X",
        "Y",
        "Z",
        "R",
        "J1",
        "J2",
        "J3",
        "J4"};
    const int menuCount = 8;

    int selectedIndex = 0; // 当前选中
    // int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;      // 星号起始X,坐标Y沿用lineHeight
    const int textX = 30;      // 所有文字的起始坐标X
    const int startY = 65;     // 所有文字起始的坐标Y
    const int lineHeight = 25; // 标题以外每段文件的间距

};





#endif

