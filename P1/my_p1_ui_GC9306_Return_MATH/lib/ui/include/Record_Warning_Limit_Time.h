#ifndef __RECORD_WARNING_LIMIT_TIME_H__
#define __RECORD_WARNING_LIMIT_TIME_H__

#include "UI_Manager.h"
#include "Screen_Base.h"

//关联界面
#include "Save.h"
#include "Drag_Teach.h"
#include "Record_Warning_Less_Time.h"

class Record_Warning_Limit_Time : public Screen_Base
{
public:
    Record_Warning_Limit_Time(TFT_eSPI &tft, Button &button);
    ~Record_Warning_Limit_Time();

    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:

#ifdef MyCobot_Pro_450
    void MyCobot_Pro_450_UI();
    void MyCobot_Pro_450_Select();
    void MyCobot_Pro_450_Update();


#endif

};







#endif
