#ifndef __TIM_H_
#define __TIM_H_
#include "stm32f10x.h"
#include "CAN.h"
void TIM2_Init(void);
uint8_t Is_Over_Time(int16_t over_time);
extern int16_t over_time;

extern CanTxMsg TxMessage;			     //���ͽṹ��
extern CanRxMsg RxMessage;				 //���սṹ��
extern uint8_t  read_flag; 
#endif


