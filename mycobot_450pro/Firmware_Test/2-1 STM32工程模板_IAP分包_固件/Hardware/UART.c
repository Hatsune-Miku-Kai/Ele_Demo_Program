#include "UART.h"
#include "DEBUG_UART.h"
#include "GPIO.h"
#include "string.h"
#include "TIM.h"

uint8_t send_buffer_len;


//�жϺ���
void USART1_IRQHandler(void)
{
if (USART_GetFlagStatus(USART1, USART_FLAG_IDLE) == SET) // �������жϱ�־
    {

		//GPIO_Set_Output(GPIOA,GPIO_Pin_0,1);
		USART1->SR;
		USART1->DR;

//		printf("%d",tick_count);
		// ��������жϱ�־
		//printf("IDLE");
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
		// ֹͣ DMA ����
		uint16_t remaining = DMA_GetCurrDataCounter(DMA1_Channel5);
		//printf("Remaining data: %u\n", remaining);
		
		// ��ȡ���յ������ݳ���
		gripper_len = 1024 - DMA_GetCurrDataCounter(DMA1_Channel5);
//		printf("%d",gripper_len);
//		for(int i = 0; i< len ;i++)
//			printf("%02x ",send_buffer[i]);
	
		UART1_To_CAN1(gripper_len);	
		
		// �������� DMA ����
		DMA_SetCurrDataCounter(DMA1_Channel5, 1024);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		

		
		if(Is_Over_Time(over_time))
		{
			printf("Over time\n\r");
			memset(send_buffer, 0, sizeof(send_buffer));
			gripper_len = 0;//�����Ҫ���͵��ֽ���
		}
		
		TIM_Cmd(TIM2,DISABLE);
		CAN_ReadBufferReset();

		//���ÿ����ж�
        //USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
    }
}



/**
 * @brief ����DMAͨ������USART1����
 * 
 * ����DMA1ͨ��4����USART1�ķ��͹��ܡ�����DMA�������ַΪUSART1�����ݼĴ�����
 * ������Ϊ�ڴ浽����Ĵ���ģʽ��
 */
void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. ʹ�� DMA ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 2. ���� DMA ͨ�� 4����Ӧ USART1_TX��
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; // USART1 DR ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)recv_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // �ڴ浽����
    DMA_InitStructure.DMA_BufferSize = 0; // ���ݴ�С���ڷ���ʱ����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	
	DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; // USART1 DR ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)send_buffer; // ���ջ�������ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ���赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = 1024; // ��������С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    // 7. ʹ�� DMA ͨ��
	DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    // 4. ���� NVIC
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ���� MODBUS �ж�����
  * @param  ��
  * @retval ��
  */
void MODBUS_NVIC_Config(void)
{
    /* ����һ�� NVIC �ṹ�� */
    NVIC_InitTypeDef nvic_initstruct = {0};
    
    /* ���� AFIO ��ص�ʱ�� */
    //RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
    /* �����ж�Դ */
    nvic_initstruct.NVIC_IRQChannel                     = USART1_IRQn;
    /* ������ռ���ȼ� */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
    /* ���������ȼ� */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* ʹ�������ж�ͨ�� */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
	
	

}


/**
 * @brief  ��ʼ������ MODBUS ��IO
 * @param  ��
 * @retval ��
 */
