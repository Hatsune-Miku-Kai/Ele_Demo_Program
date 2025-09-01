#include "Motor_dm.h"
#include "SD100.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>

Motor_dm DM_R("/dev/ttyACM2");
Motor_dm DM_L("/dev/ttyACM1");
SD100 sd("/dev/ttyACM0");
uint8_t motorIDs[] = {1,2,3,4,5,6} ;// 6个电机的ID
uint8_t speedIDs[] = {6};
Angles angles;
Speeds speed;


float Zero[] = {0,0,0,0,0,0};


const std::vector<float> J1_angles1 = {0,60,-30,0,30,60,10};       const std::vector<float> J1_angles2 = {0,60,-15,0,-30,60};


const std::vector<float> J2_angles1 = {-40,80,-15,0,30,0};      const std::vector<float> J2_angles2 = {40,80,-15,0,-30,0};   


const std::vector<float> J3_angles1 = {0,88,0,0,0,0,0};         const std::vector<float> J3_angles2 = {0,88,0,0,0,0};


const std::vector<float> J4_angles1 = {-140,88,0,0,0,0};         const std::vector<float> J4_angles2 = {140,88,0,0,0,0};


const std::vector<float> J5_angles1 = {120,88,0,0,0,0};         const std::vector<float> J5_angles2 = {-120,88,0,0,0,0};


const std::vector<float> J6_angles1 = {0,0,0,0,0,0};         const std::vector<float> J6_angles2 = {0,0,0,0,0,0};


uint8_t Serial_Send_Buffer_CAN_1[30] = {0x55, 0xAA, 0x1E, 0x01, 0X01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xFF,
                                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x88}; // 第13位   

extern float liju;
float speeds[] = {1};
char recv_buffer[256];
int timeout_ms = 2000;

void IoInit();
void Get_error();
void Get_All_error();
int main()
{

    // DM.Clear_Err(3);

    // usleep(1000);
    // DM.Set_Calibration(1);
    // sleep(1);
// DM.Power_On_All();
// DM_R.Power_Off_All();
// DM_L.Power_Off_All();
// sleep(10);

// for(int i = 1 ; i < 7 ; i++)
// {
//     //DM_R.Clear_Err(i);
//     DM_L.Clear_Err(i);
// }
// sleep(2);

// //DM_R.Power_On_All();
// DM_L.Power_On_All();
// sleep(2);

//DM_R.Set_Angles_Single(1,0,10);
DM_L.Set_Angles_Single(5,0,10);
DM_L.Set_Angles_Single(3,0,10);
DM_L.Set_Angles_Single(2,0,10);
DM_L.Get_Status(5);
DM_L.Get_Status(3);

// sleep(10);
// DM.Set_Calibration(1);
// DM.Set_Calibration(2);
// DM.Set_Calibration(3);
// DM.Set_Calibration(4);
// DM.Set_Calibration(5);
// DM.Set_Calibration(6);
// DM.Power_On_All();

    // while(1)
    // {
    // sd.Set_Pos(1,200,300,200,200);
    // DM_R.Set_Angles_All(J1_angles1,20);
    // DM_L.Set_Angles_Single(7,30,10);
    // DM_L.Set_Angles_Single(8,20,10);
    // DM_L.Set_Angles_All(J1_angles2,20);
    // sleep(15);

    // sd.Set_Pos(1,0,300,200,200);
    // DM_R.Set_Angles_All(J2_angles1,20);
    // DM_L.Set_Angles_Single(7,-30,10);
    // DM_L.Set_Angles_Single(8,-10,10);
    // DM_L.Set_Angles_All(J2_angles2,20);
    // sleep(15);

    // sd.Set_Pos(1,200,300,200,200);
    // DM_R.Set_Angles_All(J3_angles1,20);
    // DM_L.Set_Angles_Single(7,30,10);
    // DM_L.Set_Angles_Single(8,20,10);
    // DM_L.Set_Angles_All(J3_angles2,20);
    // sleep(15);
    
    // sd.Set_Pos(1,0,300,200,200);
    // DM_R.Set_Angles_All(J4_angles1,20);
    // DM_L.Set_Angles_Single(7,-30,10);
    // DM_L.Set_Angles_Single(8,-10,10);
    // DM_L.Set_Angles_All(J4_angles2,20);
    // sleep(15);

    // sd.Set_Pos(1,200,300,200,200);
    // DM_R.Set_Angles_All(J5_angles1,20);
    // DM_L.Set_Angles_Single(7,30,10);
    // DM_L.Set_Angles_Single(8,20,10);
    // DM_L.Set_Angles_All(J5_angles2,20);
    // sleep(20);
    
    // sd.Set_Pos(1,0,300,200,200);
    // DM_R.Set_Angles_All(J6_angles1,20);
    // DM_L.Set_Angles_Single(7,-30,10);
    // DM_L.Set_Angles_Single(8,-10,10);
    // DM_L.Set_Angles_All(J6_angles2,20);
    // sleep(15);


    // }

}
