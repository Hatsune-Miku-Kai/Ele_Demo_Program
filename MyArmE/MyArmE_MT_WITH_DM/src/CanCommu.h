#ifndef CAN_COMMU_h
#define CAN_COMMU_h

#include "config.h"
#include <driver/can.h>
#include <vector>
#include "Arduino.h"

#define CAN_RX 33 
#define CAN_TX 32

#define MOTOR_NUMS 6

#define CAN_RECEIVE_TIMEOUT 10 //ms
#define CAN_RECV_RX_LEN 8 //recv can queue len
#define CAN_SEND_MSG_TIMEOUT 10 //send msg timeout
#define MAX_LOSS_COUNT 50 //continue loss 50 is communicate error

class CanCommu
{
private:
    /* data */
public:
    CanCommu(/* args */);
    ~CanCommu();

    bool InitCan(uint8_t can_rx, uint8_t can_tx, int baud); //intcan
    bool StopCan(); //stop can
    bool SendMsg(uint8_t *data, uint8_t len, int ID); //send can msg
    void ReadMsg(uint8_t *data, uint8_t len, int ID, std::pair<int, std::vector<uint8_t>> &read_buf, bool only_read = false); //recv can msg
    void InitParam();

private:
    bool can_driver_installed{false}, can_send_state{false};
    uint8_t send_err_count[MOTOR_NUMS];
};

class debug 
{
public:
    static bool debug_state; 
};

#endif