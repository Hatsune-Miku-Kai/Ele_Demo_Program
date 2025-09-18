#include "stm32f10x.h"                  // Device header
#include "CAN.h"
#include "DEBUG_UART.h"
#include "UART.h"
#include "GPIO.h"
#include "IAP.h"
#include "FLASH.h"
#include "TIM.h"
#include "SEGGER_RTT.h"
#include "WS2812.h"
#include "DELAY.h"

extern int16_t over_time;//超时时间,默认1s
extern uint8_t Into_APP_Flag;
extern uint8_t Into_BootLoader_Flag;

int main(void)
{
	
	SCB->VTOR = 0x08000000;	
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
	
//	SEGGER_RTT_printf(0,"HELLO WORLD\r\n");
	
	ShowAll_Color(COLOR_GREEN);
	
	if(Read_Deliver_Addr() == 0x12345678)//判断是否有新固件
		iap_load_app(FLASH_APP1_ADDR);	

	while(1)
	{
//		SEGGER_RTT_printf(0,"HELLO WORLD\r\n");

		
		if(Into_APP_Flag)
		{
			Into_APP_Flag = 0;
			Load_App(); 
		}
		

		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET)
			GPIO_SetBits(GPIOB,GPIO_Pin_10);
//		
//		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET)
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		


		
//		if(Into_BootLoader_Flag)
//		{
//			Into_BootLoader_Flag = 0;
//			Into_Bootloader(FLASH_BASE);
//		}
			
	}
}

