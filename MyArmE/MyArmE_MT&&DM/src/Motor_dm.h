#ifndef MOTOR_DM_H
#define MOTOR_DM_H

#include <array>
#include <Arduino.h>

#include "driver/can.h"

using Angles = std::array<float,6>;
using Errors = std::array<uint8_t,6>;
using PID = std::array<float,6>;
using Rads = std::array<float,6>;

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

#define SPEED_KP_ARRD 0x19
#define SPEED_KI_ARRD 0x1A
#define POS_KP_ARRD 0x1B
#define POS_KI_ARRD 0X1C

#define PID_CMD 0x7FF

class Motor_dm {
public:
    Motor_dm();
    ~Motor_dm();
    
    void InitCan();

    u_char get_status(uint8_t id);
    float get_rad(uint8_t id);
    float get_angle(uint8_t id);
    float get_speed(uint8_t id);
    void power_on(uint8_t id);
    void power_off(uint8_t id);
    void set_zero(uint8_t id);
    void clear_err(uint8_t id);
    void send_rad(uint8_t id, float rad, float speed);
    void send_angle(uint8_t id, float angles, float speed);

    //封装好的接口起始

    void Power_On(uint8_t *id, uint8_t len);

    void Power_Off(uint8_t *id, uint8_t len);

    void Set_Calibration(uint8_t *id, uint8_t len);

    void Clear_Err(uint8_t *id, uint8_t len);

    void Set_Rads(uint8_t *id, uint8_t len, float *rad, float speed);

    void Set_Angles(uint8_t *id, uint8_t len, float *angles, float speed);

    Angles Get_Angles(uint8_t *id, uint8_t len);

    Rads Get_Rads(uint8_t *id, uint8_t len);

    //void Set_Rad(uint8_t id, float rad, float speed);//可以不开放

    Errors Get_Status(uint8_t *id, uint8_t len);

    PID Get_KI(uint8_t *id, uint8_t len, uint8_t mode);
    
    PID Get_KP(uint8_t *id, uint8_t len, uint8_t mode);
    
    void Change_I(uint8_t *id, float *KI, uint8_t len, uint8_t mode);

    void Change_P(uint8_t *id, float *KP, uint8_t len, uint8_t mode);

    void Save_PID(uint8_t *id, uint8_t len);

    //封装好的接口结束
};


uint8_t Is_In_Position(uint8_t *ID, uint8_t id_len, float *angles);
#endif