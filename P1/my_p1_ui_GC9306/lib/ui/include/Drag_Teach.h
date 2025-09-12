#ifndef __DRAG_TEACH_H__
#define __DRAG_TEACH_H__

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SetUp.h>

class Drag_Teach
{
public:
    Drag_Teach(TFT_eSPI &tft);
    ~Drag_Teach();


    void Draw_UI();
    void Update_Star();
private:
    TFT_eSPI &tft;
    Board board;
    
    const char *menuItems[2] = {
        "Record",
        "Play"
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

