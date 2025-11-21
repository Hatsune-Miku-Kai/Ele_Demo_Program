#include "Angle_Select_Move.h"  


extern UI_Manager ui_manager;

//logo
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Confirm_logo.h"
#include "Return_logo.h"

Angle_Select_Move::Angle_Select_Move(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Angle_Select_Move;
}

Angle_Select_Move::~Angle_Select_Move()
{
}

void Angle_Select_Move::Draw_Update()
{
    Handle_Data(Global_Data::Data);
    UpdateAngle_450(Angles_Data[0], Angles_Data[1], Angles_Data[2], Angles_Data[3], Angles_Data[4], Angles_Data[5]);
}
void Angle_Select_Move::Handle_Button()
{
    MyCobot_Pro_450_Select();
}

void Angle_Select_Move::Draw_Static()
{
    MyCobot_Pro_450_UI();
}

void Angle_Select_Move::MyCobot_Pro_450_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("Angle");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    std::map<std::string, std::pair<int, int>>::iterator it;

    // 左列 Angles
    for (it = Angles_UI.begin(); it != Angles_UI.end(); it++)
    {
        const std::string &name = it->first;
        std::pair<int, int> pos = it->second;
        std::pair<int, int> oPos = Angles_Signal[name];

        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(oPos.first, oPos.second);
        tft.printf("o");

        tft.setCursor(pos.first, pos.second);
        tft.printf("%s:", name.c_str());
    }
    
    auto pos = Angles_UI[menuItems[selectedIndex]];
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(pos.first - 15, pos.second + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    tft.pushImage(15, 210, 24, 30, down_select_logo);
    tft.pushImage(105, 210, 25, 30, up_select_logo);
    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(275, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Angles_Sprite();
}


void Angle_Select_Move::Handle_Data(std::vector<uint8_t>& data)
{
    int idx = 0;
    for (uint8_t i = 0; i < (ANGLES_DATA_LEN * 2); i += 2)
    {
        float temp = (float)(data[i] << 8 | data[i + 1]);
        if (temp > 33000)
            temp -= 65536;
        temp /= 100.0f;
        Angles_Data[idx++] = temp;
    }
}

void Angle_Select_Move::UpdateAngle_450(float j1, float j2, float j3, float j4, float j5, float j6)
{
    int leftX = 40 + 20;   // 这里+号表示修改过,去掉+恢复原来的布局
    int rightX = 180 + 30; // 这里+号表示修改过,去掉+恢复原来的布局
    int showY = 62;

    AngleSprite angles[7] = {
        {&J1sprite, j1, leftX, showY},
        {&J2sprite, j2, leftX, showY + 35},
        {&J3sprite, j3, leftX, showY + 70},
        {&J4sprite, j4, rightX, showY},
        {&J5sprite, j5, rightX, showY + 35},
        {&J6sprite, j6, rightX, showY + 70}};

    char buf[6]; // 用于存格式化数字

    for (int i = 0; i < 6; i++)
    {
        angles[i].sprite->fillSprite(TFT_BLACK);           // 清空 sprite
        angles[i].sprite->setFreeFont(&FreeSansBold9pt7b); // 设置字体
        angles[i].sprite->setTextColor(TFT_WHITE);
        angles[i].sprite->setTextDatum(TR_DATUM); // 右对齐

        sprintf(buf, "%5.1f", angles[i].value);                 // 固定宽度 5, 保留 1 位小数
        angles[i].sprite->drawString(String(buf), 50, 0);       // x 坐标用 sprite 宽度右对齐
        angles[i].sprite->pushSprite(angles[i].x, angles[i].y); // 推送到屏幕`1
    }


}


void Angle_Select_Move::MyCobot_Pro_450_Select()
{
uint8_t btn = button.Get_Button_Status(); // 读取当前按键
    int8_t dir = 0;
    if (btn == BTN1)
        dir = 1; // 下
    else if (btn == BTN2)
        dir = -1; // 上

    else if (btn == BTN3)
        {
            button.Wait();

            ui_manager.Select_id = selectedIndex + 1;//传递当前所选的关节位号
            ui_manager.Select_Name = menuItems[selectedIndex];//传递当前所选的关节名称

            Fine_Tuning_Angles * fine_tuning_angles = new Fine_Tuning_Angles(tft, button);
            ui_manager.RegisterScreen(fine_tuning_angles);
            Delete_Angles_Sprite();
            ui_manager.Change_UI(fine_tuning_angles,true);
            return;
        }
    else if (btn == BTN4)
        {
            button.Wait();
            Delete_Angles_Sprite();
            ui_manager.Go_Back();
            return;
        }
    else
        return; // 没按键直接返回

    {
        button.Wait();
        tft.setFreeFont(&FreeSansBold12pt7b);
        auto pos = Angles_UI[menuItems[selectedIndex]]; // 根据旧 index 取坐标
        int sx = pos.first;
        int sy = pos.second;
        tft.fillRect(sx - 15, sy - 15, 15, 20, TFT_BLACK); // 擦除星号区域
    }

    selectedIndex += dir;
    if (selectedIndex < 0)
        selectedIndex = menuCount - 1;

    if (selectedIndex >= menuCount)
        selectedIndex = 0;

    {
        auto pos = Angles_UI[menuItems[selectedIndex]];
        int sx = pos.first;
        int sy = pos.second;
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(sx - 15, sy + 5);
        tft.print("*");
    }
}


void Angle_Select_Move::Create_Angles_Sprite()
{
    J1sprite.createSprite(55, 20);
    J2sprite.createSprite(55, 20);
    J3sprite.createSprite(55, 20);
    J4sprite.createSprite(55, 20);
    J5sprite.createSprite(55, 20);
    J6sprite.createSprite(55, 20);
}

void Angle_Select_Move::Delete_Angles_Sprite()
{
    J1sprite.deleteSprite();
    J2sprite.deleteSprite();
    J3sprite.deleteSprite();
    J4sprite.deleteSprite();
    J5sprite.deleteSprite();
    J6sprite.deleteSprite();
}