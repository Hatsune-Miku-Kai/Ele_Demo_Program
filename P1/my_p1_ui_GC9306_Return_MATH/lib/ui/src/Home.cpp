#include "Home.h"
#include "SPI.hpp"

//logo
#include "Program_logo.h"
#include "Coord_logo.h"
#include "IO_logo.h"
#include "Connect_logo.h"


extern UI_Manager ui_manager;

extern uint8_t Send_Buffer[20];
extern uint8_t Recv_Buffer[20];
extern float parsed[6];
extern float J1;
extern float J2;
extern float J3; 

Home::Home(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    Serial.println("Home created");
    id = ScreenID::ScreenID_Home;
}


Home::~Home()
{
    Serial.println("Home destroyed");
}


void Home::Draw_Static()
{
    Draw_UI();
}

void Home::Draw_Update()
{
    SendArray(Send_Buffer, Recv_Buffer);

    UpdateAngle(J1+35 , J2, J3, 0);
    ultraArmP1.rot = (J1+35) / 180.0 * PI;//1.75;-0.175, 0.060, 1.569
	ultraArmP1.low = J2 / 180.0 * PI;// 0.87;
	ultraArmP1.high = (J3 + 90) / 180.0 * PI;// 2.44;
	ultraArmP1.end = 0;
	ultraArmP1.calculateCoords();

    UpdateCoord(ultraArmP1.xmm, ultraArmP1.ymm, ultraArmP1.zmm, ultraArmP1.emm);
    delay(100);
}


void Home::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN1)
    {
        Program* program = new Program(tft, button);
        ui_manager.RegisterScreen(program);//将页面放入注册列表
        button.Wait();
        ui_manager.Change_UI(program, true);
    }

    else if (btn == BTN3)
    {
        IO* io = new IO(tft, button);
        ui_manager.RegisterScreen(io);//将页面放入注册列表
        button.Wait();
        ui_manager.Change_UI(io, true);
    }

    else if (btn == BTN4)
    {
        if(!(ui_manager.Go_To(ScreenID::ScreenID_Connection_Status)))
        {
            Connection_Status* connection_status = new Connection_Status(tft, button);
            ui_manager.RegisterScreen(connection_status);//将页面放入注册列表
            button.Wait();
            ui_manager.Change_UI(connection_status, true);
        }
    }

}


void Home::UpdateCoord(float x, float y, float z, float o)
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


void Home::UpdateAngle(float j1, float j2, float j3, float j4)
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


void Home::Draw_UI()
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