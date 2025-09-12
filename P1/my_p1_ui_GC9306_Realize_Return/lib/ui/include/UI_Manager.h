#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__ 

#include "Serial.hpp"

class Screen_Base;


#include <Arduino.h>

//用以管理UI界面,并为每个UI提供返回和跳转接口
class UI_Manager
{
public:

    UI_Manager();
    void Change_UI(Screen_Base* screen, bool returnable = true);
    void Go_Back();//返回上一个UI界面
    void Go_Home();//返回Home界面



    void loop();//此处执行当前UI界面需要频繁处理的事务,如绘制更新,按键处理等



    Screen_Base* current_screen = nullptr;//用以表示当前的UI结点

    Screen_Base* home_screen = nullptr;//表示对于所有UI而言的Home结点
};





#endif
