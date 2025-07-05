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
//extern uint8_t cmd_buffer[256];//�洢ÿ�����ݵ�����
extern  uint8_t len_buffer[8];//�洢ÿ�����ݵĳ���\

extern uint8_t len_idx;//���Ȼ�������λ��
extern uint8_t cmd_idx;//���������λ��
extern uint8_t recv_idx;//���ݻ�����
extern uint8_t send_idx;//���ͻ�����λ��
extern uint8_t len;//��צ���ص����ݳ���

extern uint8_t  read_flag; 
extern CanTxMsg TxMessage;			     //���ͽṹ��
extern CanRxMsg RxMessage;				 //���սṹ��

extern uint8_t list;
extern uint8_t list_back;

extern uint32_t tick_count;
extern int16_t over_time;
#endif


