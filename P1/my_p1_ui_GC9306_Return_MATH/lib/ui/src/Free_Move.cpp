#include "Free_Move.h"
#include "SPI.hpp"


//logo
#include "Return_logo.h"
#include "Coord_logo.h"
#include "Angles_logo.h"

extern UI_Manager ui_manager;

extern UI_Manager ui_manager;

extern uint8_t Send_Buffer[20];
extern uint8_t Recv_Buffer[20];
extern float parsed[6];
extern float J1;
extern float J2;
extern float J3; 

Free_Move::Free_Move(TFT_eSPI& tft, Button& button):Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Free_Move;
}


Free_Move::~Free_Move()
{
    // 析构函数
}


void Free_Move::Draw_Static()
{
    Draw_UI();
}



void Free_Move::Draw_Update()
{
#ifdef MyCobot_Pro_450
    Handle_Data(Global_Data::Data);
    UpdateAngle_450(Angles_Data[0], Angles_Data[1], Angles_Data[2], Angles_Data[3], Angles_Data[4], Angles_Data[5]);
#endif

#ifdef UltraArmP1
    UltraArmP1_UI();
#endif
}



void Free_Move::Handle_Button()
{
#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_Select();
#endif

}

void Free_Move::Draw_UI()
{
   MyCobot_Pro_450_UI();
}


#ifdef MyCobot_Pro_450
void Free_Move::MyCobot_Pro_450_Select()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN3)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_Free_Move_Coords)))
        {
            button.Wait();
            Free_Move_Coords *free_move_coords = new Free_Move_Coords(tft, button);
            ui_manager.RegisterScreen(free_move_coords); // 将页面放入注册列表
            Delete_Angles_Sprite();
            ui_manager.Change_UI(free_move_coords, true);
        }

    }

    else if (btn == BTN4)
    {
        button.Wait();
        Delete_Angles_Sprite();
        ui_manager.Go_To(ScreenID::ScreenID_Quick_Move);
    }
}

void Free_Move::Handle_Data(std::vector<uint8_t> &data)
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

void Free_Move::Create_Angles_Sprite()
{
    J1sprite.createSprite(55, 20);
    J2sprite.createSprite(55, 20);
    J3sprite.createSprite(55, 20);
    J4sprite.createSprite(55, 20);
    J5sprite.createSprite(55, 20);
    J6sprite.createSprite(55, 20);
}

void Free_Move::Delete_Angles_Sprite()
{
    J1sprite.deleteSprite();
    J2sprite.deleteSprite();
    J3sprite.deleteSprite();
    J4sprite.deleteSprite();
    J5sprite.deleteSprite();
    J6sprite.deleteSprite();
}


void Free_Move::MyCobot_Pro_450_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("FreeMove");

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

    tft.fillRect(110, 235, 18, 3, TFT_WHITE);

    tft.pushImage(105, 200, 30, 30, angles_logo);
    tft.pushImage(190, 208, 30, 30, coord_logo);
    tft.pushImage(275, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Angles_Sprite();
}


void Free_Move::UpdateAngle_450(float j1, float j2, float j3, float j4, float j5, float j6)
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

#endif