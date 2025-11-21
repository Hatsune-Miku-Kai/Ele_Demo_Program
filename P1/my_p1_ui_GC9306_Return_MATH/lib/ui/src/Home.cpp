#include "Home.h"
#include "SPI.hpp"

// logo
#include "Program_logo.h"
#include "Coord_logo.h"
#include "IO_logo.h"
#include "Connect_logo.h"
#include "Angles_logo.h"

// Global_Data
#include "Global_Data.h"

extern UI_Manager ui_manager;

Home::Home(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    // Serial.println("Home created");
    id = ScreenID::ScreenID_Home;
}

Home::~Home()
{
    // Serial.println("Home destroyed");
}

void Home::Draw_Static()
{
    Draw_UI();
}

void Home::Draw_Update()
{
    vTaskDelay(30);
#ifdef UltraArm_P1
    // SendArray(Send_Buffer, Recv_Buffer);

    // Serial.printf("J1: ");
    // Serial.println(J1);
    // Serial.printf("J2: ");
    // Serial.println(J2);
    // Serial.printf("J3: ");
    // Serial.println(J3);

    // UpdateAngle_P1(J1 , J2, J3, 0);
    // ultraArmP1.rot = (J1) / 180.0 * PI;//1.75;-0.175, 0.060, 1.569
    // ultraArmP1.low = J2 / 180.0 * PI;// 0.87;
    // ultraArmP1.high = (J3 + 90) / 180.0 * PI;// 2.44;
    // ultraArmP1.end = 0;
    // ultraArmP1.calculateCoords();

    // UpdateCoord_P1(ultraArmP1.xmm, ultraArmP1.ymm, ultraArmP1.zmm, ultraArmP1.emm);
#endif
#ifdef MyCobot_Pro_450

    Handle_Data(Global_Data::Data);
    UpdateAngle_450(Angles_Data[0], Angles_Data[1], Angles_Data[2], Angles_Data[3], Angles_Data[4], Angles_Data[5]);
    // tft.fillScreen(TFT_BLACK);
    // for(int i = 10; i < 15; i++)
    // {
    //     tft.setFreeFont(&FreeSansBold9pt7b);
    //     tft.setTextColor(TFT_WHITE);
    //     tft.setCursor(0 + (i-10) * 60, 100);
    //     tft.printf("0x%02X", Global_Data::Data[i]);
    // }
#endif
}

void Home::Handle_Button()
{
#ifdef UltraArm_P1
    UltraArm_P1_Select();
#endif

#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_Select();
#endif
}

#ifdef UltraArm_P1
void Home::UpdateCoord_P1(float x, float y, float z, float o)
{
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // 背景黑色，防止重影

    tft.fillRect(25, 80, 50, 25, TFT_BLACK);
    tft.setCursor(25, 100);
    tft.printf("%6.1f", x);

    tft.fillRect(25, 105, 50, 25, TFT_BLACK);
    tft.setCursor(25, 125);
    tft.printf("%6.1f", y);

    tft.fillRect(25, 130, 50, 25, TFT_BLACK);
    tft.setCursor(25, 150);
    tft.printf("%6.1f", z);

    tft.fillRect(25, 155, 50, 25, TFT_BLACK);
    tft.setCursor(25, 175);
    tft.printf("%6.1f", o);
    // tft.printf("°");
}

void Home::UpdateAngle_P1(float j1, float j2, float j3, float j4)
{
    int rightX = 175;
    int clearX = 180;
    tft.setFreeFont(&FreeSansBold9pt7b);

    tft.fillRect(clearX, 80, 10, 30, TFT_BLACK);
    tft.fillRect(clearX, 105, 10, 30, TFT_BLACK);
    tft.fillRect(clearX, 125, 10, 30, TFT_BLACK);

    tft.setTextColor(TFT_WHITE);

    tft.fillRect(rightX, 80, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 100);
    tft.printf("%6.1f", j1);

    tft.fillRect(rightX, 105, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 125);
    tft.printf("%6.1f", j2);

    tft.fillRect(rightX, 130, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 150);
    tft.printf("%6.1f", j3);

    tft.fillRect(rightX, 155, 50, 25, TFT_BLACK);
    tft.setCursor(rightX, 175);
    tft.printf("%6.1f", j4);
}
#endif

