#include "Free_Move_Coords.h"

extern UI_Manager ui_manager;


//logo
#include "angles_logo.h"
#include "coord_logo.h"
#include "Return_logo.h"

Free_Move_Coords::Free_Move_Coords(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Free_Move_Coords;
}

Free_Move_Coords::~Free_Move_Coords()
{
}

void Free_Move_Coords::Draw_Static()
{
    Draw_UI();
}

void Free_Move_Coords::Draw_Update()
{
    Update_UI();
}

void Free_Move_Coords::Handle_Button()
{
#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_Select();
#endif
}
void Free_Move_Coords::Draw_UI()
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

    for(it = Coord_UI.begin(); it != Coord_UI.end(); it++)
    {
        const std::string& name = it->first;
        const std::pair<int, int>& pos = it->second;
        std::pair<int, int> oPos = Coord_Signal[name];

        if(name == "X" || name == "Y" || name == "Z")
        {
            tft.setFreeFont(&FreeSansBold9pt7b);
            tft.setTextColor(TFT_WHITE);

            tft.setCursor(oPos.first, oPos.second);
            tft.printf("mm");

            tft.setCursor(pos.first, pos.second);
            tft.printf("%s:", name.c_str());
        }

        else if(name == "Rx" || name == "Ry" || name == "Rz")
        {
            tft.setFreeFont(&FreeSansBold9pt7b);
            tft.setTextColor(TFT_WHITE);

            tft.setCursor(oPos.first, oPos.second);
            tft.printf("o");

            tft.setCursor(pos.first, pos.second);
            tft.printf("%s:", name.c_str());
        }
    }

    tft.pushImage(105, 208, 30, 30, angles_logo);
    tft.pushImage(190, 200, 30, 30, coord_logo);
    tft.pushImage(275, 210, 32, 28, return_logo);

    tft.fillRect(195, 235, 18, 3, TFT_WHITE);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Coord_Sprite();

}

void Free_Move_Coords::Update_UI()
{
    Handle_Data(Global_Data::Data);
    UpdateCoords(Coords_Data[0], Coords_Data[1], Coords_Data[2], Coords_Data[3], Coords_Data[4], Coords_Data[5]);
}


void Free_Move_Coords::UpdateCoords(float x, float y, float z, float rx, float ry, float rz)
{
    int leftX = 27 + 20;    //这里+号表示修改过,去掉+恢复原来的布局
    int rightX = 177 + 30;  //这里+号表示修改过,去掉+恢复原来的布局
    int showY = 62;

    AngleSprite Coords[6] = {
        {&Xsprite, x, leftX, showY},
        {&Ysprite, y, leftX, showY + 35},
        {&Zsprite, z, leftX, showY + 70},
        {&Rxsprite, rx, rightX, showY},
        {&Rysprite, ry, rightX, showY + 35},
        {&Rzsprite, rz, rightX, showY + 70}
        };

    char buf[6]; // 用于存格式化数字

    for(int j = 0; j < 3; j++)
    {
        Coords[j].sprite->fillSprite(TFT_BLACK);           // 清空 sprite
        Coords[j].sprite->setFreeFont(&FreeSansBold9pt7b); // 设置字体
        Coords[j].sprite->setTextColor(TFT_WHITE);
        Coords[j].sprite->setTextDatum(TR_DATUM);          // 右对齐

        sprintf(buf, "%5.1f", Coords[j].value);                 // 固定宽度 5, 保留 1 位小数
        Coords[j].sprite->drawString(String(buf), 55, 0);       // x 坐标用 sprite 宽度右对齐
        Coords[j].sprite->pushSprite(Coords[j].x, Coords[j].y); // 推送到屏幕
    }

    for (int i = 3; i < 6; i++)
    {
        Coords[i].sprite->fillSprite(TFT_BLACK);           // 清空 sprite
        Coords[i].sprite->setFreeFont(&FreeSansBold9pt7b); // 设置字体
        Coords[i].sprite->setTextColor(TFT_WHITE);
        Coords[i].sprite->setTextDatum(TR_DATUM);          // 右对齐

        sprintf(buf, "%5.1f", Coords[i].value);                 // 固定宽度 5, 保留 1 位小数
        Coords[i].sprite->drawString(String(buf), 55, 0);       // x 坐标用 sprite 宽度右对齐
        Coords[i].sprite->pushSprite(Coords[i].x, Coords[i].y); // 推送到屏幕
    }
}


void Free_Move_Coords::Handle_Data(std::vector<uint8_t>& data)
{
//获取坐标数据
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

//更新机械臂状态
    //Get_Robot_State_Data(Global_Data::Data, Global_Data::Robot_States);//基类方法,更新机械臂状态
}


void Free_Move_Coords::MyCobot_Pro_450_Select()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN2)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_Free_Move)))
        {
            button.Wait();
            Free_Move *free_move = new Free_Move(tft, button);
            ui_manager.RegisterScreen(free_move); // 将页面放入注册列表
            Delete_Coord_Sprite();
            ui_manager.Change_UI(free_move, true);
        }

    }

    else if (btn == BTN4)
    {
        button.Wait();
        Delete_Coord_Sprite();
        ui_manager.Go_To(ScreenID::ScreenID_Quick_Move);
    }

}



void Free_Move_Coords::Create_Coord_Sprite()
{
    Xsprite.createSprite(55, 20);
    Ysprite.createSprite(55, 20);
    Zsprite.createSprite(55, 20);
    Rxsprite.createSprite(55, 20);
    Rysprite.createSprite(55, 20);
    Rzsprite.createSprite(55, 20);
}

void Free_Move_Coords::Delete_Coord_Sprite()
{
    Xsprite.deleteSprite();
    Ysprite.deleteSprite();
    Zsprite.deleteSprite();
    Rxsprite.deleteSprite();
    Rysprite.deleteSprite();
    Rzsprite.deleteSprite();
}