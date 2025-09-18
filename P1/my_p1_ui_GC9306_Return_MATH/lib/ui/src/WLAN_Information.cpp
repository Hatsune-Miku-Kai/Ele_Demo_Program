#include <WiFi.h>
#include "WLAN_Information.h"

//logo
#include "Program_logo.h"
#include "Return_logo.h"

extern UI_Manager ui_manager;

extern bool WLAN_Open;
// 需要连接的wifi SSID 和密码
extern const char* ssid;
extern const char* password;

WLAN_Information::WLAN_Information(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_WLAN_Information;
}



WLAN_Information::~WLAN_Information()
{
}


void WLAN_Information::Draw_Static()
{
    Draw_UI();
}


void WLAN_Information::Draw_Update()
{

}

void WLAN_Information::Handle_Button()
{   
    uint8_t btn = button.Get_Button_Status();

    if(btn == BTN3)
    {
        WLAN_Program* wlan_program = new WLAN_Program(tft, button);
        ui_manager.RegisterScreen(wlan_program);//将页面放入注册列表
        button.Wait();
        ui_manager.Change_UI(wlan_program, true);
    }

    else if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }

}


void WLAN_Information::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("WLAN");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    // tft.setTextColor(TFT_WHITE);
    // tft.setCursor(20, 70);
    // tft.print("Status: ");
    // tft.print("Connected");

    // tft.setCursor(20, 95);
    // tft.print("SSID  : ");
    // tft.print("UltraArm P1");

    // tft.setCursor(20, 120);
    // tft.print("IP       : ");
    // tft.print("192.168.1.100");

    // tft.setCursor(20, 145);
    // tft.print("Port    : ");
    // tft.print("8080");

    tft.fillRect(0, 90, tft.width(), 200, TFT_BLACK);
    tft.setCursor(20, 70);
    tft.print("Status: ");
    tft.print("Closed");



    tft.pushImage(190, 210, 30, 30, program_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}


void WLAN_Information::Update_UI()
{
    delay(500);
    if(WLAN_Open)
    {
        WiFi.begin(ssid, password);
        if(WiFi.status() != WL_CONNECTED)
        {
            tft.fillRect(0, 90, tft.width(), 200, TFT_BLACK);
            delay(500);
            tft.setCursor(20, 70);
            tft.print("Status: ");
            tft.print("Connecting...");
        }

        else if(WiFi.status() == WL_CONNECTED)
        {
            tft.setCursor(20, 70);
            tft.print("Status: ");
            tft.print("Connected");

            tft.setCursor(20, 95);
            tft.print("SSID  : ");
            tft.print(ssid);

            tft.setCursor(20, 120);
            tft.print("IP       : ");
            tft.print(WiFi.localIP());

            tft.setCursor(20, 145);
            tft.print("Port    : ");
            tft.print("8080");
        }
    }

    else if(!WLAN_Open)
    {
        tft.fillRect(0, 90, tft.width(), 200, TFT_BLACK);
        tft.setCursor(20, 70);
        tft.print("Status: ");
        tft.print("Closed");
    }
}