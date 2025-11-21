#include "Coord_Select_Move.h"

//logo
#include "Down_Select_logo.h"
#include "Up_Select_logo.h"
#include "Confirm_logo.h"
#include "Return_logo.h"

//Global_Data
#include "Global_Data.h"

extern UI_Manager ui_manager;

Coord_Select_Move::Coord_Select_Move(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Coords_Select_Move;
}

Coord_Select_Move::~Coord_Select_Move()
{

}


void Coord_Select_Move::Draw_Static()
{
    Draw_UI();
}

void Coord_Select_Move::Draw_Update()
{
    Update_UI();
}

void Coord_Select_Move::Handle_Button()
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

            Fine_Tuning_Coords * fine_tuning_coords = new Fine_Tuning_Coords(tft, button);
            ui_manager.RegisterScreen(fine_tuning_coords);
            Delete_Coord_Sprite();
            ui_manager.Change_UI(fine_tuning_coords,true);
            return;
        }
    else if (btn == BTN4)
        {
            button.Wait();
            Delete_Coord_Sprite();
            ui_manager.Go_Back();
            return;
        }
    else
        return; // 没按键直接返回

    {
        button.Wait();
        tft.setFreeFont(&FreeSansBold12pt7b);
        auto pos = Coord_UI[menuItems[selectedIndex]]; // 根据旧 index 取坐标
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
        auto pos = Coord_UI[menuItems[selectedIndex]];
        int sx = pos.first;
        int sy = pos.second;
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(sx - 15, sy + 5);
        tft.print("*");
    }

}

void Coord_Select_Move::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("Coordinate");

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

    auto pos = Coord_UI[menuItems[selectedIndex]];
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(pos.first - 15, pos.second + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    tft.pushImage(15, 210, 24, 30, down_select_logo);
    tft.pushImage(105, 208, 25, 30, up_select_logo);
    tft.pushImage(190, 210, 30, 30, confirm_logo);
    tft.pushImage(275, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Coord_Sprite();

}

void Coord_Select_Move::Update_UI()
{
    vTaskDelay(50);
    Handle_Data(Global_Data::Data);
    UpdateCoords(Coords_Data[0], Coords_Data[1], Coords_Data[2], Coords_Data[3], Coords_Data[4], Coords_Data[5]);
}


void Coord_Select_Move::UpdateCoords(float x, float y, float z, float rx, float ry, float rz)
{
    int leftX = 27 + 20;    //这里+号表示修改过,去掉+恢复原来的布局
    int rightX = 177 + 30;  //这里+号表示修改过,去掉+恢复原来的布局
    int showY = 62;

    CoordSprite Coords[6] = {
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


void Coord_Select_Move::Handle_Data(std::vector<uint8_t>& data)
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

void Coord_Select_Move::Create_Coord_Sprite()
{
    Xsprite.createSprite(55, 20);
    Ysprite.createSprite(55, 20);
    Zsprite.createSprite(55, 20);
    Rxsprite.createSprite(55, 20);
    Rysprite.createSprite(55, 20);
    Rzsprite.createSprite(55, 20);
}

void Coord_Select_Move::Delete_Coord_Sprite()
{
    Xsprite.deleteSprite();
    Ysprite.deleteSprite();
    Zsprite.deleteSprite();
    Rxsprite.deleteSprite();
    Rysprite.deleteSprite();
    Rzsprite.deleteSprite();
}

