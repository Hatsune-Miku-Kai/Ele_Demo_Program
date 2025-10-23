#include "Global_Data.h"


namespace Global_Data {
    std::vector<uint8_t> Data(256,0x00);
    std::map<std::string, uint16_t> Robot_States
    {
        {"Power Code", 0},//is power on

        {"Robot Crashed", 0},//is crashed
        {"Robot Moving", 0},//is moving

        {"J1", 0},//over joint limit
        {"J2", 0},//over joint limit
        {"J3", 0},//over joint limit
        {"J4", 0},//over joint limit
        {"J5", 0},//over joint limit
        {"J6", 0},//over joint limit

        {"J1 Motor Code", 0},//motor error
        {"J2 Motor Code", 0},//motor error
        {"J3 Motor Code", 0},//motor errors
        {"J4 Motor Code", 0},//motor error
        {"J5 Motor Code", 0},//motor error
        {"J6 Motor Code", 0},//motor error
        
        {"J1 Software Code", 0},//software error
        {"J2 Software Code", 0},//software error
        {"J3 Software Code", 0},//software error
        {"J4 Software Code", 0},//software error
        {"J5 Software Code", 0},//software error
        {"J6 Software Code", 0},//software error
    };
    SemaphoreHandle_t Data_Mutex = xSemaphoreCreateMutex();
}