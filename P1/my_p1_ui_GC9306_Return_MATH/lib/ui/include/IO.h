#ifndef __IO_H__
#define __IO_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Connection_Status.h"

class IO : public Screen_Base
{
public:
    IO(TFT_eSPI& tft, Button& button);
    ~IO();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;



private:
    void Draw_UI();
    void Update_UI();
};



#endif

