
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "SD_MMC.h"
#include "FS.h"


#include "SPI.hpp"
#include "Serial.hpp"
#include "UI_Manager.h"
#include "Screen_Base.h"
#include "Home.h"



#include "main.hpp"
#include "Button.h"
#include "Program_logo.h"
#include "Return_logo.h"


#define SDIO_CLK  34
#define SDIO_CMD  35
#define SDIO_D0   33
#define SDIO_D1   47
#define SDIO_D2   36
#define SDIO_D3   37


TFT_eSPI tft = TFT_eSPI();
Button button;
UI_Manager ui_manager;
void Task_UI_Manager(void *pvParameters);
void Task_Protocal(void *pvParameters);

void Task_UI_Manager(void *pvParameters)
{
    while (1)
    {
        ui_manager.loop();
    }
}

void Task_Protocal(void *pvParameters)
{
    while (1)
    {
        String c = Serial_ReadLine();
        if(c != "")
        {
            SendGcode(c.c_str());
            Serial.println(c);
        }
    }
}


void setup() 
{
    Init_GC9306();//屏幕初始化需要在SPI前面
    SPI_Init();

    ui_manager.home_screen = new Home(tft, button);
    ui_manager.RegisterScreen(ui_manager.home_screen);//将家页面放在列表头
    ui_manager.Change_UI(ui_manager.home_screen, false);

    xTaskCreatePinnedToCore(Task_UI_Manager, "Task_UI_Manager", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task_Protocal, "Task_Protocal", 4096, NULL, 1, NULL, 1);

}

void loop() 
{

}


