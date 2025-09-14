#include "UI_Manager.h"
#include "Screen_Base.h"

extern Button button;

UI_Manager::UI_Manager()
{
    Serial.begin(115200);
    button.Init();
}


// 注册页面到内部列表
void UI_Manager::RegisterScreen(Screen_Base* screen)
{
    if (!screen) return;

    for (int i = 0; i < registered_pages.size(); ++i)
    {
        if (registered_pages[i] == screen) return; // 如果已经注册,就直接返回
    }
    registered_pages.push_back(screen);
}


// 查找已注册页面
Screen_Base* UI_Manager::GetRegisteredScreenByID(Screen_Base::ScreenID id)
{
    for (int i = 0; i < registered_pages.size(); ++i)
    {
        if (registered_pages[i]->id == id)
            return registered_pages[i];
    }
    return nullptr;
}


// 切换界面
void UI_Manager::Change_UI(Screen_Base* next_screen, bool returnable)
{
    if (current_screen != nullptr && returnable)
    {
        next_screen->prev_screen = current_screen; // 如果下一个界面可以往回跳转,将下一个界面的prev_screen指针指向当前界面
    }
    else if (current_screen != nullptr && !returnable)
    {
        delete current_screen;
        next_screen->prev_screen = nullptr; // 如果下一个界面不允许往回跳转,删除当前界面,并将下一个界面的prev_screen指针指向null
    }

    current_screen = next_screen; // 不管如何,UI管理器都会认为当前页面发生了切换并修改指针指向
    current_screen->Draw_Static();
}


// 跳转到上一个界面
void UI_Manager::Go_Back()
{
    if (current_screen == nullptr || current_screen->prev_screen == nullptr)
        return; // 没有上一个页面就不处理

    Screen_Base* prev = current_screen->prev_screen; // 保存上一个页面指针

    // 从注册表中删除当前页面指针
    for (int i = 0; i < registered_pages.size(); ++i)
    {
        if (registered_pages[i] == current_screen)
        {
            registered_pages.erase(registered_pages.begin() + i);
            break;
        }
    }

    delete current_screen;       // 释放当前页面资源
    current_screen = prev;       // 回退到上一个页面

    // 绘制上一个页面
    if (current_screen != nullptr)
        current_screen->Draw_Static();
}


// 返回家界面
void UI_Manager::Go_Home()
{
    if (home_screen == nullptr)
        return;

    // 将所有链表节点释放掉,因为家界面永远是第一个界面,因此此时链表理应只有一个节点
    while (current_screen != home_screen && current_screen != nullptr)
    {
        Screen_Base* temp = current_screen->prev_screen; // 保存上一个页面

        // 从注册表中删除当前页面
        for (int i = 0; i < registered_pages.size(); ++i)
        {
            if (registered_pages[i] == current_screen)
            {
                registered_pages.erase(registered_pages.begin() + i);
                break;
            }
        }

        delete current_screen; // 释放当前界面资源
        current_screen = temp; // 回退
    }

    current_screen = home_screen; // 将当前界面指针指向家界面
    current_screen->Draw_Static(); // 绘制家界面
}


// 跳转到指定注册页面（跨文件安全）
bool UI_Manager::Go_To(Screen_Base::ScreenID target_id)
{
    if (current_screen == nullptr) return false;

    // 通过ID查找注册页面
    Screen_Base* target = GetRegisteredScreenByID(target_id);
    if (!target) return false;
    if (current_screen == target) return true; // 已经在目标页面

    // 从当前页面向前遍历，删除与目标页面之间的页面,同时更新注册表
    while (current_screen != target)
    {
        Screen_Base* prev = current_screen->prev_screen; // 保存上一个页面指针

        // 从注册表中删除当前页面指针
        for (int i = 0; i < registered_pages.size(); ++i)
        {
            if (registered_pages[i] == current_screen)
            {
                registered_pages.erase(registered_pages.begin() + i);
                break;
            }
        }

        delete current_screen;   // 删除页面对象
        current_screen = prev;   // 回退到上一个页面
    }

    // 绘制目标页面
    if (current_screen != nullptr)
    {
        current_screen->Draw_Static();
        return true;
    }

    return false;//如果所有条件都不满足.返回false
}



// loop 刷新界面和处理按键
void UI_Manager::loop()
{
    if (current_screen != nullptr) current_screen->Draw_Update();      // 刷新数据
    if (current_screen != nullptr) current_screen->Handle_Button();    // 获取按键信息
    String c = Serial_ReadLine();
    // if(c != "")
    // {
    //     Serial.print(c);
    // }
}




//保留,看看以后用不用

// void UI_Manager::Go_Back() // 跳转到上一个或指定界面
// {
//     if (current_screen == nullptr)
//         return;

//     // 如果当前页面有指定跳回的指针,就跳回指定界面,否则跳回上一个界面
//     Screen_Base* target = current_screen->target_screen ? current_screen->target_screen : current_screen->prev_screen;
//     if (!target) return; // 如果目标为空就不处理

//     // 循环删除当前页面到目标页面之间的页面，并更新注册表
//     while (current_screen != target)
//     {
//         Screen_Base* prev = current_screen->prev_screen; // 保存上一个页面指针

//         // 从注册表中删除当前页面指针
//         for (int i = 0; i < registered_pages.size(); ++i)
//         {
//             if (registered_pages[i] == current_screen)
//             {
//                 registered_pages.erase(registered_pages.begin() + i);
//                 break;
//             }
//         }

//         delete current_screen; // 释放当前页面资源
//         current_screen = prev; // 回退到上一个页面
//     }

//     // 绘制目标页面
//     if (current_screen != nullptr)
//         current_screen->Draw_Static();
// }