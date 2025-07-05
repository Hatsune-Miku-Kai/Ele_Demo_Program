#ifndef __UART_H_
#define __UART_H_
#include "stm32f10x.h"                  // Device header
#include "CAN.h"

void MODBUS_USART_Init(void);
void MODBUS_Send_Data(void);
void DMA_Config(void);
void UART2_Send_Data_DMA(uint8_t *buf, uint16_t len);
void UART2_To_CAN1(uint8_t lenth);
void UART2_To_CAN1(uint8_t lenth);

extern  uint8_t recv_buffer[64];
extern  uint8_t send_buffer[64];
//extern uint8_t cmd_buffer[256];//存储每条数据的命令
extern  uint8_t len_buffer[8];//存储每条数据的长度\

extern uint8_t len_idx;//长度缓冲区的位置
extern uint8_t cmd_idx;//命令缓冲区的位置
extern uint8_t recv_idx;//数据缓冲区
extern uint8_t send_idx;//发送缓存区位置
extern uint8_t len;//夹爪返回的数据长度

extern uint8_t  read_flag; 
extern CanTxMsg TxMessage;			     //发送结构体
extern CanRxMsg RxMessage;				 //接收结构体

extern uint8_t list;
extern uint8_t list_back;

extern uint32_t tick_count;
extern int16_t over_time;
#endif


