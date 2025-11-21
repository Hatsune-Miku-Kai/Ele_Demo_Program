#ifndef PLAY_FROM_RAM_OR_FLASH_H
#define PLAY_FROM_RAM_OR_FLASH_H

#include "Screen_Base.h"
#include "UI_Manager.h"

// 关联界面
#include "Save_To_Blockly_Runner.h"

class Play_From_RAM_OR_FLASH : public Screen_Base
{
public:
    Play_From_RAM_OR_FLASH(TFT_eSPI &tft, Button &button);
    ~Play_From_RAM_OR_FLASH();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;

private:
    void Draw_UI();
    void Update_UI();

#ifdef MyCobot_Pro_450
        void Start_Run();
#endif

    TFT_eSprite Play_sprite = TFT_eSprite(&tft);
    TFT_eSprite Second_sprite = TFT_eSprite(&tft);

    int pause_flag = 0;                   
    unsigned long elapsed_time = 0;       
    unsigned long last_update = millis(); 
    unsigned long last_sec = 0;                 
    uint8_t last_btn = 0; // 上一次按键状态
    uint8_t can_save = 0;
    bool Play_State = false;
    unsigned long play_time = 0;
};



#endif
