
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


uint8_t Send_Buffer[13] = {0xA5, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,};//Get_Angles_Cmd
uint8_t Recv_Buffer[13];

void setup() 
{
    Init_GC9306();//屏幕初始化需要在SPI前面
    SPI_Init();

    ui_manager.home_screen = new Home(tft, button);
    ui_manager.Change_UI(ui_manager.home_screen, false);
}

void loop() 
{
    ui_manager.loop();
}


