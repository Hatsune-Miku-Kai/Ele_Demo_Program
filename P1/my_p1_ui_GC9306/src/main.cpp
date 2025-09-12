
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "SD_MMC.h"
#include "FS.h"

#include "Program.h"
#include "Home.h"
#include "Error.h"
#include "IO.h"
#include "Connect.h"
#include "Drag_Teach.h"
#include "Record.h"
#include "SPI.hpp"
#include "Serial.hpp"
#include "Play.h"


#include "SetUp.h"
#include "Program_logo.h"
#include "Return_logo.h"


#define SDIO_CLK  34
#define SDIO_CMD  35
#define SDIO_D0   33
#define SDIO_D1   47
#define SDIO_D2   36
#define SDIO_D3   37

int32_t a = 1;

TFT_eSPI tft = TFT_eSPI();
void Init_GC9306();

Program program(tft);
Home home(tft);
Error error(tft);
IO io(tft);
Connect connect(tft);
Drag_Teach drag_teach(tft);
Record record(tft);
Play play(tft);


uint8_t Send_Buffer[13] = {0xA5, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,};//Get_Angles_Cmd
uint8_t Recv_Buffer[13];

void setup() {

    Serial.begin(115200);
    // delay(1000);

    Init_GC9306();
    // delay(1000);

    SPI_Init();
    // home.Draw_UI();
    io.Draw_UI();
    // tft.pushImage(10, 200, 36, 20, program_logo);
    // tft.pushImage(50, 200, 13, 12, return_logo);
    // error.Draw_UI("Angle is out of limit");
    // io.Draw_UI();
    // connect.Draw_UI();
    // drag_teach.Draw_UI();
    // record.Draw_UI();
    // program.Draw_UI();
    // play.Draw_UI();
    
    // SendArray(Send_Buffer, Recv_Buffer);

    // float J1 = bytes_to_float(Recv_Buffer + 1);
    // float J2 = bytes_to_float(Recv_Buffer + 5);
    // float J3 = bytes_to_float(Recv_Buffer + 9);
    // Serial.println(J1);
    // Serial.println(J2);
    // Serial.println(J3);

    // home.UpdateAngle(J1, J2, J3);
    // delay(1000);



}

void loop() {

    // sendGcode("G01 X-20 F1000");
    // delay(3000);
    // sendGcode("G01 X20 F1000");
    // delay(3000);
    
    String c = Serial_ReadLine();
    if(c != "")
    {
        Serial.print(c);
        //SendGcode(c.c_str());
    }

    // SendArray(Send_Buffer, Recv_Buffer);

    // float J1 = bytes_to_float(Recv_Buffer + 1);
    // float J2 = bytes_to_float(Recv_Buffer + 5);
    // float J3 = bytes_to_float(Recv_Buffer + 9);
    // Serial.println(J1);
    // Serial.println(J2);
    // Serial.println(J3);

    // home.UpdateAngle(J1, J2, J3);
    // delay(3000);
    
    

    // record.Update_UI();
    // delay(50);
    // program.Update_Star();
    // drag_teach.Update_Star( 
}









typedef struct {
    uint8_t cmd;        // 命令
    uint8_t data[14];   // 数据（最多 14 个）
    uint8_t len;        // 数据长度 (bit7=1 表示需要延时 120ms)
} lcd_cmd_t;

lcd_cmd_t lcd_gc9306[] = {
    {0xFE, {0}, 0},
    {0xEF, {0}, 0},

    {0x36, {0x28}, 1},
    {0x3A, {0x05}, 1},

    {0xA4, {0x44, 0x44}, 2},
    {0xA5, {0x42, 0x42}, 2},
    {0xAA, {0x88, 0x88}, 2},
    {0xE8, {0x11, 0x0B}, 2},
    {0xE3, {0x01, 0x10}, 2},
    {0xFF, {0x61}, 1},
    {0xAC, {0x00}, 1},
    {0xAD, {0x33}, 1},   // LDO enable
    {0xAE, {0x2B}, 1},
    {0xAF, {0x55}, 1},

    {0xA6, {0x2A, 0x2A}, 2},
    {0xA7, {0x2B, 0x2B}, 2},
    {0xA8, {0x18, 0x18}, 2},
    {0xA9, {0x2A, 0x2A}, 2},

    // 窗口设置 240x320
    {0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
    {0x2B, {0x00, 0x00, 0x01, 0x3F}, 4},

    {0x35, {0x00}, 1},
    {0x44, {0x00, 0x0A}, 2},

    // Gamma 设置
    {0xF0, {0x02,0x00,0x00,0x25,0x26,0x05}, 6},
    {0xF1, {0x01,0x03,0x00,0x33,0x33,0x0A}, 6},
    {0xF2, {0x05,0x03,0x3C,0x04,0x04,0x4C}, 6},
    {0xF3, {0x09,0x04,0x47,0x03,0x02,0x52}, 6},
    {0xF4, {0x0A,0x17,0x16,0x1F,0x21,0x0F}, 6},
    {0xF5, {0x07,0x12,0x12,0x1F,0x20,0x0F}, 6},

    // 休眠退出 + 延时
    {0x11, {0}, 0 | 0x80},  // Sleep Out
    {0x29, {0}, 0 | 0x80},  // Display On
    {0x2C, {0}, 0},         // Memory Write
};


void Init_GC9306() {

    tft.begin();
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH);
    delay(120);
    digitalWrite(TFT_RST, LOW);
    delay(100);
    digitalWrite(TFT_RST, HIGH);
    delay(120);

    for (uint8_t i = 0; i < (sizeof(lcd_gc9306) / sizeof(lcd_cmd_t)); i++) {
        tft.writecommand(lcd_gc9306[i].cmd);
        for (int j = 0; j < (lcd_gc9306[i].len & 0x7F); j++) {
            tft.writedata(lcd_gc9306[i].data[j]);
        }

        if (lcd_gc9306[i].len & 0x80) {
            delay(120);  // 带延时标志的命令延时
        }
    }
    delay(1000);
}