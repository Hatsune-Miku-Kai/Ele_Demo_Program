#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H__


#include <vector>
#include <map>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace Global_Data {
    extern std::vector<uint8_t> Data;
    extern std::map<std::string, uint16_t> Robot_States;
    extern SemaphoreHandle_t Data_Mutex;
}

#endif
