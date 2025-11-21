#ifndef RECORD_H
#define RECORD_H  

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Save.h"
#include "Record_Warning_Less_Time.h"
#include "Record_Warning_Limit_Time.h"



class Record : public Screen_Base
{
public:
    Record(TFT_eSPI &tft, Button &button);
    ~Record();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;



private:
    void Draw_UI();
    void Update_UI();

    TFT_eSprite Second_sprite = TFT_eSprite(&tft);
    TFT_eSprite Stop_sprite = TFT_eSprite(&tft);

    int pause_flag = 0;                   
    unsigned long elapsed_time = 0;       
    unsigned long last_update = millis(); 
    unsigned long last_sec = 0;                         
    uint8_t last_btn = 0; // 上一次按键状态

    unsigned long total_time = 0; // 总时间
    uint8_t can_save = 0;//是否可以保存
    int8_t Record_Start = 0; //是否正在记录

};







#endif
