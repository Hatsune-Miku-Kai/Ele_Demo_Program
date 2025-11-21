#include "Record.h"
#include "SPI.hpp"

// logo
#include "Start_logo.h"
#include "Stop_logo.h"
#include "Cannot_Save_logo.h"
#include "Can_Save_logo.h"
#include "Return_logo.h"
#include "Pause_logo.h"
#include "Cannot_Return_logo.h"

extern UI_Manager ui_manager;

Record::Record(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    id = ScreenID::ScreenID_Record;
}

Record::~Record()
{
}

void Record::Draw_Static()
{
    Draw_UI();
}

void Record::Draw_Update()
{
    Update_UI();
}

void Record::Handle_Button()
{
    uint8_t btn = button.Get_Button_Status();
    if (btn == BTN3 && can_save)
    {
        can_save = 0;
        ui_manager.total_time = total_time;
        Save *save = new Save(tft, button);
        ui_manager.RegisterScreen(save);
        Second_sprite.deleteSprite();
        Stop_sprite.deleteSprite();
        button.Wait();
        ui_manager.Change_UI(save, true);
    }

    else if (btn == BTN4)
    {
        if (pause_flag == 0)
        {
            return;
        }

        if (total_time != 0)
        {
            ui_manager.total_time = total_time;
            Second_sprite.deleteSprite();
            Stop_sprite.deleteSprite();
            button.Wait();
            if (ui_manager.total_time < 5)
            {
                Record_Warning_Less_Time *record_warning_less_time = new Record_Warning_Less_Time(tft, button);
                ui_manager.RegisterScreen(record_warning_less_time);
                ui_manager.Change_UI(record_warning_less_time, true);
            }
        }

        else if (total_time == 0)
        {
            button.Wait();
            ui_manager.Go_Back();
        }
    }
}

void Record::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Create sprites only once
    Second_sprite.createSprite(55, 20);
    Stop_sprite.createSprite(100, 25);

    // 标题
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30);
    tft.print("Record");

    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    // 静态文字
    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);

    if (ui_manager.total_time != 0)
    {
        tft.drawString("Time : " + String(ui_manager.total_time) + "s", 30, 65);
        tft.setTextColor(TFT_BLUE);
        tft.drawString("Pausing", 32, 155);
        pause_flag = 1;
        Record_Start = 1;
        total_time = ui_manager.total_time;
    }

    else
    {
        tft.drawString("Time : 0s", 30, 65);
        tft.setTextColor(TFT_GREEN);
        tft.drawString("Stop", 32, 155);
    }

    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Record Limit : 120s", 32, 95);

    // 图标
    tft.pushImage(15, 205, 30, 30, start_logo);
    tft.pushImage(105, 205, 30, 30, stop_logo);
    tft.pushImage(190, 208, 30, 30, cannot_save_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Record::Update_UI()
{

    if (Record_Start == -1)
    {
        return;
    }

    if (total_time >= 5)
    {
        Record_Start = -1;
        Stop_Record_450();
        ui_manager.total_time = total_time;
        Second_sprite.deleteSprite();
        Stop_sprite.deleteSprite();
        Record_Warning_Limit_Time *record_warning_limit_time = new Record_Warning_Limit_Time(tft, button);
        ui_manager.RegisterScreen(record_warning_limit_time);
        ui_manager.Change_UI(record_warning_limit_time, true);
    }

    uint8_t btn = button.Get_Button_Status();
    unsigned long now = millis();

    // ================ 正在录制 ================
    if (Record_Start)
    {
        // ---------- Stop ----------
        if (btn == BTN2 && last_btn != BTN2)
        {
            Stop_Record_450();

            Stop_sprite.fillSprite(TFT_BLACK);
            Stop_sprite.setTextColor(TFT_GREEN);
            Stop_sprite.setFreeFont(&FreeSansBold9pt7b);
            Stop_sprite.drawString("Stop", 0, 0);
            Stop_sprite.pushSprite(32, 155);

            tft.pushImage(190, 208, 30, 30, can_save_logo);
            tft.pushImage(278, 210, 32, 28, return_logo);

            pause_flag = -1;
            can_save = 1;
        }

        // ---------- Pause / Resume ----------
        if (btn == BTN1 && last_btn != BTN1)
        {
            if (pause_flag == 0) // Pause
            {
                Stop_Record_450();
                pause_flag = 1;
                Stop_sprite.fillSprite(TFT_BLACK);
                Stop_sprite.setTextColor(TFT_BLUE);
                Stop_sprite.setFreeFont(&FreeSansBold9pt7b);
                Stop_sprite.drawString("Pausing...", 0, 0);
                Stop_sprite.pushSprite(32, 155);

                tft.pushImage(15, 205, 30, 30, start_logo);
                tft.pushImage(190, 208, 30, 30, cannot_save_logo);
                tft.pushImage(278, 210, 32, 28, return_logo);

                elapsed_time += now - last_update;
            }
            else if (pause_flag == 1) // Resume
            {
                Start_Record_450();
                pause_flag = 0;
                Stop_sprite.fillSprite(TFT_BLACK);
                Stop_sprite.setTextColor(TFT_YELLOW);
                Stop_sprite.setFreeFont(&FreeSansBold9pt7b);
                Stop_sprite.drawString("Recording...", 0, 0);
                Stop_sprite.pushSprite(32, 155);

                tft.pushImage(15, 205, 30, 30, pause_logo);
                tft.pushImage(190, 208, 30, 30, cannot_save_logo);
                tft.pushImage(278, 210, 32, 28, cannot_return_logo);

                last_update = now;
            }
        }

        // ---------- 时间累计 ----------
        if (pause_flag == 0)
        {
            elapsed_time += now - last_update;
            last_update = now;
        }

        // ---------- 每秒刷新 Sprite ----------
        total_time = elapsed_time / 1000;

        if (total_time != last_sec)
        {
            last_sec = total_time;

            // 先刷新显示
            Second_sprite.fillSprite(TFT_BLACK);
            Second_sprite.setTextColor(TFT_WHITE);
            Second_sprite.setFreeFont(&FreeSansBold9pt7b);

            Second_sprite.drawString(String(total_time) + "s", 0, 0);
            Second_sprite.pushSprite(88, 65);

            // 刷新后再判断 → 自动 120 秒停止
            if (total_time >= 120)
            {
                Stop_Record_450();
                pause_flag = -1;
                can_save = 1;

                Stop_sprite.fillSprite(TFT_BLACK);
                Stop_sprite.setTextColor(TFT_GREEN);
                Stop_sprite.setFreeFont(&FreeSansBold9pt7b);
                Stop_sprite.drawString("Stop", 0, 0);
                Stop_sprite.pushSprite(32, 155);

                tft.pushImage(190, 208, 30, 30, can_save_logo);

                last_btn = btn;
                return;
            }
        }

        last_btn = btn;
        return;
    }

    // ================ 第一次开始录制 ================
    else if (!Record_Start && Record_Start != -1)
    {
        if (btn == BTN1 && last_btn != BTN1)
        {
            Start_Record_450();
            Record_Start = true;

            tft.pushImage(15, 205, 30, 30, pause_logo);
            tft.pushImage(278, 210, 32, 28, cannot_return_logo);

            Stop_sprite.fillSprite(TFT_BLACK);
            Stop_sprite.setTextColor(TFT_YELLOW);
            Stop_sprite.setFreeFont(&FreeSansBold9pt7b);
            Stop_sprite.drawString("Recording...", 0, 0);
            Stop_sprite.pushSprite(32, 155);

            last_update = now;
        }

        last_btn = btn;
    }
}

#ifdef MyCobot_Pro_450

#endif