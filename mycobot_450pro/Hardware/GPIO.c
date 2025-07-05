#include "stm32f10x.h"                  // Device header
#include "GPIO.h"
// ����GPIOΪ���ģʽ
void GPIO_Init_Output()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//����ģʽ-�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;  //ָ����Ӧ����  GPIOA0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// ����GPIO�����ƽ
void GPIO_Set_Output(GPIO_TypeDef* GPIOx, uint16_t Pin, uint8_t state)
{
    if (state)
        GPIO_SetBits(GPIOx, Pin); // ����ߵ�ƽ
    else
        GPIO_ResetBits(GPIOx, Pin); // ����͵�ƽ
}



void GPIO_Init_Input()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // ʹ��GPIOʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // ����GPIOΪ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// ��ȡGPIO�����ƽ
uint8_t GPIO_Read_Input(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
    return GPIO_ReadInputDataBit(GPIOx, Pin); // ��ȡ�����ƽ
}

