#include "Coords.h"


//logo
#include "Program_logo.h"
#include "Coord_logo.h"
#include "IO_logo.h"
#include "Connect_logo.h"
#include "Angles_logo.h"

//Global_Data
#include "Global_Data.h"

extern UI_Manager ui_manager;

Coords::Coords(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Coords;
}

Coords::~Coords()
{

}


void Coords::Draw_Static()
{
    Draw_UI();
}

void Coords::Draw_Update()
{
    Update_UI();
}

void Coords::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    // if(btn == BTN1) 第一版暂时不用
    // {
    //     Program* program = new Program(tft, button);
    //     ui_manager.RegisterScreen(program);//将页面放入注册列表
    //     button.Wait();
    //     Delete_Coord_Sprite();
    //     ui_manager.Change_UI(program, true);
    // }

    if(btn == BTN2)
    {
        button.Wait();
        Delete_Coord_Sprite();
        ui_manager.Go_To(ScreenID::ScreenID_Home);
    }

    else if(btn == BTN4)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_IO)))
        {
            IO* io = new IO(tft, button);
            ui_manager.RegisterScreen(io);//将页面放入注册列表
            button.Wait();
            Delete_Coord_Sprite();
            ui_manager.Change_UI(io, true);
        }
    }
}

void Coords::Draw_UI()
{
     tft.fillScreen(TFT_BLACK);

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("MyCobot Pro 450");

    // 右上角绿色对勾
    tft.fillCircle(tft.width() - 30, 20, 10, TFT_GREEN);

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

    tft.pushImage(15, 210, 30, 30, program_logo);
    tft.pushImage(105, 208, 30, 30, angles_logo);
    tft.pushImage(190, 200, 30, 30, coord_logo);
    tft.pushImage(275, 210, 30, 30, io_logo);

    tft.fillRect(195, 235, 18, 3, TFT_WHITE);

    //ip分割线
    tft.drawLine(0, 170, tft.width(), 170, TFT_WHITE);

    tft.setCursor(60, 190);
    tft.printf("IP: %s", IP_Coords_UI.c_str());

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Coord_Sprite();

}

void Coords::Update_UI()
{
    vTaskDelay(50);
    Handle_Data(Global_Data::Data);
    // Coords_Data[0]-= 1.8;
    // Coords_Data[1] = 50;
    // Coords_Data[2]+= 1.8;
    // Coords_Data[3]+= 1.8;
    // Coords_Data[4]-= 1.8;
    // Coords_Data[5]--;
    UpdateCoords(Coords_Data[0], Coords_Data[1], Coords_Data[2], Coords_Data[3], Coords_Data[4], Coords_Data[5]);
}


void Coords::UpdateCoords(float x, float y, float z, float rx, float ry, float rz)
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

    IP_Coords_sprite.fillSprite(TFT_BLACK);
    IP_Coords_sprite.setFreeFont(&FreeSansBold9pt7b);
    IP_Coords_sprite.setTextColor(TFT_WHITE);
    IP_Coords_sprite.setTextDatum(TL_DATUM);
    IP_Coords_sprite.drawString(String(IP_Coords_UI.c_str()), 0, 2);
    IP_Coords_sprite.pushSprite(88, 175);
    // int leftX = 30;
    // int rightX = 175;
    // int clearX_left = 40;
    // int clearX_right = 180;
    // int clearY = 60;
    // int showY = 75;

    // //清除左侧数据
    // tft.fillRect(clearX_left, clearY, 45, 20, TFT_BLACK);
    // tft.fillRect(clearX_left, clearY + 25, 45, 20, TFT_BLACK);
    // tft.fillRect(clearX_left, clearY + 50, 45, 20, TFT_BLACK);

    // //清除右侧数据
    // tft.fillRect(clearX_right, clearY, 45, 20, TFT_BLACK);
    // tft.fillRect(clearX_right, clearY + 25, 45, 20, TFT_BLACK);
    // tft.fillRect(clearX_right, clearY + 50, 45, 20, TFT_BLACK);

    // //更新左侧数据
    // tft.setFreeFont(&FreeSansBold9pt7b);
    // tft.setTextColor(TFT_WHITE);

    // tft.setCursor(leftX, showY);
    // tft.printf("%6.2f", x);

    // tft.setCursor(leftX, showY + 25);
    // tft.printf("%6.2f", y);

    // tft.setCursor(leftX, showY + 50);
    // tft.printf("%6.2f", z);

    // //更新右侧数据
    // tft.setCursor(rightX, showY);
    // tft.printf("%6.2f", rx);

    // tft.setCursor(rightX, showY + 25);
    // tft.printf("%6.2f", ry);

    // tft.setCursor(rightX, showY + 50);
    // tft.printf("%6.2f", rz);
}


void Coords::Handle_Data(std::vector<uint8_t>& data)
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

// 更新IP地址和端口号
    std::string ip = "";
    uint16_t port = 0;
    for(uint8_t i = IP_DATA_START; i < IP_DATA_START + IP_DATA_LEN; i++)
    {
        ip += std::to_string(data[i]) + ".";
    }
    port = (data[PORT_DATA_START] << 8 | data[PORT_DATA_START + 1]);

    ip.pop_back();
    ip += " : " + std::to_string(port);
    IP_Coords_UI = ip;

//更新机械臂状态
    //Get_Robot_State_Data(Global_Data::Data, Global_Data::Robot_States);//基类方法,更新机械臂状态
}

void Coords::Create_Coord_Sprite()
{
    Xsprite.createSprite(55, 20);
    Ysprite.createSprite(55, 20);
    Zsprite.createSprite(55, 20);
    Rxsprite.createSprite(55, 20);
    Rysprite.createSprite(55, 20);
    Rzsprite.createSprite(55, 20);
    IP_Coords_sprite.createSprite(200, 20);
}

void Coords::Delete_Coord_Sprite()
{
    Xsprite.deleteSprite();
    Ysprite.deleteSprite();
    Zsprite.deleteSprite();
    Rxsprite.deleteSprite();
    Rysprite.deleteSprite();
    Rzsprite.deleteSprite();
    IP_Coords_sprite.deleteSprite();
}

