#include "Program.h"

extern UI_Manager ui_manager;

extern const GFXfont FreeSansBold9pt7b;

Program::Program(TFT_eSPI& tft, Button& button) : Screen_Base(tft, button)
{
    Serial.println("Program created");
    id = ScreenID::ScreenID_Program;
}

Program::~Program()
{
    Serial.println("Program destroyed");
}


void Program::Draw_Static()
{
    Draw_UI();
}


void Program::Draw_Update()
{
    Update_Star();
}



void Program::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();

    if(btn == BTN3)
    {
        switch(selectedIndex)
        {
            case 0://DragTeach
            {
                Drag_Teach* drag_teach = new Drag_Teach(tft, button);
                ui_manager.RegisterScreen(drag_teach);//将页面放入注册列表
                button.Wait();
                ui_manager.Change_UI(drag_teach, true);
                break;
            }

            case 1://Blockly_Runner
            {
                break;
            }

            case 2://Quick_Move
            {
                break;
            }

            case 3://Connection
            {
                break;
            }

            case 4://FirmWare
            {
                break;
            }
        }
    }

    else if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}



void Program::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);//标题起始坐标
    tft.print("Program");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 菜单文字 9pt
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    int y = startY;
    for (int i = 0; i < menuCount; i++)
    {
        tft.setCursor(textX, y);
        tft.print(menuItems[i]);
        y += lineHeight;
    }

    // 默认星号 9pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setCursor(starX, startY + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    // 底部分割线
    tft.drawLine(0, 190, tft.width(), 190, TFT_WHITE);
}


void Program::Update_Star()
{
    uint8_t btn = button.Get_Button_Status(); // 读取当前按键

    int8_t dir = 0;
    if (btn == BTN1)
        dir = 1; // 上
    else if (btn == BTN2)
        dir = -1;  // 下
    else
        return;   // 没按键直接返回

    // 擦除旧的 *
    tft.fillRect(0, startY + (selectedIndex - 1) * lineHeight, textX, lineHeight, TFT_BLACK);
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 更新索引
    selectedIndex += dir;
    if (selectedIndex < 0)
        selectedIndex = menuCount - 1;
    if (selectedIndex >= menuCount)
        selectedIndex = 0;

    // 绘制新的 *
    tft.setCursor(starX, startY + selectedIndex * lineHeight + 5);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");

    button.Wait();
}