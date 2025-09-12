#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//子界面
#include "WIFI_Information.h"


class Connection : public Screen_Base
{
public:
    Connection(TFT_eSPI& tft, Button& button);
    ~Connection();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_Star();

    const char *menuItems[3] = {
        "WLAN",
        "USB",
        "BlueTooth"};
    const int menuCount = 3;

    int selectedIndex = 0; // 当前选中
    int lastIndex = 0;     // 上一次选中

    // 位置
    const int starX = 10;      // 星号起始X,坐标Y沿用lineHeight
    const int textX = 30;      // 所有文字的起始坐标X
    const int startY = 70;     // 所有文字起始的坐标Y
    const int lineHeight = 25; // 标题以外每段文件的间距






};





#endif