void MODBUS_USART_PinConfig(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. ʹ�� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. ���� USART1 TX ���� (PA9)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���츴��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ���� USART1 RX ���� (PA10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
}

/**
 * @brief  ���� MODBUS
 * @param  ��
 * @retval ��
 */
void MODBUS_USART_ModeConfig(int32_t Baud_Rate)
{
  
    USART_InitTypeDef USART_InitStructure;
	USART_DeInit(USART1);	//�����ڻָ���Ĭ��״̬
    // 1. ʹ�� USART1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 2. ���� USART1 ����
    USART_InitStructure.USART_BaudRate = Baud_Rate; // ������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // ����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No; // ��У��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ���ͺͽ���ģʽ
    USART_Init(USART1, &USART_InitStructure);

    // 3. ʹ�� USART1 DMA ��������
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    // 4. ʹ�� USART1
    USART_Cmd(USART1, ENABLE);
}

void MODBUS_USART_Init(void)
{
    MODBUS_NVIC_Config();
    
    MODBUS_USART_ModeConfig(115200);

    MODBUS_USART_PinConfig();
    
    USART_Cmd(USART1,ENABLE);
}

 /** USART2����len���ֽ�.
  * buf:�������׵�ַ
  * len:���͵��ֽ���
  */
void UART1_Send_Data_DMA(uint8_t *buf, uint16_t len)
{
    // ���� DMA ͨ��
    DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,len);
    // ʹ�� DMA ͨ��
    DMA_Cmd(DMA1_Channel4, ENABLE);
	while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);	//�ȴ�DMA�������
	DMA_ClearFlag(DMA1_FLAG_TC4);	
	//printf("DMA");
	send_buffer_len = 0; 	//���������ɱ�־λ
}


void MODBUS_Send_Data()
{
	if(read_flag)
	{

		for(int i = 0; i < list_back; i++)
		{
			send_buffer_len += len_buffer[i];//���㱾����Ҫ���Ͷ�������
		}
		
//		printf("%d\r\n",send_buffer_len);
//		for(int i = 0; i < send_buffer_len; i++)
//			printf("%02x ",recv_buffer[i]);
		UART1_Send_Data_DMA(recv_buffer, send_buffer_len);

	}
}


void UART1_To_CAN1(uint8_t lenth)
{
	//printf("send");
	if(lenth > 6)
	{
		uint8_t templata_buffer[8] = {0x07,0xb4,0,0,0,0,0,0};
		uint8_t list = 0; //������ݴ���5,��ζ��һ������ʱһ���������������
		int8_t lenth_temp = lenth;//��Ҫ���з��������
		//printf("%d",lenth_temp);
		while(1)
		{	
			if(lenth_temp > 0)
			{
				lenth_temp = lenth_temp - 5;
				//printf("%d\n\r",lenth_temp);
				list++;
				//printf("%d\n\r",list);
			}
			else break;

		}
		//uint8_t last = lenth % 5;
		uint8_t first = 0;
	
		for(int i = list; i > 0; i--)
		{	
			templata_buffer[0] = (lenth > 5 ? 7 : (lenth + 2));
			templata_buffer[2] = i;
			for(int j = 0; j < 5; j++)
			{
				templata_buffer[3 + j] = send_buffer[first];
				first ++;//round1:0-4 round2 5-9 round3:10-14 
			}
			CAN_SetMsg(&TxMessage,templata_buffer ,(lenth > 5 ? 8 : (lenth + 3)),SERVO_ID); //��������ֽ�������5��,�ͷ�������CAN����,
																						//���С��5��,��Ϊ��ǰȷ����������ͳ��Ⱥ����,�����Ҫ + 3
			CAN_Transmit(CAN1,&TxMessage);
			lenth -= 5;
		}
//		CAN_SetMsg(&TxMessage,templata_buffer + first ,last,0x002);
//		CAN_Transmit(CAN1,&TxMessage);
	}
	
	if(lenth <= 6)//���ؿ϶�ֻ��һ��,һ���Ļ�����Ҫ����Ҫ�����Ч���ݿ��������6���ֽ�
	{
		uint8_t templata_buffer[8] = {0x07,0xb4,0,0,0,0,0,0};
		templata_buffer[0] = (lenth == 6 ? 7 : lenth + 1);
		for(int i = 0; i < lenth; i++)
		{
			templata_buffer[2 + i] = send_buffer[i];
		}
		CAN_SetMsg(&TxMessage,templata_buffer ,lenth + 2,SERVO_ID);
		CAN_Transmit(CAN1,&TxMessage);
	}
	CAN_ReadBufferReset();
	memset(send_buffer, 0, sizeof(send_buffer));
	gripper_len = 0;//�����Ҫ���͵��ֽ���
}


