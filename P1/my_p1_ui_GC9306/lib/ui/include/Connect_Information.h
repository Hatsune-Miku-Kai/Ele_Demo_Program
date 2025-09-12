#ifndef __CONNECT_INFORMATION_H__
#define __CONNECT_INFORMATION_H__

#include <Arduino.h>
#include "TFT_eSPI.h"
#include "SetUp.h"


class Connect_Information
{
public:
    Connect_Information(TFT_eSPI &display);
    ~Connect_Information();
    void Draw_UI();
    void Update_Information();

private:
    TFT_eSPI &tft;
    Board board;
};



#endif

