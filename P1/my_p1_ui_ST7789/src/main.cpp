
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "SD_MMC.h"
#include "FS.h"

#include "Program.h"
#include "Home.h"
#include "Error.h"
#include "IO.h"
#include "Connect.h"

#include "SetUp.h"
#include "Program_logo.h"
#include "Return_logo.h"

#define SDIO_CLK  34
#define SDIO_CMD  35
#define SDIO_D0   33
#define SDIO_D1   47
#define SDIO_D2   36
#define SDIO_D3   37


TFT_eSPI tft = TFT_eSPI();


Program program(tft);
Home home(tft);
Error error(tft);
IO io(tft);
Connect connect(tft);

void setup() {

    Serial.begin(115200);
    delay(1000);

    tft.begin();
    delay(1000);
    tft.setRotation(1);

    


    // home.Draw_UI(); // 绘制菜单和默认 *
    // tft.pushImage(10, 200, 36, 20, program_logo);
    // tft.pushImage(50, 200, 13, 12, return_logo);
    // error.Draw_UI("Angle is out of limit");
    // io.Draw_UI();
    connect.Draw_UI();
}

void loop() {

    // program.Update_Star();
}
