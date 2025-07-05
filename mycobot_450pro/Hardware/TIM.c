#include "TIM.h"

volatile uint32_t tick_count = 0; // ÿ100ms����1

// ��ʼ����ʱ��2��ÿ100ms����һ���ж�
void TIM2_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ�ܶ�ʱ��2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // ���ö�ʱ��2
    TIM_TimeBaseStructure.TIM_Period = 99; // �Զ���װ��ֵ����ʱ��������9999ʱ���
    TIM_TimeBaseStructure.TIM_Prescaler = 7199; // Ԥ��Ƶ����72MHz / (7200) = 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // ʹ�ܶ�ʱ��2�ĸ����ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // �����ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ������ʱ��2
    //TIM_Cmd(TIM2, ENABLE);
}

// ��ʱ��2�жϴ�����
void TIM2_IRQHandler(void) 
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // ����жϱ�־λ
        tick_count++; // ÿ100ms����1
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

