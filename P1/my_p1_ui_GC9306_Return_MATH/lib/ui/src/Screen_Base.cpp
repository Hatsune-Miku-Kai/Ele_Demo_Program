#include "Screen_Base.h"


Screen_Base::Screen_Base(TFT_eSPI& tft, Button& button) : tft(tft), button(button)
{
    Serial.println("Screen_Base created");
}


Screen_Base::~Screen_Base() 
{ 
    Serial.println("Screen_Base destroyed");
}