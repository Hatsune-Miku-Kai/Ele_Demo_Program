#ifndef WLAN_PROGRAM_H
#define WLAN_PROGRAM_H

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "WLAN_Information.h"

class WLAN_Program : public Screen_Base
{
public:
    WLAN_Program(TFT_eSPI& tft, Button& button);
    ~WLAN_Program();
    void Draw_Static();
    void Draw_Update();
    void Handle_Button();


private:
    void Draw_UI();
    void Update_UI();
    void WLAN_Start();
    void Draw_Gray_Program();
    void Draw_White_Program();
    void Update_Gray_Program();
    void Update_White_Program();

    const char *menuItems[4] = {
        "Switch",
        "Avaliable",
        "History",
        "Properties"
        };
    const int menuCount = 4;

    int selectedIndex = 0; // 当前选中
    // int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;//星号起始X,坐标Y沿用lineHeight
    const int textX = 30;//所有文字的起始坐标X
    const int startY = 70;//所有文字起始的坐标Y
    const int lineHeight = 25;//标题以外每段文件的间距
};



#endif
