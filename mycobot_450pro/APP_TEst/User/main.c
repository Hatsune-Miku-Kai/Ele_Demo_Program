#include "stm32f10x.h"                  // Device header
#include "CAN.h"
#include "DEBUG_UART.h"
#include "UART.h"
#include "GPIO.h"
#include "IAP.h"
#include "FLASH.h"
#include "TIM.h"
#include "WS2812.h"

extern int16_t over_time;//超时时间,默认1s
extern uint8_t Into_APP_Flag;
extern uint8_t Into_BootLoader_Flag;


int main(void)
{
    __disable_irq();
    SCB->VTOR = 0x08003000;

    over_time = 5000;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    CAN_Start();
	
    MODBUS_USART_Init();
	DMA_Config();
	
    //DEBUG_USART_Init();
    EXTI_PB1_PB11_Start();

    TIM2_Init();
    TIM3_Init();

    __enable_irq();



	Write_Deliver_Addr(DELIVER_MESSAGE);
    ShowAll_Color(COLOR_GREEN);
	
    while (1)
    {
        if (Into_BootLoader_Flag) {
            Into_BootLoader_Flag = 0;
            Into_Bootloader(FLASH_BASE);
        }

		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET)
			GPIO_SetBits(GPIOB,GPIO_Pin_10);

		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_SET)
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
    }
}




