#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__ 

#include "Serial.hpp"
#include <vector>
#include "Global_Data.h"
// class Screen_Base;

#include "Screen_Base.h"

#include <Arduino.h>

// extern std::vector<uint8_t> Data;

//用以管理UI界面,并为每个UI提供返回和跳转接口
class UI_Manager
{
public:

    UI_Manager();
    void Change_UI(Screen_Base* screen, bool returnable = true);
    void Go_Back();//返回上一个UI界面
    void Go_Home();//返回Home界面
    bool Go_To(Screen_Base::ScreenID target_id);//跳转到指定UI界面
    void RegisterScreen(Screen_Base* screen);//注册页面到内部列表

    
    void Get_Robot_State_Data(std::vector<uint8_t> &data, std::map<std::string, uint16_t>& robot_states);//从数据中提取机械臂状态

    void Handle_Robot_States_Data(std::map<std::string, uint16_t>& robot_states);//给所有子类提供处理机械臂状态的接口

    Screen_Base* GetRegisteredScreenByID(Screen_Base::ScreenID id);//查找已注册页面


    void loop();//此处执行当前UI界面需要频繁处理的事务,如绘制更新,按键处理等

    Screen_Base* current_screen = nullptr;//用以表示当前的UI结点

    Screen_Base* home_screen = nullptr;//表示对于所有UI而言的Home结点

    uint8_t Select_id = 0;//点动时当前选中的id,可用于传递给校准界面或其他界面

    std::string Select_Name = "";//点动时当前选中的名称,可用于传递给校准界面或其他界面

    unsigned long total_time = 0; // 拖动示教录制总时间

    bool Is_Drag_Tech_Save = false;//拖动示教是否保存了

    

    // Screen_Base* error_screen = nullptr;//表示错误界面

private:
    std::vector<Screen_Base*> registered_pages;//已注册页面列表,也可以反应页面的前后关系

    
};





#endif
