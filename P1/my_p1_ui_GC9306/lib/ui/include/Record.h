#ifndef __RECORD_H__
#define __RECORD_H__    

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "SetUp.h"


class Record
{
public:
    Record(TFT_eSPI &display);
    ~Record();
    void Draw_UI(); //绘制不会改变的元素
    void Update_UI();//时间的显示由millis相减来实现,不通过自定义函数实现
    void Save();

private:
    TFT_eSPI &tft;
    Board board;

    
    const char *menuItems[2] = {
        "Save into RAM",
        "Save into FLASH"
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


