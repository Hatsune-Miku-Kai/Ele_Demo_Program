#ifndef WLAN_INFORMATION_H
#define WLAN_INFORMATION_H


#include "Screen_Base.h"
#include "UI_Manager.h"


//关联界面
#include "WLAN_Program.h"


class WLAN_Information : public Screen_Base
{
public:
    WLAN_Information(TFT_eSPI &tft, Button &button);
    ~WLAN_Information();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:
    void Draw_UI();
    void Update_UI();

};







#endif
