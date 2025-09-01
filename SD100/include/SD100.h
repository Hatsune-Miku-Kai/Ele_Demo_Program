#ifndef __SD100_H__
#define __SD100_H__



#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <iomanip>

#define POS_ADDR 0X607A
#define SPEED_ADDR 0x6081
#define ACC_ADDR 0x6083
#define DEC_ADDR 0x6084
#define MODE_ADDR 0x6060
#define CONTROL_ARRD 0x6040


class SD100
{


public:
    SD100();
    ~SD100();

    void Power_On(uint8_t ID);
    void Power_Off(uint8_t ID);

    void Set_Pos(uint8_t ID, int32_t len, uint32_t speed, uint32_t acc, uint32_t dec);
    void Set_Mode(uint8_t ID, uint8_t mode);
    void Set_Speed(uint8_t ID, uint32_t speed);

    void Set_Acc(uint8_t ID, uint32_t acc);
    void Set_Dec(uint8_t ID, uint32_t dec);

    int32_t Read_Pos(uint8_t ID);
    void Run(uint8_t ID);

    void Stop(uint8_t ID);

    void Set_Up();

    void Clear_Err(uint8_t ID);
    
    void Set_Calibration(uint8_t ID);

    // int32_t Read_Offset(uint8_t ID);
};

#endif