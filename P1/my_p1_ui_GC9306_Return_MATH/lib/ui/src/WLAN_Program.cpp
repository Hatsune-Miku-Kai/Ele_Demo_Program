#include <WiFi.h>
#include "WLAN_Program.h"

// logo
#include "Confirm_logo.h"
#include "Up_Select_logo.h"
#include "Down_Select_logo.h"
#include "Return_logo.h"
#include "Switch_Off_logo.h"
#include "Switch_On_logo.h"

extern UI_Manager ui_manager;

static int8_t Star_Dir = 0;

bool WLAN_Open = false;

const char *ssid = "ElephantWiFi-AP";
const char *password = "Aria6666";

WLAN_Program::WLAN_Program(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_WLAN_Program;
}

WLAN_Program::~WLAN_Program()
{
}

void WLAN_Program::Draw_Static()
{
    Draw_UI();
}

void WLAN_Program::Draw_Update()
{
    Update_UI();
}

void WLAN_Program::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN3)
    {
        if (WLAN_Open == false)
        {
            switch (selectedIndex)
            {
                case 0:
                {
                    Draw_White_Program();
                    tft.fillRect(250, 55, 50, 50, TFT_BLACK);
                    tft.pushImage(250, 55, 30, 20, switch_on_logo);
                    WLAN_Open = true;
                    button.Wait();
                    break;
                }

                case 1:
                {
                    break;
                }
            }
        }

        else if (WLAN_Open)
        {
            switch (selectedIndex)
            {
                case 0:
                {
                    Draw_Gray_Program();
                    tft.fillRect(250, 55, 50, 50, TFT_BLACK);
                    tft.pushImage(250, 55, 30, 18, switch_off_logo);
                    WLAN_Open = false;
                    button.Wait();
                    break;
                }

                case 1:
                {
                    break;
                }

                case 2:
                {
                    break;
                }

                case 3:
                {
                    break;
                }
            }
        }
    }

    else if (btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}

void WLAN_Program::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    selectedIndex = 0; // 默认选中第一个选项

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("WLAN");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    if (WLAN_Open)
    {
        Draw_White_Program();
    }

    else if (!WLAN_Open)
    {
        Draw_Gray_Program();
    }

    // 默认星号 9pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(starX, startY + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    tft.pushImage(250, 55, 30, 18, switch_off_logo);
    tft.pushImage(15, 205, 24, 30, down_select_logo);
    tft.pushImage(105, 205, 25, 30, up_select_logo);
    tft.pushImage(190, 208, 30, 30, confirm_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void WLAN_Program::Update_UI()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键

    if (btn == BTN1)
        Star_Dir = 1; // 下
    else if (btn == BTN2)
        Star_Dir = -1; // 上
    else
        return; // 没按键直接返回
    if (WLAN_Open)
    {
        Update_White_Program();
    }

    else if (!WLAN_Open)
    {
        Update_Gray_Program();
    }

    Star_Dir = 0;
    button.Wait();
}

void WLAN_Program::WLAN_Start()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void WLAN_Program::Draw_Gray_Program()
{
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;

    tft.setCursor(textX, y);
    tft.print("Switch");
    y += lineHeight;

    tft.setTextColor(TFT_DARKGREY);
    tft.setCursor(textX, y);
    tft.print("Avaliable");
    y += lineHeight;

    tft.setTextColor(TFT_DARKGREY);
    tft.setCursor(textX, y);
    tft.print("History");
    y += lineHeight;

    tft.setTextColor(TFT_WHITE);
    tft.setCursor(textX, y);
    tft.print("Properties");
}

void WLAN_Program::Draw_White_Program()
{
    // 菜单文字 9pt
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;
    for (int i = 0; i < menuCount; i++)
    {
        tft.setCursor(textX, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }
}

void WLAN_Program::Update_White_Program()
{
    tft.setFreeFont(&FreeSansBold12pt7b);
    // 擦除旧的 *
    tft.fillRect(0, 30, textX, 120, TFT_BLACK);
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 更新索引
    selectedIndex += Star_Dir;
    if (selectedIndex < 0)
        selectedIndex = menuCount - 1;
    if (selectedIndex >= menuCount)
        selectedIndex = 0;

    // 绘制新的 *
    tft.setCursor(starX, startY + selectedIndex * lineHeight + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");
}

void WLAN_Program::Update_Gray_Program()
{
    tft.setFreeFont(&FreeSansBold12pt7b);
    // 擦除旧的 *
    tft.fillRect(0, 30, textX, 120, TFT_BLACK);
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 更新索引
    selectedIndex += Star_Dir;
    if (selectedIndex < 0)
        selectedIndex = 1;
    if (selectedIndex >= 2)
        selectedIndex = 0;

    // 绘制新的 *
    tft.setCursor(starX, startY + selectedIndex * lineHeight * 3 + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");
}