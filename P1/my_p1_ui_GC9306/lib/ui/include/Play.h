#ifndef __PLAY_H__
#define __PLAU_H__
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "SetUp.h"


class Play
{

public:
    Play(TFT_eSPI &display);
    ~Play();

    void Draw_UI(); // 绘制菜单和默认星号
    void Update_Star();     // 更新 * 位置
    void Play_UI();

private:
    TFT_eSPI &tft;
    Board board;

    const char *menuItems[2] = {
        "Play from RAM",
        "Play from FLASH"
    };
    const int menuCount = 2;

    int selectedIndex = 0; // 当前选中
    int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;//星号起始X,坐标Y沿用lineHeight
    const int textX = 30;//所有文字的起始坐标X
    const int startY = 70;//所有文字起始的坐标Y
    const int lineHeight = 25;//标题以外每段文件的间距

};


#endif

