#ifndef __ERROR_H__
#define __ERROR_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

class Error : public Screen_Base
{
public:

    Error(TFT_eSPI& tft, Button& button);
    ~Error();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


private:

};




#endif
