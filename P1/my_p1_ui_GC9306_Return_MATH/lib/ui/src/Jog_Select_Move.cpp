#include "Jog_Select_Move.h"
#include "SPI.hpp"

// logo
//  #include "Add_logo.h"
//  #include "Reduce_logo.h"
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Return_logo.h"
#include "Confirm_logo.h"

extern UI_Manager ui_manager;

extern uint8_t Send_Buffer[20];
extern uint8_t Recv_Buffer[20];
extern float parsed[6];
extern float J1;
extern float J2;
extern float J3;

bool selected = false;

Jog_Select_Move::Jog_Select_Move(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Jog_Select_Move;
}

Jog_Select_Move::~Jog_Select_Move()
{
}

void Jog_Select_Move::Draw_Static()
{
    Draw_UI();
}

void Jog_Select_Move::Draw_Update()
{
    Update_UI();
    static long time = millis();
    if (millis() - time > 150)
    {
        // SendArray(Send_Buffer, Recv_Buffer);
        ultraArmP1.rot = (J1) / 180.0 * PI;       // 1.75;-0.175, 0.060, 1.569
        ultraArmP1.low = J2 / 180.0 * PI;         // 0.87;
        ultraArmP1.high = (J3 + 90) / 180.0 * PI; // 2.44;
        ultraArmP1.end = 0;
        ultraArmP1.calculateCoords();
        time = millis();

        UpdateAngle(J1, J2, J3, 0, TFT_BLACK);
        UpdateCoord(ultraArmP1.xmm, ultraArmP1.ymm, ultraArmP1.zmm, ultraArmP1.emm, TFT_BLACK);
    }
}

void Jog_Select_Move::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    if (btn == BTN3)
    {
        if (!selected)
        {
            button.Wait();
            Handle_Select_P1(true);
            selected = true;
        }

        else if (selected)
        {
            button.Wait();
            Handle_Select_P1(false);
            selected = false;
        }
    }

    else if (btn == BTN4)
    {
        if (selected)
        {
            button.Wait();
        }

        else if (!selected)
        {
            button.Wait();
            ui_manager.Go_Back();
        }
    }
}

void Jog_Select_Move::Update_UI()
{
    if (selected)
        return;
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键
    int8_t dir = 0;
    if (btn == BTN1)
        dir = 1; // 下
    else if (btn == BTN2)
        dir = -1; // 上
    else
        return; // 没按键直接返回

    {
        tft.setFreeFont(&FreeSansBold12pt7b);
        auto pos = starPos[menuItems[selectedIndex]]; // 根据旧 index 取坐标
        int sx = pos.first;
        int sy = pos.second;
        tft.fillRect(sx, sy - 25, 15, 20, TFT_BLACK); // 擦除星号区域
    }

    selectedIndex += dir;
    if (selectedIndex < 0)
        selectedIndex = menuCount - 1;

    if (selectedIndex >= menuCount)
        selectedIndex = 0;

    {
        auto pos = starPos[menuItems[selectedIndex]];
        int sx = pos.first;
        int sy = pos.second;
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(sx, sy);
        tft.print("*");
    }

    button.Wait();
}

