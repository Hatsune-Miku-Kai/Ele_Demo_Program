#ifndef __AUTOMATIC_CALIBRATION_H__
#define __AUTOMATIC_CALIBRATION_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Calibration_Joint.h"

class Automatic_Calibration : public Screen_Base
{
    friend class Calibration_Joint;//让Calibration_Joint可以访问Automatic_Calibration的私有成员变量selectedIndex从而知道当前需要校准的是哪个关节
public:
    Automatic_Calibration(TFT_eSPI& tft, Button& button);
    ~Automatic_Calibration();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_UI();

    const char *menuItems[3] = {
        "J1:",
        "J2:",
        "J3:"};
    const int menuCount = 3;

    int selectedIndex = 0; // 当前选中
    // int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;      // 星号起始X,坐标Y沿用lineHeight
    const int textX = 30;      // 所有文字的起始坐标X
    const int startY = 70;     // 所有文字起始的坐标Y
    const int lineHeight = 25; // 标题以外每段文件的间距
};












#endif
