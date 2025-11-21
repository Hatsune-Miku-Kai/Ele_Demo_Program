#include "Fine_Tuning_Coords.h"

extern UI_Manager ui_manager;

Fine_Tuning_Coords::Fine_Tuning_Coords(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Fine_Tuning_Coords;
}

Fine_Tuning_Coords::~Fine_Tuning_Coords()
{
}

void Fine_Tuning_Coords::Draw_Static()
{
#ifdef UltraArm_P1
    UltraArm_P1_UI();
#endif

#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_UI();
#endif
}

void Fine_Tuning_Coords::Draw_Update()
{
    Handle_Data(Global_Data::Data);
    UpdateCoords_450(Coords_Data[0], Coords_Data[1], Coords_Data[2], Coords_Data[3], Coords_Data[4], Coords_Data[5]);
}

void Fine_Tuning_Coords::Handle_Button()
{
#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_Select();
#endif

#ifdef UltraArm_P1
    UltraArm_P1_Select();
#endif
}

void Fine_Tuning_Coords::MyCobot_Pro_450_UI()
{
    tft.fillScreen(TFT_BLACK);

    std::pair<int, int> Modify_Pos = Coord_UI[ui_manager.Select_Name];
    tft.fillRect(Modify_Pos.first - 30, Modify_Pos.second - 25, 150, 35, TFT_WHITE);

    auto pos = Coord_UI[ui_manager.Select_Name];
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(pos.first - 15, pos.second + 5);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.print("*");

    // 标题
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    if (ui_manager.GetRegisteredScreenByID(Screen_Base::ScreenID::ScreenID_Jog_Select_Move) == nullptr)
        tft.print("Calibration");
    else
        tft.print("Coordinate");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    std::map<std::string, std::pair<int, int>>::iterator it;

    for (it = Coord_UI.begin(); it != Coord_UI.end(); it++)
    {
        const std::string &name = it->first;
        const std::pair<int, int> &pos = it->second;
        std::pair<int, int> oPos = Coord_Signal[name];

        if (name == "X" || name == "Y" || name == "Z")
        {
            tft.setFreeFont(&FreeSansBold9pt7b);
            if (name == ui_manager.Select_Name)
            {
                tft.setTextColor(TFT_BLACK);
            }
            else
                tft.setTextColor(TFT_WHITE);

            tft.setCursor(oPos.first, oPos.second);
            tft.printf("mm");

            tft.setCursor(pos.first, pos.second);
            tft.printf("%s:", name.c_str());
        }

        else if (name == "Rx" || name == "Ry" || name == "Rz")
        {
            tft.setFreeFont(&FreeSansBold9pt7b);
            if (name == ui_manager.Select_Name)
            {
                tft.setTextColor(TFT_BLACK);
            }
            else
                tft.setTextColor(TFT_WHITE);

            tft.setCursor(oPos.first, oPos.second);
            tft.printf("o");

            tft.setCursor(pos.first, pos.second);
            tft.printf("%s:", name.c_str());
        }
    }

    tft.pushImage(15, 220, 30, 6, reduce_logo);
    tft.pushImage(105, 210, 30, 30, add_logo);

    if (ui_manager.GetRegisteredScreenByID(Screen_Base::ScreenID::ScreenID_Jog_Select_Move) == nullptr)
        tft.pushImage(190, 208, 30, 30, can_save_logo);

    tft.pushImage(275, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Coords_Sprite();
}

void Fine_Tuning_Coords::Create_Coords_Sprite()
{
    Xsprite.createSprite(55, 20);
    Ysprite.createSprite(55, 20);
    Zsprite.createSprite(55, 20);
    Rxsprite.createSprite(55, 20);
    Rysprite.createSprite(55, 20);
    Rzsprite.createSprite(55, 20);
}

void Fine_Tuning_Coords::Delete_Coords_Sprite()
{
    Xsprite.deleteSprite();
    Ysprite.deleteSprite();
    Zsprite.deleteSprite();
    Rxsprite.deleteSprite();
    Rysprite.deleteSprite();
    Rzsprite.deleteSprite();
}

void Fine_Tuning_Coords::MyCobot_Pro_450_Select()
{
    ButtonEvent res = button.Get_Press_Type();

    if (res.type == SHORT_PRESS_TYPE)
    {
        if (res.id == BTN1)
        {
            Set_Jog_Increment_Coord_450(ui_manager.Select_id, -0.1, 10);
        }
        else if (res.id == BTN2)
        {
            Set_Jog_Increment_Coord_450(ui_manager.Select_id, 0.1, 10);
        }
    }
    else if (res.type == LONG_PRESS_TYPE)
    {
        if (res.id == BTN1)
        {
            Set_VR_Mode_450(1);
            Move_Up = true;
            Jog_Move_Coord_450(ui_manager.Select_id, 0, 10);
        }

        else if (res.id == BTN2)
        {
            Set_VR_Mode_450(1);
            Move_Up = true;
            Jog_Move_Coord_450(ui_manager.Select_id, 1, 10);
        }
    }
    else if (button.Get_Button_Status() == BTN4)
    {
        button.Wait();
        Delete_Coords_Sprite();
        ui_manager.Go_Back();
    }
    else if (Move_Up && button.Get_Button_Status() == 0)
    {
        Set_VR_Mode_450(0);
        Move_Up = false;
        Program_Stop_450(0);
        // while(Global_Data::Recv_Data_Origin[4] != 0)
        // {
        //     Program_Stop_450(1);
        // }
    }
}

void Fine_Tuning_Coords::UpdateCoords_450(float x, float y, float z, float rx, float ry, float rz)
{
    int leftX = 27 + 20;   // 这里+号表示修改过,去掉+恢复原来的布局
    int rightX = 177 + 30; // 这里+号表示修改过,去掉+恢复原来的布局
    int showY = 62;

    CoordSprite Coords[6] = {
        {&Xsprite, x, leftX, showY},
        {&Ysprite, y, leftX, showY + 35},
        {&Zsprite, z, leftX, showY + 70},
        {&Rxsprite, rx, rightX, showY},
        {&Rysprite, ry, rightX, showY + 35},
        {&Rzsprite, rz, rightX, showY + 70}};

    char buf[6]; // 用于存格式化数字

    for (int j = 0; j < 3; j++)
    {
        if (j == ui_manager.Select_id - 1)
            Coords[j].sprite->fillSprite(TFT_WHITE); // 清空 sprite
        else
            Coords[j].sprite->fillSprite(TFT_BLACK);

        Coords[j].sprite->setFreeFont(&FreeSansBold9pt7b); // 设置字体

        if (j == ui_manager.Select_id - 1)
            Coords[j].sprite->setTextColor(TFT_BLACK);
        else
            Coords[j].sprite->setTextColor(TFT_WHITE);

        Coords[j].sprite->setTextDatum(TR_DATUM); // 右对齐

        sprintf(buf, "%5.1f", Coords[j].value);                 // 固定宽度 5, 保留 1 位小数
        Coords[j].sprite->drawString(String(buf), 55, 0);       // x 坐标用 sprite 宽度右对齐
        Coords[j].sprite->pushSprite(Coords[j].x, Coords[j].y); // 推送到屏幕
    }

    for (int i = 3; i < 6; i++)
    {
        if (i == ui_manager.Select_id - 1)
            Coords[i].sprite->fillSprite(TFT_WHITE); // 清空 sprite
        else
            Coords[i].sprite->fillSprite(TFT_BLACK);

        Coords[i].sprite->setFreeFont(&FreeSansBold9pt7b); // 设置字体

        if (i == ui_manager.Select_id - 1)
            Coords[i].sprite->setTextColor(TFT_BLACK);
        else
            Coords[i].sprite->setTextColor(TFT_WHITE);
        Coords[i].sprite->setTextDatum(TR_DATUM); // 右对齐

        sprintf(buf, "%5.1f", Coords[i].value);                 // 固定宽度 5, 保留 1 位小数
        Coords[i].sprite->drawString(String(buf), 55, 0);       // x 坐标用 sprite 宽度右对齐
        Coords[i].sprite->pushSprite(Coords[i].x, Coords[i].y); // 推送到屏幕
    }
}

void Fine_Tuning_Coords::Handle_Data(std::vector<uint8_t> &data)
{
    int idx = 0;
    for(uint8_t i = COORDS_DATA_START; i < COORDS_DATA_START + XYZ_DATA_LEN* 2; i += 2)
    {
        float temp = (float)(data[i] << 8 | data[i + 1]);

        if(temp > 33000)
            temp -= 65536;
        temp /= 10.0f;

        Coords_Data[idx++] = temp;
    }

    uint8_t len = COORDS_DATA_START + XYZ_DATA_LEN* 2;

    for(uint8_t i = len; i < len + RXRYRZ_DATA_LEN* 2; i += 2)
    {
        float temp = (float)(data[i] << 8 | data[i + 1]);
        
        if(temp > 33000)
            temp -= 65536;
        temp /= 100.0f;

        Coords_Data[idx++] = temp;
    }
}


