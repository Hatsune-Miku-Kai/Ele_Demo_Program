#include "stm32f10x.h"                  // Device header
#include "GPIO.h"
// 配置GPIO为输出模式
void GPIO_Init_Output()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置模式-推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;  //指定相应引脚  GPIOA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 设置GPIO输出电平
void GPIO_Set_Output(GPIO_TypeDef* GPIOx, uint16_t Pin, uint8_t state)
{
    if (state)
        GPIO_SetBits(GPIOx, Pin); // 输出高电平
    else
        GPIO_ResetBits(GPIOx, Pin); // 输出低电平
}



void GPIO_Init_Input()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能GPIO时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置GPIO为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// 读取GPIO输入电平
uint8_t GPIO_Read_Input(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
    return GPIO_ReadInputDataBit(GPIOx, Pin); // 读取输入电平
}

