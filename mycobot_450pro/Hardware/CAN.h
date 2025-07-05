#ifndef __CAN_H_
#define __CAN_H_
#include "stm32f10x.h"                  // Device header

#define MAX_RECV 256
#define MAX_SEND 256
#define MAX_CMD 256
#define MAX_LEN 256
#define SERVO_ID 0x0A

/* CAN串口数据结构体 */




extern uint8_t send_buffer_len;
extern uint32_t APP_LEN;
extern uint32_t last_addr;
void CAN_Start(void);
void Init_RxMes(CanRxMsg *RxMessage);
void Handle_Data(CanRxMsg* data);
void CAN_ReadBufferReset(void);
void Select_Cmd(uint8_t cmd);
void CAN_SetMsg(CanTxMsg *TxMessage, uint8_t *TxData,uint8_t len,uint16_t ID);
void Default_Return(void);

#endif
