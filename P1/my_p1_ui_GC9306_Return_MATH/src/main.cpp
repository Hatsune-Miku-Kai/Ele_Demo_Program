
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "SD_MMC.h"
#include "FS.h"


#include "SPI.hpp"
#include "Serial.hpp"


//Screen_Related
#include "UI_Manager.h"
#include "Screen_Base.h"
#include "Home.h"
#include "Error.h"


#include "main.hpp"
#include "Button.h"
#include "Program_logo.h"
#include "Return_logo.h"

//Global_Data
#include "Global_Data.h"

//logo
#include "Mycobot_logo.h"

#define SDIO_CLK  34
#define SDIO_CMD  35
#define SDIO_D0   33
#define SDIO_D1   47
#define SDIO_D2   36
#define SDIO_D3   37

extern uint8_t Recv_Buffer[1024];

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
        Serial_Read();
        // Serial_Write();
        // vTaskDelay(500);
        // Serial.write(Send, sizeof(Send));
    }
}



void setup() 
{
    Init_24QG217_13();//屏幕初始化需要在SPI前面
    // SPI_Slave_Init();
    Serial.begin(1000000);

    tft.fillScreen(TFT_BLACK);
    // tft.pushImage(0, 0, 320, 240, mycobot_logo);
    // delay(3000);
    ui_manager.home_screen = new Home(tft, button);
    ui_manager.RegisterScreen(ui_manager.home_screen);//将家页面放在列表头
    ui_manager.Change_UI(ui_manager.home_screen, false);

    xTaskCreatePinnedToCore(Task_Protocal, "Task_Protocal", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task_UI_Manager, "Task_UI_Manager", 4096 * 2, NULL, 1, NULL, 1);
}

void loop() 
{
}


