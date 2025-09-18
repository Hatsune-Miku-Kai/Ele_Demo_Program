#ifndef __CALIBRATION_JOINT_H__
#define __CALIBRATION_JOINT_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Automatic_Calibration.h"

class Calibration_Joint : public Screen_Base
{
public:
        Calibration_Joint(TFT_eSPI& tft, Button& button);
        ~Calibration_Joint();
        void Draw_Static() override;
        void Draw_Update() override;
        void Handle_Button() override;

private:
        void Draw_UI(uint8_t index);
        void Update_UI();

        uint8_t Calibration_Status = 0;//0--未开始校准; 1--校准中; 2--校准完成

};







#endif

