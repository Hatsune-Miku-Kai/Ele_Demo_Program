#ifndef ERROR_H
#define ERROR_H

#include <TFT_eSPI.h>
#include "SetUp.h"

class Error
{
public:
    Error(TFT_eSPI &display);  // 使用引用
    ~Error();

    void Draw_UI(const String& text);                               // 绘制完整UI（静态+默认数值）

private:
    TFT_eSPI &tft; // 引用显示对象

    
};




#endif
