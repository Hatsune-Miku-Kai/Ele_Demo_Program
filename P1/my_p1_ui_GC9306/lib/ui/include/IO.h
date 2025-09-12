#ifndef __IO_H_
#define __IO_H_

#include <TFT_eSPI.h>
#include "SetUp.h"
#include "Program_logo.h"
#include "Return_logo.h"
#include "Connect_logo.h"

class IO
{

public:
    IO(TFT_eSPI &display);
    ~IO();

    void Draw_UI();

private:
    TFT_eSPI &tft; // 引用显示对象

};




#endif

