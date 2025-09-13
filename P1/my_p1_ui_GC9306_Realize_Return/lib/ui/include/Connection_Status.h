#ifndef __CONNECTION_STATUS_H__
#define __CONNECTION_STATUS_H__


#include "Screen_Base.h"
#include "UI_Manager.h"


//关联界面
#include "Home.h"
#include "IO.h"


class Connection_Status : public Screen_Base
{
public:
    Connection_Status(TFT_eSPI& tft, Button& button);
    ~Connection_Status();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;



private:
    void Draw_UI();
    void Update_UI();

};






#endif