void Home::Draw_UI()
{
#ifdef UltraArm_P1
    UltraArm_P1_UI();
#endif

#ifdef MyCobot_Pro_450
    MyCobot_Pro_450_UI();
#endif
}

#ifdef UltraArm_P1
void Home::UltraArm_P1_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("UltraArm P1");

    // 右上角绿色对勾
    tft.fillCircle(tft.width() - 30, 20, 10, TFT_GREEN);

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 左列 Coord
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(35, 75);
    tft.print("Coord");

    // 右列 Angle
    int rightX = 150;
    tft.setCursor(rightX + 25, 75);
    tft.print("Angle");

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // 背景黑色，防止重影
    tft.setCursor(10, 100);
    tft.printf("X:           mm");

    tft.setCursor(10, 125);
    tft.printf("Y:           mm");

    tft.setCursor(10, 150);
    tft.printf("Z:           mm");

    tft.setCursor(10, 175);
    tft.printf("R:");
    tft.setCursor(80, 165);
    tft.printf("o");

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(230, 90);
    tft.printf("o");
    tft.setCursor(rightX, 100);
    tft.printf("J1:                ");

    tft.setCursor(230, 115);
    tft.printf("o");
    tft.setCursor(rightX, 125);
    tft.printf("J2:         ");

    tft.setCursor(230, 140);
    tft.printf("o");
    tft.setCursor(rightX, 150);
    tft.printf("J3:         ");

    tft.setCursor(230, 165);
    tft.printf("o");
    tft.setCursor(rightX, 175);
    tft.printf("J4:         ");

    tft.fillRect(110, 235, 18, 3, TFT_WHITE);

    tft.pushImage(15, 210, 30, 30, program_logo);
    tft.pushImage(105, 200, 30, 30, coord_logo);
    tft.pushImage(190, 210, 30, 30, io_logo);
    tft.pushImage(275, 217, 30, 18, connect_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}
#endif

#ifdef MyCobot_Pro_450
void Home::MyCobot_Pro_450_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 MyCobot Pro 450 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);

    tft.print("MyCobot Pro 450");

    // 右上角绿色圆,若未上电,红圈
    // if(Global_Data::Robot_States["Power Code"]== 1)
    // {
    tft.fillCircle(tft.width() - 30, 20, 10, TFT_GREEN);
    // }

    // else if(Global_Data::Robot_States["Power Code"] == 0 || Global_Data::Robot_States["Power Code"] == 2)
    // {
    //     tft.fillCircle(tft.width() - 30, 20, 10, TFT_RED);
    //     Error *error = new Error(tft, button);
    //     ui_manager.RegisterScreen(error); // 将页面放入注册列表
    //     button.Wait();
    //     Delete_Angles_Sprite();
    //     ui_manager.Change_UI(error, true);
    // }

    // //对勾
    // tft.drawLine(283, 20, 287, 26, TFT_BLACK);
    // tft.drawLine(288, 26, 298, 16, TFT_BLACK);

    // // 加粗 2px
    // for(int i=-1; i<=1; i++)
    // {  // i控制偏移
    //     tft.drawLine(281, 20+i, 287, 26+i, TFT_BLACK);
    //     tft.drawLine(282+i, 20, 287+i, 26, TFT_BLACK);

    //     tft.drawLine(286, 26+i, 297, 15+i, TFT_BLACK);
    //     tft.drawLine(287+i, 26, 297+i, 15, TFT_BLACK);
    // }

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

    tft.pushImage(15, 210, 30, 30, program_logo);
    tft.pushImage(105, 200, 30, 30, angles_logo);
    tft.pushImage(190, 208, 30, 30, coord_logo);
    tft.pushImage(275, 210, 30, 30, io_logo);

    // ip分割线
    tft.drawLine(0, 170, tft.width(), 170, TFT_WHITE);

    tft.setCursor(60, 190);
    tft.printf("IP: %s", IP_Angles_UI.c_str());

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    Create_Angles_Sprite();
}

