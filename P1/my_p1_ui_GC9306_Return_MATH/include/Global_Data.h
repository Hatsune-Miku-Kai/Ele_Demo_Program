#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H__


#include <vector>
#include <map>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
using Handle_Cmd_Func = std::function<void(const std::vector<uint8_t>&)>;

namespace Global_Data {
    extern std::vector<uint8_t> Data;//发送数据的数组,没有帧头和校验位和命令和长度
    extern std::vector<uint8_t> Recv_Data_Origin;//接收数据的原始数组,没有帧头和校验位,包含命令和长度
    extern std::vector<uint8_t> BackEnd_Data;//后端主动发送的数据,没有帧头和校验位,包含命令和长度

    extern std::map<std::string, uint16_t> Robot_States;//机器人状态,键值对,键为状态名,值为状态值
    extern SemaphoreHandle_t Data_Mutex;
    extern std::unordered_map<uint8_t, Handle_Cmd_Func> Cmd_Table;//通过cmd找到对应的函数,利用无序映射表
}

#endif
