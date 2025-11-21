#ifndef __CALIBRATION_JOINT_H__
#define __CALIBRATION_JOINT_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Automatic_Calibration.h"
#include "Fine_Tuning_Angles.h"

class Calibration_Joint : public Screen_Base
{
public:
        Calibration_Joint(TFT_eSPI& tft, Button& button);
        ~Calibration_Joint();
        void Draw_Static() override;
        void Draw_Update() override;
        void Handle_Button() override;

private:
        void Update_UI();

        uint8_t Calibration_Status = 0;//0--未开始校准; 1--校准中; 2--校准完成

        
    struct AngleSprite {
        TFT_eSprite* sprite;
        float value;
        int x;
        int y;
    };

#ifdef MyCobot_Pro_450
    #define ANGLES_DATA_LEN 6

    void UpdateAngle_450(float j1, float j2, float j3, float j4, float j5, float j6);
    void MyCobot_Pro_450_UI();
    void MyCobot_Pro_450_Select();
    void Delete_Angles_Sprite();
    void Create_Angles_Sprite();

    void Handle_Data(std::vector<uint8_t>& data);//处理本页面所需数据

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

    TFT_eSprite J1sprite = TFT_eSprite(&tft);
    TFT_eSprite J2sprite = TFT_eSprite(&tft);
    TFT_eSprite J3sprite = TFT_eSprite(&tft);
    TFT_eSprite J4sprite = TFT_eSprite(&tft);
    TFT_eSprite J5sprite = TFT_eSprite(&tft);
    TFT_eSprite J6sprite = TFT_eSprite(&tft);

    float Angles_Data[ANGLES_DATA_LEN] = {0.0};
#endif


#ifdef UltraArm_P1
        UltraArm_P1_Select();
#endif



};

#endif

