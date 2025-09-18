#include "stm32f10x.h"                  // Device header
#include "GPIO.h"
#include "CAN.h"


volatile uint8_t pb1_flag = 0;
volatile uint8_t pb11_flag = 0;

void TIM3_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_InitStruct.TIM_Period = 49;         // 根据频率调节消抖时间
    TIM_InitStruct.TIM_Prescaler = 7199;      // 72MHz / 7200 = 10kHz
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_InitStruct);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


// 配置GPIO为输出模式
void GPIO_Init_Output()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置模式-推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10 | GPIO_Pin_2 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度

    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//配置模式-推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

void EXTI_Mode_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);   // PB1 -> EXTI1
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);  // PB11 -> EXTI11

    /* EXTI1 (PB1) */
    EXTI_InitStructure.EXTI_Line    = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 可改 Rising
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* EXTI11 (PB11) */
    EXTI_InitStructure.EXTI_Line    = EXTI_Line11;
    EXTI_Init(&EXTI_InitStructure);
}


void EXTI_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    /* EXTI1 */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    /* EXTI11 共用 EXTI15_10_IRQn */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_InitStructure);
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置GPIO为浮空输入模式
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//配置模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 读取GPIO输入电平
uint8_t GPIO_Read_Input(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
    return GPIO_ReadInputDataBit(GPIOx, Pin); // 读取输入电平
}



void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {

        /* 处理 PB1 中断 */
        EXTI_ClearITPendingBit(EXTI_Line1);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET)
        {
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			pb1_flag = 1;
			
			TIM_Cmd(TIM3, DISABLE);                     // 停止定时器
            TIM_SetCounter(TIM3, 0);                    // 清零计数器
            TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清中断标志
			TIM_Cmd(TIM3, ENABLE);   // 启动定时器消抖			
        }

    }
}


void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        /* 处理 PB11 中断 */
        EXTI_ClearITPendingBit(EXTI_Line11);

        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET)
        {
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			pb11_flag = 1;
            TIM_Cmd(TIM3, DISABLE);                     // 停止定时器
            TIM_SetCounter(TIM3, 0);                    // 清零计数器
            TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清中断标志
			TIM_Cmd(TIM3, ENABLE);   // 启动定时器消抖
        }		

    }
}


void EXTI_PB1_PB11_Start(void)
{
    GPIO_Init_Input();			// 1. 时钟
	GPIO_Init_Output();            
    EXTI_Mode_Config();         // 2. EXTI
    EXTI_NVIC_Config();         // 3. NVIC
}


void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        TIM_Cmd(TIM3, DISABLE); // 停止定时器

//        if (pb1_flag && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET) {
//            uint8_t buf[2] = {0x01,0x01};
//            CAN_SetMsg(&TxMessage, buf, sizeof(buf), SERVO_ID);
//            CAN_Transmit(CAN1, &TxMessage);
//            CAN_ReadBufferReset();
//        }
//        if (pb11_flag && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET) {
//            uint8_t buf[2] = {0x02,0x01};
//            CAN_SetMsg(&TxMessage, buf, sizeof(buf), SERVO_ID);
//            CAN_Transmit(CAN1, &TxMessage);
//            CAN_ReadBufferReset();
//        }

        pb1_flag = pb11_flag = 0;
    }
}
