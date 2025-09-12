#include "Play_From_RAM_OR_FLASH.h"

extern UI_Manager ui_manager;

static uint8_t can_save = 0;

Play_From_RAM_OR_FLASH::Play_From_RAM_OR_FLASH(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
}

Play_From_RAM_OR_FLASH::~Play_From_RAM_OR_FLASH()
{
}

void Play_From_RAM_OR_FLASH::Draw_Static()
{
    Draw_UI();
}

void Play_From_RAM_OR_FLASH::Draw_Update()
{
    Update_UI();
}

void Play_From_RAM_OR_FLASH::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if(btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }

}

void Play_From_RAM_OR_FLASH::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Play");

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
    tft.print("Playing...");

    // 底部分割线
    tft.drawLine(0, 190, tft.width(), 190, TFT_WHITE);
}

void Play_From_RAM_OR_FLASH::Update_UI()
{
    unsigned long now = millis();
    uint8_t btn = button.Get_Button_Status();

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