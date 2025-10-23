#include "Firmware.h"

// logo
#include "Return_logo.h"

extern UI_Manager ui_manager;

Firmware::Firmware(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Firmware;
}

Firmware::~Firmware()
{
}

void Firmware::Draw_Static()
{
    Draw_UI();
}

void Firmware::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}

void Firmware::Draw_Update()
{
}

void Firmware::Update_UI()
{
}

void Firmware::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // 标题 UltraArm P1 (12pt)
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30);
    tft.print("FirmWare");

    // 顶部分割线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // Robot ID
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);

    tft.pushImage(278, 210, 32, 28, return_logo);

#ifdef UltraArm_P1
    
    int labelX = 35;
    int colonX = 135;
    int valueX = 150;
    std::map<std::string, int>::iterator it;
    for (it = firmwareItems.begin(); it != firmwareItems.end(); ++it)
    {
        const std::string &key = it->first;
        int y = it->second;

        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.setTextColor(TFT_WHITE);

        tft.setCursor(labelX, y);
        tft.print(key.c_str());

        tft.setCursor(colonX, y);
        tft.print(":");

        tft.setCursor(valueX, y);

        if (key == "RobotID")
            tft.print(RoboID);
        else if (key == "Display")
            tft.print(Display);
        else if (key == "Version")
            tft.print(Version);
    }
#endif

#ifdef MyCobot_Pro_450

    int labelX = 35;
    int colonX = 135;
    int valueX = 150;
    std::map<std::string, int>::iterator it;
    for (it = firmwareItems.begin(); it != firmwareItems.end(); ++it)
    {
        const std::string &key = it->first;
        int y = it->second;

        tft.setFreeFont(&FreeSansBold9pt7b);
        tft.setTextColor(TFT_WHITE);

        tft.setCursor(labelX, y);
        tft.print(key.c_str());

        tft.setCursor(colonX, y);
        tft.print(":");

        tft.setCursor(valueX, y);

        if (key == "RobotID")
            tft.print(RoboID);
        else if (key == "Screen")
            tft.print(Screen);
        else if (key == "System")
            tft.print(System);
        else if (key == "Soft")
            tft.print(Software);
        else if (key == "Tool")
            tft.print(Tool);
    }

#endif
}
