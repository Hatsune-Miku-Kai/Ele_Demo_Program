#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

class FirmWare : public Screen_Base
{
public:

    FirmWare(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button);
    ~FirmWare();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_UI();
    

};

#endif
