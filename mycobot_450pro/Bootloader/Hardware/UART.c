#include "UART.h"
#include "DEBUG_UART.h"
#include "GPIO.h"
#include "string.h"
#include "TIM.h"

uint8_t send_buffer_len;


//中断函数
void USART1_IRQHandler(void)
{
if (USART_GetFlagStatus(USART1, USART_FLAG_IDLE) == SET) // 检查空闲中断标志
    {

		//GPIO_Set_Output(GPIOA,GPIO_Pin_0,1);
		USART1->SR;
		USART1->DR;

//		printf("%d",tick_count);
		// 清除空闲中断标志
		//printf("IDLE");
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
		// 停止 DMA 接收
		uint16_t remaining = DMA_GetCurrDataCounter(DMA1_Channel5);
		//printf("Remaining data: %u\n", remaining);
		
		// 获取接收到的数据长度
		gripper_len = 1024 - DMA_GetCurrDataCounter(DMA1_Channel5);
//		printf("%d",gripper_len);
//		for(int i = 0; i< len ;i++)
//			printf("%02x ",send_buffer[i]);
	
		UART1_To_CAN1(gripper_len);	
		
		// 重新启动 DMA 接收
		DMA_SetCurrDataCounter(DMA1_Channel5, 1024);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		

		
		if(Is_Over_Time(over_time))
		{
			printf("Over time\n\r");
			memset(send_buffer, 0, sizeof(send_buffer));
			gripper_len = 0;//清除需要发送的字节数
		}
		
		TIM_Cmd(TIM2,DISABLE);
		CAN_ReadBufferReset();

		//禁用空闲中断
        //USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
    }
}



/**
 * @brief 配置DMA通道用于USART1发送
 * 
 * 配置DMA1通道4用于USART1的发送功能。设置DMA的外设地址为USART1的数据寄存器，
 * 并配置为内存到外设的传输模式。
 */
void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能 DMA 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 2. 配置 DMA 通道 4（对应 USART1_TX）
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; // USART1 DR 地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)recv_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 内存到外设
    DMA_InitStructure.DMA_BufferSize = 0; // 数据大小将在发送时设置
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
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; // USART1 DR 地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)send_buffer; // 接收缓冲区地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设到内存
    DMA_InitStructure.DMA_BufferSize = 1024; // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

    // 7. 使能 DMA 通道
	DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    // 4. 配置 NVIC
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  配置 MODBUS 中断配置
  * @param  无
  * @retval 无
  */
void MODBUS_NVIC_Config(void)
{
    /* 定义一个 NVIC 结构体 */
    NVIC_InitTypeDef nvic_initstruct = {0};
    
    /* 开启 AFIO 相关的时钟 */
    //RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
    /* 配置中断源 */
    nvic_initstruct.NVIC_IRQChannel                     = USART1_IRQn;
    /* 配置抢占优先级 */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
    /* 配置子优先级 */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* 使能配置中断通道 */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
	
	

}


/**
 * @brief  初始化控制 MODBUS 的IO
 * @param  无
 * @retval 无
 */
void MODBUS_USART_PinConfig(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置 USART1 TX 引脚 (PA9)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推挽复用
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置 USART1 RX 引脚 (PA10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
}

/**
 * @brief  配置 MODBUS
 * @param  无
 * @retval 无
 */
void MODBUS_USART_ModeConfig(int32_t Baud_Rate)
{
  
    USART_InitTypeDef USART_InitStructure;
	USART_DeInit(USART1);	//将串口恢复到默认状态
    // 1. 使能 USART1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 2. 配置 USART1 参数
    USART_InitStructure.USART_BaudRate = Baud_Rate; // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; // 无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 发送和接收模式
    USART_Init(USART1, &USART_InitStructure);

    // 3. 使能 USART1 DMA 发送请求
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    // 4. 使能 USART1
    USART_Cmd(USART1, ENABLE);
}

void MODBUS_USART_Init(void)
{
    MODBUS_NVIC_Config();
    
    MODBUS_USART_ModeConfig(115200);

    MODBUS_USART_PinConfig();
    
    USART_Cmd(USART1,ENABLE);
}

 /** USART2发送len个字节.
  * buf:发送区首地址
  * len:发送的字节数
  */
void UART1_Send_Data_DMA(uint8_t *buf, uint16_t len)
{
    // 禁用 DMA 通道
    DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,len);
    // 使能 DMA 通道
    DMA_Cmd(DMA1_Channel4, ENABLE);
	while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);	//等待DMA工作完成
	DMA_ClearFlag(DMA1_FLAG_TC4);	
	//printf("DMA");
	send_buffer_len = 0; 	//清除工作完成标志位
}


void MODBUS_Send_Data()
{
	if(read_flag)
	{

		for(int i = 0; i < list_back; i++)
		{
			send_buffer_len += len_buffer[i];//计算本次需要发送多少数据
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
		uint8_t list = 0; //如果数据大于5,意味着一定返回时一定会有两条或更多
		int8_t lenth_temp = lenth;//需要用有符号数表达
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
			CAN_SetMsg(&TxMessage,templata_buffer ,(lenth > 5 ? 8 : (lenth + 3)),SERVO_ID); //如果数据字节数大于5个,就发送完整CAN报文,
																						//如果小于5个,因为提前确定好了命令和长度和序号,因此需要 + 3
			CAN_Transmit(CAN1,&TxMessage);
			lenth -= 5;
		}
//		CAN_SetMsg(&TxMessage,templata_buffer + first ,last,0x002);
//		CAN_Transmit(CAN1,&TxMessage);
	}
	
	if(lenth <= 6)//返回肯定只有一条,一条的话不需要加需要因此有效数据可以有最多6个字节
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
	gripper_len = 0;//清除需要发送的字节数
}


