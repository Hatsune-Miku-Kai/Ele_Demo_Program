#ifndef HOME_H
#define HOME_H

#include <TFT_eSPI.h>
#include "SetUp.h"
#include "SPI.hpp"

// #include <Fonts//GFXFF/FreeMonoBold12pt7b.h>//12pt字体
// #include <Fonts//GFXFF/FreeMonoBold12pt7b.h>//9pt字体

class Home
{
public:
    Home(TFT_eSPI &display);  // 使用引用
    ~Home();

    void Draw_UI();                               // 绘制完整UI（静态+默认数值）
    void UpdateCoord(float x, float y, float z, float o); // 更新坐标
    void UpdateAngle(float j1, float j2, float j3, float j4); // 更新角度
    void UpdateAngle(float j1, float j2, float j3);

private:
    TFT_eSPI &tft; // 引用显示对象

    void drawStaticUI(); // 绘制不变的界面元素
};



#endif
