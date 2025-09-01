#ifndef __MOTORCMD_H__
#define __MOTORCMD_H__

#define SINGLE_MOTOR_CMD 0x140
#define ALL_MOTOR_CMD 0x280


#define READ_ANGLE 0x92 // Read the angle
#define SET_ANGLE  0xA4  // Set the angle for a single loop
#define READ_ERROR 0x9A  // Read the error of motor
#define POWER_ON 0x77 
#define POWER_OFF 0x80
#define CHANGE_Motor_ID 0x79
#define CHANGE_Motor_PID 0x31
#define SET_CALIBRATION 0x64
#define STOP_RUN 0x81
#define READ_CALIBRATION 0x62
#define RESET 0x76
#define READ_MOTOR_PID 0x30



#endif
