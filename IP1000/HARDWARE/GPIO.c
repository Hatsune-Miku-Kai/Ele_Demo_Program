#include <stm32f4xx.h>

#include "GPIO.h"

void GPIO_Digital_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA |
        RCC_AHB1Periph_GPIOB |
        RCC_AHB1Periph_GPIOC |
        RCC_AHB1Periph_GPIOE,
        ENABLE
    );

    /* 输入口配置*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;       // 输入模式
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   // 无上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // PE2~PE6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // PC13~PC15、PC0~PC3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // PA1~PA4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 输出口配置*/
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      // 输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 推挽输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PC4~PC5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // PB0~PB1, PB10~PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // PE7~PE15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
                                  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
                                  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}


