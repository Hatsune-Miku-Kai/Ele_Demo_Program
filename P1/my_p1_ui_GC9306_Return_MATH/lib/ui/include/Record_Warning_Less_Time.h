#ifndef __RECORD_WARNING_LESS_TIME_H__
#define __RECORD_WARNING_LESS_TIME_H__

#include "UI_Manager.h"
#include "Screen_Base.h"


class Record_Warning_Less_Time : public Screen_Base
{
public:
    Record_Warning_Less_Time(TFT_eSPI &tft, Button &button);
    ~Record_Warning_Less_Time();

    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

#ifdef MyCobot_Pro_450
    void Draw_UI();
    void Update_UI();
    void MyCobot_Pro_450_Select();
#endif

};

#endif