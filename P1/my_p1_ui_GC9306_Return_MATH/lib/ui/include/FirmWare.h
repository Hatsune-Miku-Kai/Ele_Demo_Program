#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

class Firmware : public Screen_Base
{
public:

    Firmware(TFT_eSPI& tft, Button& button);
    ~Firmware();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_UI();

#ifdef UltraArm_P1

    #define RoboID "01"
    #define Display "V1.0"
    #define Version "V1.0"
    std::map<std::string, int> firmwareItems = {
        {"RobotID", 75},
        {"Display", 100},
        {"Version", 125},
    };
#endif
    
#ifdef MyCobot_Pro_450

    #define RoboID "01"
    #define Screen "V1.0"
    #define System "V1.0"
    #define Software "V1.0"
    #define Tool "V1.0"

    std::map<std::string, int> firmwareItems = {
        {"RobotID", 75},
        {"Screen", 100},
        {"System", 125},
        {"Soft",   150},
        {"Tool",   175}
    };

#endif


};

#endif
