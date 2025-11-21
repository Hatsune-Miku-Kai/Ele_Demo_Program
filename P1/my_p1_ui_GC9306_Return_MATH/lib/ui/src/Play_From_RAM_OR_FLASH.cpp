#include "Play_From_RAM_OR_FLASH.h"
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

    if (btn == BTN3 && can_save)
    {
        can_save = 0;
        Save_To_Blockly_Runner *save_to_blockly_runner = new Save_To_Blockly_Runner(tft, button);
        ui_manager.RegisterScreen(save_to_blockly_runner); // 将页面放入注册列表
        Second_sprite.deleteSprite();
        Play_sprite.deleteSprite();
        button.Wait();
        ui_manager.Change_UI(save_to_blockly_runner);
    }

    else if (btn == BTN4)
    {
        if (pause_flag == 0)
        {
            return;
        }

        Program_Stop_450(1);
        button.Wait();
        Second_sprite.deleteSprite();
        Play_sprite.deleteSprite();
        ui_manager.Go_Back();
    }
}

void Play_From_RAM_OR_FLASH::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);

    // Create sprites only once
    Second_sprite.createSprite(55, 20);
    Play_sprite.createSprite(100, 25);

    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Play");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Time : " + String(ui_manager.total_time) + "s", 30, 65);

    tft.setTextColor(TFT_GREEN);
    tft.drawString("Stop", 32, 155);

    tft.pushImage(15, 205, 30, 30, start_logo);
    tft.pushImage(105, 205, 30, 30, stop_logo);
    tft.pushImage(190, 208, 30, 30, cannot_save_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Play_From_RAM_OR_FLASH::Update_UI()
{
    uint8_t btn = button.Get_Button_Status();
    unsigned long now = millis();

    // ============ Playing 状态 ============
    if (Play_State)
    {
        // ---------- Stop ----------
        if (btn == BTN2 && last_btn != BTN2)
        {
            pause_flag = -1;

            Program_Stop_450(1);

            Play_sprite.fillSprite(TFT_BLACK);
            Play_sprite.setTextColor(TFT_GREEN);
            Play_sprite.setFreeFont(&FreeSansBold9pt7b);
            Play_sprite.drawString("Stop", 0, 0);
            Play_sprite.pushSprite(30, 155);

            tft.pushImage(190, 208, 30, 30, can_save_logo);
            tft.pushImage(278, 210, 32, 28, return_logo);

            can_save = 1;
        }

        // ---------- Pause / Resume ----------
        if (btn == BTN1 && last_btn != BTN1)
        {
            if (pause_flag == 0) // Pause
            {
                pause_flag = 1;

                Program_Pause_450(1);

                Play_sprite.fillSprite(TFT_BLACK);
                Play_sprite.setTextColor(TFT_BLUE);
                Play_sprite.setFreeFont(&FreeSansBold9pt7b);
                Play_sprite.drawString("Pausing...", 0, 0);
                Play_sprite.pushSprite(30, 155);

                tft.fillRect(15, 205, 30, 32, TFT_BLACK);
                tft.pushImage(15, 205, 30, 30, start_logo);
                tft.pushImage(190, 208, 30, 30, cannot_save_logo);
                tft.pushImage(278, 210, 32, 28, return_logo);

                elapsed_time += now - last_update;
            }
            else if (pause_flag == 1) // Resume
            {
                pause_flag = 0;

                Program_Resume_450();

                Play_sprite.fillSprite(TFT_BLACK);
                Play_sprite.setTextColor(TFT_YELLOW);
                Play_sprite.setFreeFont(&FreeSansBold9pt7b);
                Play_sprite.drawString("Playing...", 0, 0);
                Play_sprite.pushSprite(30, 155);

                tft.fillRect(15, 205, 30, 32, TFT_BLACK);
                tft.pushImage(15, 205, 30, 32, pause_logo);
                tft.pushImage(190, 208, 30, 30, cannot_save_logo);
                tft.pushImage(278, 210, 32, 28, cannot_return_logo);

                last_update = now;
            }
        }

        // ---------- 累计时间 ----------
        if (pause_flag == 0)
        {
            elapsed_time += now - last_update;
            last_update = now;
        }

        // ---------- 每秒刷新倒计时 ----------
        unsigned long sec = elapsed_time / 1000;
        if (sec != last_sec)
        {
            last_sec = sec;

            if (play_time > 0 && pause_flag == 0)
                play_time--; // 倒数 1 秒

            // ======== 显示秒数（即使是0也要显示） ========
            Second_sprite.fillSprite(TFT_BLACK);
            Second_sprite.setTextColor(TFT_WHITE);
            Second_sprite.setFreeFont(&FreeSansBold9pt7b);
            Second_sprite.drawString(String(play_time) + "s", 0, 0);
            Second_sprite.pushSprite(88, 65);

            // ======== 自动停止 —— 放在显示之后 ========
            if (play_time <= 0)
            {
                Play_State = false;
                pause_flag = -1;
                can_save = 1;

                Play_sprite.fillSprite(TFT_BLACK);
                Play_sprite.setTextColor(TFT_GREEN);
                Play_sprite.setFreeFont(&FreeSansBold9pt7b);
                Play_sprite.drawString("Stop", 0, 0);
                Play_sprite.pushSprite(30, 155);

                tft.pushImage(190, 208, 30, 30, can_save_logo);
                tft.pushImage(278, 210, 32, 28, return_logo);

                last_btn = btn;
                return;
            }
        }

        last_btn = btn;
        return;
    }

    // ============ 第一次开始 ============
    else if (!Play_State)
    {
        if (btn == BTN1 && last_btn != BTN1)
        {
            play_time = ui_manager.total_time;
            Play_State = true;

            Run_Record_450();

            tft.fillRect(15, 205, 30, 32, TFT_BLACK);
            tft.pushImage(15, 205, 30, 32, pause_logo);
            tft.pushImage(278, 210, 32, 28, cannot_return_logo);

            Play_sprite.fillSprite(TFT_BLACK);
            Play_sprite.setTextColor(TFT_YELLOW);
            Play_sprite.setFreeFont(&FreeSansBold9pt7b);
            Play_sprite.drawString("Playing...", 0, 0);
            Play_sprite.pushSprite(30, 155);


            last_update = now;
        }

        last_btn = btn;
    }
}

