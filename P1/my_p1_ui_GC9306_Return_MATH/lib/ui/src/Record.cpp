#include "Record.h"
#include "SPI.hpp"

// logo
#include "Start_logo.h"
#include "Stop_logo.h"
#include "Cannot_Save_logo.h"
#include "Can_Save_logo.h"
#include "Return_logo.h"
#include "Pause_logo.h"

extern UI_Manager ui_manager;

Record::Record(TFT_eSPI &tft, Button &button) : Screen_Base(tft, button)
{
    Serial.println("Record created");
    id = ScreenID::ScreenID_Record;
}

Record::~Record()
{
    Serial.println("Record destroyed");
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
        Save *save = new Save(tft, button);
        ui_manager.RegisterScreen(save); // 将页面放入注册列表
        button.Wait();
        ui_manager.Change_UI(save, true);
    }

    else if (btn == BTN4)
    {
        button.Wait();
        ui_manager.Go_Back();
    }
}

void Record::Draw_UI()
{
    tft.fillScreen(TFT_BLACK);



    // Program 标题 12pt
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(15, 30); // 标题起始坐标
    tft.print("Record");

    // 顶部横线
    tft.drawLine(0, 40, tft.width(), 40, TFT_WHITE);

    tft.setFreeFont(&FreeSansBold9pt7b);
    tft.setTextColor(TFT_WHITE);

    tft.setCursor(30, 70);
    tft.print("Points :  ");
    tft.print(0);

    tft.setCursor(30, 95);
    tft.print("Time :");

    tft.setCursor(90, 95);
    tft.print(0);
    tft.print("s");

    tft.setTextColor(TFT_BLUE);
    tft.setCursor(30, 175);
    tft.print("Pausing");

    tft.pushImage(15, 205, 30, 30, start_logo);
    tft.pushImage(105, 205, 30, 30, stop_logo);
    tft.pushImage(190, 208, 30, 30, cannot_save_logo);
    tft.pushImage(278, 210, 32, 28, return_logo);

    // 底部分割线
    tft.drawLine(0, 200, tft.width(), 200, TFT_WHITE);
}

void Record::Update_UI()
{
    uint8_t btn = button.Get_Button_Status();
    unsigned long now = millis();
    if (Record_Start)
    {
        // --- Stop 状态 ---
        if (btn == BTN2 && last_btn != BTN2) // 只有边沿触发
        {
            SendGcode("$X\r\n");   // 解锁机器
            SendGcode("G153\r\n"); // 发送绿色灯Gcode指令

            tft.fillRect(30, 155, 200, 30, TFT_BLACK);
            tft.setTextColor(TFT_GREEN);
            tft.setCursor(30, 175);
            tft.print("Stop");
            tft.pushImage(190, 208, 30, 30, can_save_logo);
            pause_flag = -1;
            can_save = 1;
        }

        // --- Pause / Resume ---
        if (btn == BTN1 && last_btn != BTN1) // 边沿触发
        {
            if (pause_flag == 0)
            {
                SendGcode("$X\r\n");   // 解锁机器
                SendGcode("G152\r\n"); // 发送蓝色灯Gcode指令

                tft.fillRect(15, 205, 30, 32, TFT_BLACK);
                tft.pushImage(15, 205, 30, 30, start_logo);

                tft.fillRect(30, 155, 200, 30, TFT_BLACK);
                tft.setTextColor(TFT_BLUE);
                tft.setCursor(30, 175);
                tft.print("Pausing...");
                pause_flag = 1;
                elapsed_time += now - last_update;
            }
            else if (pause_flag == 1)
            {
                SendGcode("$X\r\n");   // 解锁机器
                SendGcode("G151\r\n"); // 发送黄色灯Gcode指令

                tft.fillRect(15, 205, 30, 32, TFT_BLACK);
                tft.pushImage(15, 205, 30, 32, pause_logo);

                tft.fillRect(30, 155, 200, 30, TFT_BLACK);
                tft.setTextColor(TFT_YELLOW);
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
            tft.setTextColor(TFT_WHITE);
            tft.setCursor(90, 95);
            tft.print(sec);
            tft.print("s");
        }

        // --- 每 10ms 记录一次点位 ---
        if (pause_flag == 0 && (now - last_record_time >= 10))
        {
            last_record_time = now;
            point_num++;

            tft.fillRect(95, 50, 60, 30, TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setCursor(100, 70);
            tft.print(point_num);
        }

        // 保存按键状态
        last_btn = btn;
    }

    else if (!Record_Start)
    {
        if(btn == BTN1 && last_btn != BTN1)
        {
            Record_Start = true;
            tft.fillRect(15, 205, 30, 32, TFT_BLACK);
            tft.pushImage(15, 205, 30, 32, pause_logo);
            last_btn = btn;
            tft.fillRect(30, 155, 200, 30, TFT_BLACK);
            tft.setTextColor(TFT_YELLOW);
            tft.setCursor(30, 175);
            last_update = now;
            tft.print("Recording...");

            SendGcode("$X\r\n");   // 解锁机器
            SendGcode("G151\r\n"); // 发送黄色灯Gcode指令
            delay(100);
            SendGcode("$X\r\n");  // 解锁机器
            SendGcode("G11\r\n"); // 开始拖动示教
        }
    }
}