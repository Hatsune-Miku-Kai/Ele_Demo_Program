#include "stm32f10x.h"                  // Device header
#include "CAN.h"
#include "DEBUG_UART.h"
#include "UART.h"
#include "GPIO.h"
#include "IAP.h"
#include "FLASH.h"
#include "TIM.h"
#include "WS2812.h"

extern int16_t over_time;//超时时间,默认5s
extern uint8_t Into_APP_Flag;
extern uint8_t Into_BootLoader_Flag;
void loop(void);

int main(void)
{

	SCB->VTOR = 0x08003000;
	__enable_irq();
	over_time = 5000;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	CAN_Start();
	DMA_Config();
	MODBUS_USART_Init();
	DEBUG_USART_Init();
	EXTI_PB1_PB11_Start();
	
	TIM2_Init();
	TIM3_Init();
	

	//GPIO_Set_Output(GPIOA,GPIO_Pin_0,1);
	
    //printf("HELLO IAP\r\n");
	while(1)
	{
		if(Into_APP_Flag)
		{
			Into_APP_Flag = 0;
			Load_App(); 
		}

		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET)
			GPIO_SetBits(GPIOB,GPIO_Pin_10);
		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET)
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		
		if(Into_BootLoader_Flag)
		{
			Into_BootLoader_Flag = 0;
			Into_Bootloader(FLASH_BASE);
		}
			
	}
	
}
