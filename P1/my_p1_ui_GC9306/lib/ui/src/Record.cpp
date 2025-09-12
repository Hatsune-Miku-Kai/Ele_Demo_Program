#include "Record.h"

int8_t pause_flag = 0;
Record::Record(TFT_eSPI &display) : tft(display)
{
}

Record::~Record()
{
}

void Record::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Record");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE);

    tft.setCursor(30, 70);
    tft.print("Points :");

    tft.setCursor(30, 95);
    tft.print("Time :");
    
    tft.setCursor(90, 95);
    tft.print(0);
    tft.print("s");

    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(30, 175);
    tft.print("Recoding...");
}

void Record::Update_UI()
{
    static int pause_flag = 0;                   
    static unsigned long elapsed_time = 0;       
    static unsigned long last_update = millis(); 
    static unsigned long last_sec = 0;           
    static uint32_t point_num = 0;               
    static uint8_t last_btn = 0; // 上一次按键状态
    static unsigned long last_record_time = 0;   // 上一次记录点位时间
    static uint8_t can_save = 0;

    unsigned long now = millis();
    uint8_t btn = board.Get_Button_Status();

    // --- Stop 状态 ---
    if (btn == BTN2 && last_btn != BTN2) // 只有边沿触发
    {
        tft.fillRect(30, 155, 200, 30, TFT_BLACK);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(30, 175);
        tft.print("Stop");
        pause_flag = -1;
        can_save = 1;
    }

    // --- Pause / Resume ---
    if (btn == BTN1 && last_btn != BTN1) // 边沿触发
    {
        if (pause_flag == 0)
        {
            tft.fillRect(30, 155, 200, 30, TFT_BLACK);
            tft.setTextColor(TFT_BLUE, TFT_BLACK);
            tft.setCursor(30, 175);
            tft.print("Pausing...");
            pause_flag = 1;
            elapsed_time += now - last_update;
        }
        else if (pause_flag == 1)
        {
            tft.fillRect(30, 155, 200, 30, TFT_BLACK);
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.setCursor(30, 175);
            tft.print("Recording...");
            pause_flag = 0;
            last_update = now;
        }
    }

    // --- Save ---
    if (btn == BTN3 && last_btn != BTN3 && can_save)
    {
        Save();
        can_save = 0;
    }

    // --- 更新时间累积 ---
    if (pause_flag == 0)
    {
        elapsed_time += now - last_update;
        last_update = now;
    }

    // --- 每秒刷新显示 ---
    unsigned long sec = elapsed_time / 1000;
    if (sec != last_sec)
    {
        last_sec = sec;
        tft.fillRect(90, 80, 60, 40, TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(90, 95);
        tft.print(sec);
        tft.print("s");
    }

    // --- 每 10ms 记录一次点位 ---
    if (pause_flag == 0 && (now - last_record_time >= 10))
    {
        last_record_time = now;
        point_num++;

        tft.fillRect(90, 50, 60, 30, TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(90, 70);
        tft.print(point_num);
    }

    // 保存按键状态
    last_btn = btn;
}



void Record::Save()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Save");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSans9pt7b);
    tft.setTextColor(TFT_WHITE);

    tft.setCursor(30, 70);
    tft.print("Save into RAM");

    tft.setCursor(30, 95);
    tft.print("Save into FLASH");

    tft.setFreeFont(&FreeSans9pt7b);
    tft.setCursor(starX, startY);
    tft.setTextColor(TFT_WHITE);
    tft.print("*");
    delay(500);

    while (1)
    {
        uint8_t btn = board.Get_Button_Status(); // 读取当前按键
        int8_t dir = 0;
        if (btn == BTN1)
            dir = -1; // 上
        else if (btn == BTN2)
            dir = 1; // 下
        else if (btn == BTN3)
            {
                tft.fillRect(0, 50, tft.width(), 80, TFT_BLACK);
                tft.fillRect(0, 95, tft.width(), 80, TFT_BLACK);
                tft.setCursor(110,125);
                tft.print("Saving...");
                delay(3000);
                tft.fillRect(110, 100, tft.width(), 80, TFT_BLACK);
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
                tft.setCursor(110,125);
                tft.print("Saved!");
                break;
            } 
        else if (btn == BTN4)
            {
                //先保留不写返回的逻辑
            }
        else 
            continue; // 没按键直接返回

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
        tft.setCursor(starX, startY + selectedIndex * lineHeight);
        tft.setTextColor(TFT_WHITE);
        tft.print("*");

        while (board.Get_Button_Status() != 0) // 消抖
        {
            delay(20);
        }
    }
}
