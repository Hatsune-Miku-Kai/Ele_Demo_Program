#include "Fine_Tuning_Angles.h"

extern UI_Manager ui_manager;

Fine_Tuning_Angles::Fine_Tuning_Angles(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = Screen_Base::ScreenID::ScreenID_Fine_Tuning_Angles;
}

Fine_Tuning_Angles::~Fine_Tuning_Angles()
{
}

void Fine_Tuning_Angles::Draw_Static()
{
#ifdef UltraArm_P1
    UltraArm_P1_UI();
#endif

#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_UI();
#endif
}

void Fine_Tuning_Angles::Draw_Update()
{
    Handle_Data(Global_Data::Data);
    UpdateAngle_450(Angles_Data[0], Angles_Data[1], Angles_Data[2], Angles_Data[3], Angles_Data[4], Angles_Data[5]);
}

void Fine_Tuning_Angles::Handle_Button()
{
#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_Select();
#endif

#ifdef UltraArm_P1
    UltraArm_P1_Select();
#endif
}

void Fine_Tuning_Angles::MyCobot_Pro_450_UI()
{
    tft.fillScreen(TFT_BLACK);

    std::pair<int, int> Modify_Pos = Angles_UI[ui_manager.Select_Name];
    tft.fillRect(Modify_Pos.first - 30, Modify_Pos.second - 25, 150, 35, TFT_WHITE);

    auto pos = Angles_UI[ui_manager.Select_Name];
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(pos.first - 15, pos.second + 5);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.print("*");

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    if (ui_manager.GetRegisteredScreenByID(Screen_Base::ScreenID::ScreenID_Jog_Select_Move) == nullptr)
    {
        tft.print("Calibration");
        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.setTextColor(TFT_YELLOW);
        tft.setCursor(40, 170);
        tft.print("Fine - tune with -/+ And set");

        tft.setCursor(75, 190);
        tft.print("new Zero position");
    }

    else
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

    tft.pushImage(15, 220, 30, 6, reduce_logo);
    tft.pushImage(105, 210, 30, 30, add_logo);

    if (ui_manager.GetRegisteredScreenByID(Screen_Base::ScreenID::ScreenID_Jog_Select_Move) == nullptr)
        tft.pushImage(190, 208, 30, 30, can_save_logo);

    tft.pushImage(275, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Angles_Sprite();
}

void Fine_Tuning_Angles::Create_Angles_Sprite()
{
    J1sprite.createSprite(55, 20);
    J2sprite.createSprite(55, 20);
    J3sprite.createSprite(55, 20);
    J4sprite.createSprite(55, 20);
    J5sprite.createSprite(55, 20);
    J6sprite.createSprite(55, 20);
}

void Fine_Tuning_Angles::Delete_Angles_Sprite()
{
    J1sprite.deleteSprite();
    J2sprite.deleteSprite();
    J3sprite.deleteSprite();
    J4sprite.deleteSprite();
    J5sprite.deleteSprite();
    J6sprite.deleteSprite();
}

void Fine_Tuning_Angles::MyCobot_Pro_450_Select()
{
    ButtonEvent res = button.Get_Press_Type();

    if (res.type == SHORT_PRESS_TYPE)
    {
        if (res.id == BTN1)
        {
            Set_Jog_Increment_Angle_450(ui_manager.Select_id, -0.1, 10);
        }
        else if (res.id == BTN2)
        {
            Set_Jog_Increment_Angle_450(ui_manager.Select_id, 0.1, 10);
        }
        else if (res.id == BTN3)
        {
            if (ui_manager.GetRegisteredScreenByID(Screen_Base::ScreenID::ScreenID_Angle_Select_Move) != nullptr)
                return;
            button.Wait();
            Set_Calibration_450(ui_manager.Select_id);
        }
    }
    else if (res.type == LONG_PRESS_TYPE)
    {
        if (res.id == BTN1)
        {
            Set_VR_Mode_450(1);
            Move_Up = true;
            Jog_Move_Angle_450(ui_manager.Select_id, 0, 10);
        }

        else if (res.id == BTN2)
        {
            Set_VR_Mode_450(1);
            Move_Up = true;
            Jog_Move_Angle_450(ui_manager.Select_id, 1, 10);
        }
    }
    else if (button.Get_Button_Status() == BTN4)
    {
        button.Wait();
        Delete_Angles_Sprite();
        ui_manager.Go_Back();
    }
    else if (Move_Up && button.Get_Button_Status() == 0)
    {
        Set_VR_Mode_450(0);
        Move_Up = false;
        Program_Stop_450(0);
        // while(Global_Data::Recv_Data_Origin[4] != 0)
        // {
        //     Jog_Stop_450();
        // }
    }
}

void Fine_Tuning_Angles::UpdateAngle_450(float j1, float j2, float j3, float j4, float j5, float j6)
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
        if (i == ui_manager.Select_id - 1)
            angles[i].sprite->fillSprite(TFT_WHITE);
        else
            angles[i].sprite->fillSprite(TFT_BLACK); // 清空 sprite

        angles[i].sprite->setFreeFont(&FreeSansBold9pt7b); // 设置字体

        if (i == ui_manager.Select_id - 1)
            angles[i].sprite->setTextColor(TFT_BLACK);
        else
            angles[i].sprite->setTextColor(TFT_WHITE);

        angles[i].sprite->setTextDatum(TR_DATUM);               // 右对齐
        sprintf(buf, "%5.1f", angles[i].value);                 // 固定宽度 5, 保留 1 位小数
        angles[i].sprite->drawString(String(buf), 50, 0);       // x 坐标用 sprite 宽度右对齐
        angles[i].sprite->pushSprite(angles[i].x, angles[i].y); // 推送到屏幕`1
    }
}

void Fine_Tuning_Angles::Handle_Data(std::vector<uint8_t> &data)
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
