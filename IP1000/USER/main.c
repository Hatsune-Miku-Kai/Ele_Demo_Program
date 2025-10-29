#include "GPIO.h"
#include "485_Init.h"
#include "SEGGER_RTT.h"
#include "delay.h"

uint8_t test[5] = {0xfe, 0xfe, 0x05, 0x01,0xfa};

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	GPIO_Digital_Init();
	USART1_Init(115200);
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,1);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,1);

	while(1)
	{
//		DMA_Send_Data(test,5);
//		delay_ms(1000);
		//USART_SendData(USART1,0XFE);
		
	}
}

