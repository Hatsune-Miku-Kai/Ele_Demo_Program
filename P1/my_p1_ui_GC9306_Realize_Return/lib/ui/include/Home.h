#ifndef __HOME_H__
#define __HOME_H__

#include "Screen_Base.h"
#include "UI_Manager.h"

//关联界面
#include "Program.h"
#include "IO.h"
#include "Connection_Status.h"

class Home:public Screen_Base
{
public:
    Home(TFT_eSPI& tft, Button& button);//派生类的构造函数调用父类构造函数来引用tft
    ~Home() override;
    void Draw_Static() override;
    void Draw_Update() override;
    void Handle_Button() override;
        //派生类不可以重新声明新的指针,不然会形成菱形继承




//内部函数
private: 

    void Draw_UI();
    void UpdateCoord(float x, float y, float z, float o);
    void UpdateAngle(float j1, float j2, float j3, float j4);


};


#endif

