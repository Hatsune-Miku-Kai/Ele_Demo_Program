#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <TFT_eSPI.h>
#include "SetUp.h"
// #include <Fonts//GFXFF/FreeMonoBold12pt7b.h>//12pt字体
// #include <Fonts//GFXFF/FreeMonoBold12pt7b.h>//9pt字体
class Program
{
public:
    Program(TFT_eSPI &display);  // 构造函数传引用
    ~Program();

    void Draw_UI(); // 绘制菜单和默认星号
    void Update_Star();     // 更新 * 位置

private:
    TFT_eSPI &tft; // 引用，不重新创建
    Board board;

    // 菜单项
    const char *menuItems[5] = {
        "DragTeach",
        "BlocklyRunner",
        "QuickMove",
        "Connection",
        "Firmware"};
    const int menuCount = 5;

    int selectedIndex = 0; // 当前选中
    int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;//星号起始X,坐标Y沿用lineHeight
    const int textX = 30;//所有文字的起始坐标X
    const int startY = 70;//所有文字起始的坐标Y
    const int lineHeight = 25;//标题以外每段文件的间距
};



#endif