void Jog_Select_Move::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    selectedIndex = 0;

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("JogMove");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 左列 Coord
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(65, 75);
    tft.print("Coord");

    // 右列 Angle
    int rightX = 150;
    tft.setCursor(rightX + 55, 75);
    tft.print("Angle");

    // 符号
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setCursor(80 + 30, 170);
    tft.printf("o");

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(230 + 40, 95);
    tft.printf("o");

    tft.setCursor(230 + 40, 120);
    tft.printf("o");

    tft.setCursor(230 + 40, 145);
    tft.printf("o");

    tft.setCursor(230 + 40, 170);
    tft.printf("o");

    // 菜单文字 9pt
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;
    for (int i = 0; i < menuCount - 4; i++)
    {
        tft.setCursor(textX, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    y = startY;
    for (int i = menuCount - 4; i < menuCount; i++)
    {
        tft.setCursor(textX + 150, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    // 默认星号 9pt
    auto pos = starPos[menuItems[selectedIndex]];
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(pos.first, pos.second);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    // tft.pushImage(15, 220, 30, 6, reduce_logo);
    // tft.pushImage(105, 205, 30, 30, add_logo);
    tft.pushImage(15, 205, 24, 30, down_select_logo);
    tft.pushImage(105, 205, 25, 30, up_select_logo);
    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Jog_Select_Move::UpdateCoord(float x, float y, float z, float o, uint16_t colour) // 默认是黑色
{
    int start_X = 25 + 30;
    tft.setFreeFont(&FreeSansBold9pt7b);

    tft.fillRect(start_X, 85, 50, 18, itemSelected[0] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[0] ? colour : ~colour);                  // 如果此时选中,则颜色反相
    tft.setCursor(start_X, 100);
    tft.printf("%6.1f", x);

    tft.fillRect(start_X, 110, 50, 18, itemSelected[1] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[1] ? colour : ~colour);                   // 如果此时选中,则颜色反相
    tft.setCursor(start_X, 125);
    tft.printf("%6.1f", y);

    tft.fillRect(start_X, 135, 50, 18, itemSelected[2] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[2] ? colour : ~colour);                   // 如果此时选中,则颜色反相
    tft.setCursor(start_X, 150);
    tft.printf("%6.1f", z);


    tft.fillRect(start_X, 160, 50, 18, itemSelected[3] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[3] ? colour : ~colour);                   // 如果此时选中,则颜色反相
    tft.setCursor(start_X, 175);
    tft.printf("%6.1f", o);
    tft.setCursor(80 + 30, 170);
    tft.printf("o");

}

void Jog_Select_Move::UpdateAngle(float j1, float j2, float j3, float j4, uint16_t colour) // 默认是黑色
{
    int rightX = 175 + 35;
    int clearX = 180 + 35;
    tft.setFreeFont(&FreeSansBold9pt7b);

    tft.fillRect(rightX, 85, 50, 20, itemSelected[4] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[4] ? colour : ~colour);                 // 如果此时选中,则颜色反相
    tft.setCursor(rightX, 100);
    tft.printf("%6.1f", j1);
    tft.setCursor(230 + 40, 95);
    tft.printf("o");

    tft.fillRect(rightX, 110, 50, 20, itemSelected[5] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[5] ? colour : ~colour);                  // 如果此时选中,则颜色反相
    tft.setCursor(rightX, 125);
    tft.printf("%6.1f", j2);
    tft.setCursor(230 + 40, 120);
    tft.printf("o");

    tft.fillRect(rightX, 135, 50, 20, itemSelected[6] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[6] ? colour : ~colour);                  // 如果此时选中,则颜色反相
    tft.setCursor(rightX, 150);
    tft.printf("%6.1f", j3);
    tft.setCursor(230 + 40, 145);
    tft.printf("o");

    tft.fillRect(rightX, 160, 50, 20, itemSelected[7] ? ~colour : colour); // 如果此时选中,则背景颜色反相为白色
    tft.setTextColor(itemSelected[7] ? colour : ~colour);                  // 如果此时选中,则颜色反相
    tft.setCursor(rightX, 175);
    tft.printf("%6.1f", j4);
    tft.setCursor(230 + 40, 170);
    tft.printf("o");
}

void Jog_Select_Move::Handle_Select_P1(bool IsHighlight)
{
    int clearX = 150;
    int clearY = 20;
    switch (selectedIndex)
    {
    case 0: // X
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);

        break;
    }

    case 1: // Y
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }

    case 2: // Z
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }

    case 3: // R
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }

    case 4: // J1
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }

    case 5: // J2
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }

    case 6: // J3
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }

    case 7: // J4
    {
        if (IsHighlight)
            HighlightItem(selectedIndex, true);
        else
            HighlightItem(selectedIndex, false);
        break;
    }
    }
}

void Jog_Select_Move::HighlightItem(int index, bool highlight)
{
    int clearX = 150;
    int clearY = 20;
    if (highlight)
    {
        itemSelected[index] = highlight;

        // 根据菜单名取坐标
        auto pos = starPos[menuItems[index]];
        int sx = pos.first;
        int sy = pos.second;

        // 擦除背景
        tft.fillRect(sx - 10, sy - 20, clearX, clearY, TFT_WHITE);

        // 重新绘制文字（黑字白底）
        tft.setCursor(sx + 20, sy - 5);
        tft.setTextColor(TFT_BLACK);
        tft.print(menuItems[index]); //打印当前项

        // 绘制星号
        tft.setFreeFont(&FreeSansBold12pt7b);
        tft.setCursor(sx, sy);
        tft.print("*");
        tft.setFreeFont(&FreeSansBold9pt7b);
    }

    else
    {
        itemSelected[index] = highlight;

        // 根据菜单名取坐标
        auto pos = starPos[menuItems[index]];
        int sx = pos.first;
        int sy = pos.second;

        // 擦除背景
        tft.fillRect(sx - 10, sy - 20, clearX, clearY, TFT_BLACK);

        // 重新绘制文字（白字黑底）
        tft.setCursor(sx + 20, sy - 5);
        tft.setTextColor(TFT_WHITE);
        tft.print(menuItems[index]); //打印当前项

        // 绘制星号
        tft.setFreeFont(&FreeSansBold12pt7b);
        tft.setCursor(sx, sy);
        tft.print("*");
        tft.setFreeFont(&FreeSansBold9pt7b);
    }
}