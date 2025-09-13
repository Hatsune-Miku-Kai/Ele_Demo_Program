#ifndef SAVE_H
#define SAVE_H


#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Drag_Teach.h"

class Save : public Screen_Base
{
public:
    Save(TFT_eSPI &tft, Button &button);
    ~Save();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:

    void Draw_UI();
    void Update_Star();
    void Save_To_RAM();
    void Save_To_FLASH();

    const char *menuItems[2] = {
        "Save to RAM",
        "Save to FLASH"};
    const int menuCount = 2;

    int selectedIndex = 0; // 当前选中
    int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;      // 星号起始X,坐标Y沿用lineHeight
    const int textX = 30;      // 所有文字的起始坐标X
    const int startY = 70;     // 所有文字起始的坐标Y
    const int lineHeight = 25; // 标题以外每段文件的间距

};

#endif