void Home::UpdateAngle_450(float j1, float j2, float j3, float j4, float j5, float j6)
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

    IP_Angles_sprite.fillSprite(TFT_BLACK);
    IP_Angles_sprite.setFreeFont(&FreeSansBold9pt7b);
    IP_Angles_sprite.setTextColor(TFT_WHITE);
    IP_Angles_sprite.setTextDatum(TL_DATUM);
    IP_Angles_sprite.drawString(String(IP_Angles_UI.c_str()), 0, 2);
    IP_Angles_sprite.pushSprite(88, 175); //(98,175)
}
#endif

#ifdef UltraArm_P1
void Home::UltraArm_P1_Select()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN1)
    {
        Program *program = new Program(tft, button);
        ui_manager.RegisterScreen(program); // 将页面放入注册列表
        button.Wait();
        ui_manager.Change_UI(program, true);
    }

    else if (btn == BTN3)
    {
        IO *io = new IO(tft, button);
        ui_manager.RegisterScreen(io); // 将页面放入注册列表
        button.Wait();
        ui_manager.Change_UI(io, true);
    }

    else if (btn == BTN4)
    {
        if (!(ui_manager.Go_To(ScreenID::ScreenID_Connection_Status)))
        {
            Connection_Status *connection_status = new Connection_Status(tft, button);
            ui_manager.RegisterScreen(connection_status); // 将页面放入注册列表
            button.Wait();
            ui_manager.Change_UI(connection_status, true);
        }
    }
}
#endif

#ifdef MyCobot_Pro_450
void Home::MyCobot_Pro_450_Select()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN1) 
    {
        Program *program = new Program(tft, button);
        ui_manager.RegisterScreen(program); // 将页面放入注册列表
        button.Wait();
        Delete_Angles_Sprite();
        ui_manager.Change_UI(program, true);
    }

    if (btn == BTN3)
    {
        Coords *coords = new Coords(tft, button);
        ui_manager.RegisterScreen(coords); // 将页面放入注册列表
        button.Wait();
        Delete_Angles_Sprite();
        ui_manager.Change_UI(coords, true);
    }

    else if (btn == BTN4)
    {
        IO *io = new IO(tft, button);
        ui_manager.RegisterScreen(io); // 将页面放入注册列表
        button.Wait();
        Delete_Angles_Sprite();
        ui_manager.Change_UI(io, true);
    }
}

void Home::Handle_Data(std::vector<uint8_t> &data)
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

    // 更新IP地址和端口号
    std::string ip = "";
    uint16_t port = 0;
    for (uint8_t i = IP_DATA_START; i < IP_DATA_START + IP_DATA_LEN; i++)
    {
        ip += std::to_string(data[i]) + ".";
    }
    port = (data[PORT_DATA_START] << 8 | data[PORT_DATA_START + 1]);

    ip.pop_back();
    ip += " : " + std::to_string(port);
    IP_Angles_UI = ip;
}

void Home::Create_Angles_Sprite()
{
    J1sprite.createSprite(55, 20);
    J2sprite.createSprite(55, 20);
    J3sprite.createSprite(55, 20);
    J4sprite.createSprite(55, 20);
    J5sprite.createSprite(55, 20);
    J6sprite.createSprite(55, 20);
    IP_Angles_sprite.createSprite(200, 20);
}

void Home::Delete_Angles_Sprite()
{
    J1sprite.deleteSprite();
    J2sprite.deleteSprite();
    J3sprite.deleteSprite();
    J4sprite.deleteSprite();
    J5sprite.deleteSprite();
    J6sprite.deleteSprite();
    IP_Angles_sprite.deleteSprite();
}

#endif