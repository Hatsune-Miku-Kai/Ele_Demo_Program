#ifndef __IO_H__
#define __IO_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Connection_Status.h"

#define IO_DATA_START 68//IO数据起始
#define IO_DATA_SIZE 4//先返回Input再返回Output

class IO : public Screen_Base
{
public:
    IO(TFT_eSPI& tft, Button& button);
    ~IO();
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;


    //IO数据
    uint8_t IO_Data[IO_DATA_SIZE];
    
    void UltraArm_P1_Select();
    void MyCobot_Pro_450_Select();


private:
    void Draw_UI();
    void Update_UI();

    void Handle_Data(std::vector<uint8_t>& data);//处理本页面所需数据
    
    void Set_IO_Status(uint8_t Pin, uint8_t Status);
    void Get_IO_Status(uint8_t Pin);
};



#endif

