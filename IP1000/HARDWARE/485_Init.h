#ifndef __485__INIT__H__
#define __485__INIT__H__

#include <stm32f4xx.h>
#include <cstdint>
#define IO_NUM 32


void USART1_Init(int BAUD_RATE);
uint16_t Check_CRC_Serial(uint8_t *data, uint16_t length, uint8_t is_high_first);
void Handle_USART1_Data(uint8_t* data, uint8_t len);
void Handle_USART1_Cmd(uint8_t cmd, uint8_t* data, uint8_t len);
uint8_t Read_GPIO_Input(GPIO_TypeDef* GPIO_Channel, uint16_t Bit);
void Set_GPIO_Output(GPIO_TypeDef* GPIO_Channel, uint16_t Bit, uint8_t Status);
void DMA_Send_Data(uint8_t *data, uint16_t len);
void DMA_Config(void);

#endif

