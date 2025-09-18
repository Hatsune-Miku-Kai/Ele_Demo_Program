#ifndef CONNECT_H
#define CONNECT_H

#include <TFT_eSPI.h>
#include "SetUp.h"

class Connect
{
public:
    Connect(TFT_eSPI &display);  // 使用引用
    ~Connect();

    void Draw_UI();                               // 绘制完整UI（静态+默认数值）

private:
    TFT_eSPI &tft; // 引用显示对象

    
};




#endif
