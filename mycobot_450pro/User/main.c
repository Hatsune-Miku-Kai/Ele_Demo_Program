#include "stm32f10x.h"                  // Device header
#include "CAN.h"
#include "DEBUG_UART.h"
#include "UART.h"
#include "GPIO.h"
#include "IAP.h"
#include "FLASH.h"
#include "TIM.h"
extern int16_t over_time;//超时时间,默认1s


int main(void)
{
	__enable_irq();
	over_time = 1000;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	CAN_Start();
	DMA_Config();
	MODBUS_USART_Init();
	DEBUG_USART_Init();
	GPIO_Init_Input();
	GPIO_Init_Output();
	

	GPIO_Set_Output(GPIOA,GPIO_Pin_0,1);
	TIM2_Init();
	
	printf("HELLO WORLD\r\n");
	while(1)
	{}
}

