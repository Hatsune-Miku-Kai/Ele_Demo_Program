#pragma once
#ifdef __cplusplus
extern "C" {
#endif
int add(int a, int b);
#ifdef __cplusplus
}
#endif

#ifndef MOTOR_DM_H
#define MOTOR_DM_H

#include <array>
#include <cstdint>
#include <string>

/*------------ Linux SocketCAN 头 ------------*/
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <iomanip>
#include <vector>
#include "serialib.h"
typedef uint8_t byte;

using Angles  = std::array<float, 6>;
using Errors  = std::array<uint8_t, 6>;
using PIDs    = std::array<float, 6>;
using Rads    = std::array<float, 6>;
using Modes   = std::array<uint8_t, 6>;
using Torques = std::array<float, 6>;
using Speeds  = std::array<float, 6>;

/*---------- 常量保持原值 ----------*/
#define J1_MAX 165
#define J1_MIN -165
#define J2_MAX 90
#define J2_MIN -90
#define J3_MAX 157
#define J3_MIN -113
#define J4_MAX 90
#define J4_MIN -90
#define J5_MAX 90
#define J5_MIN -90
#define J6_MAX 90
#define J6_MIN -90
#define J7_MIN -45
#define J7_MAX 45
#define J8_MIN -45
#define J8_MAX 45


#define SPEED_KP_ARRD 0x19
#define SPEED_KI_ARRD 0x1A
#define POS_KP_ARRD   0x1B
#define POS_KI_ARRD   0x1C
#define RUN_MODE_ADDR 0x0A
#define REGISTER_CMD  0x7FF

#define J1_MAX_SPEED_MAPPING 1.80
#define J2_MAX_SPEED_MAPPING 1.95
#define J3_MAX_SPEED_MAPPING 1.80
#define J4_MAX_SPEED_MAPPING 1.80
#define J5_MAX_SPEED_MAPPING 2.25
#define J6_MAX_SPEED_MAPPING 2.25
#define J7_MAX_SPEED_MAPPING 1.80
#define J8_MAX_SPEED_MAPPING 1.80

class Motor_dm {
public:
    Motor_dm(const char* Port);
    ~Motor_dm();

    /* 以下是临时需要使用的,需要做修改,剩下的先不改 */
    void Power_On_Single(uint8_t id);

    void Power_Off_Single(uint8_t id);

    void Power_On_All();

    void Power_Off_All();

    void Set_Calibration(uint8_t id);

    void Set_Angles_Single(uint8_t id, float angles, float speed);

    void Set_Angles_All(const std::vector<float>& angles, float speed);

    Angles Get_Angles_All();

    float Get_Angle_Single(uint8_t id);

    void Clear_Err(uint8_t id);

    uint8_t Get_Status(uint8_t id);

    void Gripper_Control(uint8_t mode);

    void Init_Serial(const char *Port);

    void Set_Rad(uint8_t id, float rad, float speed);

    float Get_Rad(uint8_t id);

    serialib serial; // 创建串口对象
    /* 以上是临时需要使用的,需要做修改,剩下的先不改 */

};

uint8_t Is_In_Position(uint8_t id, float angles);
#endif
