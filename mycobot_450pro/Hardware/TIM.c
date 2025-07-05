#include "TIM.h"

volatile uint32_t tick_count = 0; // 每100ms增加1

// 初始化定时器2，每100ms触发一次中断
void TIM2_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能定时器2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置定时器2
    TIM_TimeBaseStructure.TIM_Period = 99; // 自动重装载值，定时器计数到9999时溢出
    TIM_TimeBaseStructure.TIM_Prescaler = 7199; // 预分频器，72MHz / (7200) = 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 使能定时器2的更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器2
    //TIM_Cmd(TIM2, ENABLE);
}

// 定时器2中断处理函数
void TIM2_IRQHandler(void) 
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
        tick_count++; // 每100ms增加1
    }
	if (RxMessage.Data[1] == 0XB5 && (read_flag == 1) && USART_GetFlagStatus(USART2, USART_FLAG_IDLE) == RESET && Is_Over_Time(over_time) == 1)
	{
			uint8_t send_buffer[3]={0x02,0x0A,0};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);	
			CAN_ReadBufferReset();
	}
}

uint8_t Is_Over_Time(int16_t over_time)
{
	int16_t time = over_time;
	if(tick_count * 10 > time)
		return 1;
	else return 0;
}

