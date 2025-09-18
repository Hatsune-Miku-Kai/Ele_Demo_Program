#ifndef __FREE_MOVE_H__
#define __FREE_MOVE_H__


#include "Screen_Base.h"
#include "UI_Manager.h"


class Free_Move:public Screen_Base
{
public:
    Free_Move(TFT_eSPI& tft, Button& button);
    ~Free_Move() override;
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_UI();
    void UpdateAngle(float j1, float j2, float j3, float j4);
    void UpdateCoord(float x, float y, float z, float o);

};





#endif

