#include "UI_Manager.h"
#include "Screen_Base.h"

extern Button button;

UI_Manager::UI_Manager()
{
    Serial.begin(115200);
    button.Init();
}


void UI_Manager::Change_UI(Screen_Base* next_screen, bool returnable)
{
    if (current_screen != nullptr && returnable)
    {
        next_screen->prev_screen = current_screen;//如果下一个界面可以往回跳转,将下一个界面的prev_screen指针指向当前界面
    }

    else if (current_screen != nullptr && !returnable)
    {
        delete current_screen;
        next_screen->prev_screen = nullptr;//如果下一个界面不允许往回跳转,删除当前界面,并将下一个界面的prev_screen指针指向null
    }


    current_screen = next_screen;//不管如何,UI管理器都会认为当前页面发生了切换并修改指针指向
    current_screen->Draw_Static();
}

void UI_Manager::Go_Back()//跳转到某个界面或上一个界面
{
    if(current_screen == nullptr)
        return;

    Screen_Base* target = current_screen->target_screen ? current_screen->target_screen : current_screen->prev_screen;//如果当前页面有指定跳回的指针,就跳回指定界面,否则跳回上一个界面

    delete current_screen;//释放当前界面面资源
    current_screen = target;//将UI管理器的指针指向需要管理的界面,current_screen是一个Screen_Base类型的指针

    if(current_screen != nullptr)//如果该界面有上一个界面,就跳转
        current_screen->Draw_Static();
}

void UI_Manager::Go_Home()//用以返回所有界面公认的家界面,类似linux下的根目录
{
    if(home_screen == nullptr)
        return;

    //将所有链表节点释放掉,因为家界面永远是第一个界面,因此此时链表理应只有一个节点
    while (current_screen != home_screen && current_screen != nullptr)
    {
        Screen_Base* temp = current_screen;//先将当前界面指针指向临时变量

        temp = current_screen->prev_screen;//再将当前界面指针指向上一个界面

        delete current_screen;//释放当前界面资源

        current_screen = temp;//总而言之,这段代码是将当前界面指针指向上一个界面,并释放当前界面资源,循环反复直到碰到家界面
    }

    current_screen = home_screen;//将当前界面指针指向家界面
    current_screen->Draw_Static();//绘制家界面
}

void UI_Manager::loop()
{
    if (current_screen != nullptr) current_screen->Draw_Update();//刷新数据
    if (current_screen != nullptr) current_screen->Handle_Button();//获取按键信息
    String c = Serial_ReadLine();
    // if(c != "")
    // {
    //     Serial.print(c);
    // }
}
