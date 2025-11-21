#ifndef __FINE_TUNING_ANGLES_H__
#define __FINE_TUNING_ANGLES_H__

#include "Screen_Base.h"
#include "UI_Manager.h"


//logo
#include "Reduce_logo.h"
#include "Add_logo.h"
#include "Can_Save_logo.h"
#include "Return_logo.h"

#define ANGLES_DATA_LEN 6
class Fine_Tuning_Angles : public Screen_Base
{
public:
    Fine_Tuning_Angles(TFT_eSPI& tft, Button& button);
    ~Fine_Tuning_Angles();

    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:
    void Draw_UI();


    struct AngleSprite {
        TFT_eSprite* sprite;
        float value;
        int x;
        int y;
    };

#ifdef MyCobot_Pro_450


    void UpdateAngle_450(float j1, float j2, float j3, float j4, float j5, float j6);
    void MyCobot_Pro_450_UI();
    void MyCobot_Pro_450_Select();
    void Delete_Angles_Sprite();
    void Create_Angles_Sprite();

    void Handle_Data(std::vector<uint8_t>& data);//处理本页面所需数据

    TFT_eSprite J1sprite = TFT_eSprite(&tft);
    TFT_eSprite J2sprite = TFT_eSprite(&tft);
    TFT_eSprite J3sprite = TFT_eSprite(&tft);
    TFT_eSprite J4sprite = TFT_eSprite(&tft);
    TFT_eSprite J5sprite = TFT_eSprite(&tft);
    TFT_eSprite J6sprite = TFT_eSprite(&tft);

    float Angles_Data[ANGLES_DATA_LEN] = {0.0};
    
#endif


#ifdef UltraArm_P1
    void UltraArm_P1_UI();
    void UpdateCoord_P1(float x, float y, float z, float o);
    void UpdateAngle_P1(float j1, float j2, float j3, float j4);
    void UltraArm_P1_Select();
#endif



#ifdef MyCobot_Pro_450

    bool Move_Up = false;
    const int menuCount = 6;
    int selectedIndex = 0; // 当前选中
    const char *menuItems[6] = {
        "J1",
        "J2",
        "J3",
        "J4",
        "J5",
        "J6"};

    std::map<std::string, std::pair<int, int>> Angles_UI = {
        //左列 Angles
        {"J1", {30, 75}},           // {"J1", {10, 75}},
        {"J2", {30, 110}},        // {"J2", {10, 100}},
        {"J3", {30, 145}},       // {"J3", {10, 125}},

        //右列 Angles
        {"J4", {180, 75}},           // {"J4", {150, 75}},
        {"J5", {180, 110}},           // {"J5", {150, 100}},
        {"J6", {180, 145}},           // {"J6", {150, 125}},
    };
    
    std::map<std::string, std::pair<int, int>> Angles_Signal = {

        //左列角度符号
        {"J1", {120, 65}},      // {"J1", {100, 65}},
        {"J2", {120, 100}},     // {"J2", {100, 90}},
        {"J3", {120, 135}},     // {"J3", {100, 115}},  

        //右列角度符号
        {"J4", {270, 65}},     // {"J4", {240, 65}},
        {"J5", {270, 100}},     // {"J5", {240, 90}},
        {"J6", {270, 135}},     // {"J6", {240, 115}},
    };

#endif

#ifdef UltraArm_P1
    std::map<std::string, std::pair<int, int>> Angles_UI = {
        //左列 Angles
        {"J1", {10, 75}},
        {"J2", {10, 100}},
        {"J3", {10, 125}},
    };
#endif


};



#endif
