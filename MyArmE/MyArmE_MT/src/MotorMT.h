#ifndef __MOTORMT_H__
#define __MOTORMT_H__
using namespace std;
#include <array>
#include <vector>


#define P 1
#define I 2
#define D 3

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



using Angles = array<float,6>;
using Speeds = array<int, 6>;
using Error = array<uint8_t, 6>;
using PID = array<float,6>;



class MotorMT
{
public:
    MotorMT();

    ~MotorMT();


    void Set_Motor_Angles(uint8_t *ID, uint8_t id_len, float* angles, int speed);



    Angles Get_Motor_Angles(uint8_t *ID, uint8_t id_len);


    Error Get_Motor_Errors(uint8_t *ID, uint8_t id_len);
    

    void Power_on();

    void Power_off();

    void Change_Motor_ID(uint8_t New_ID);

    void Change_Motor_PID(uint8_t *ID, uint8_t id_len, float New_P, 
                                float New_I, float New_D, uint8_t mode);
    
    Angles Get_Motor_PID(uint8_t *ID, uint8_t id_len, uint8_t mode);

    void Set_Calibration(uint8_t *ID, uint8_t id_len);

    void Stop_Run();

    uint8_t Read_Motor_ID();

    Angles Read_Calibration(uint8_t *ID, uint8_t id_len);

    uint8_t Is_In_Position(uint8_t *ID, uint8_t id_len, float *angles);

     

};

#endif

