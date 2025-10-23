#ifndef __ERROR_H__
#define __ERROR_H__

#include "Screen_Base.h"
#include "UI_Manager.h"


// #define ERROR_DATA_START 24
// #define ERROR_DATA_LEN 32

class Error : public Screen_Base
{
public:

    Error(TFT_eSPI& tft, Button& button);
    ~Error();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

    TFT_eSprite Error_Sprite = TFT_eSprite(&tft);

private:
    void Draw_UI();
    void Update_UI(std::map<std::string, uint16_t>& Error_Status);
    void Handle_Error_Data(std::vector<uint8_t>& data);
    void Create_Error_Sprite();
    void Delete_Error_Sprite();

    std::string Over_Limit_Error_Msg = "";
    std::string Motor_Error_Msg = "";
    std::string Software_Error_Msg = "";
};




#endif
