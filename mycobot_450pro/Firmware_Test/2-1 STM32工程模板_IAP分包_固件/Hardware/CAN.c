#include "CAN.h"
#include "DEBUG_UART.h"
#include "UART.h"
#include <string.h>
#include "GPIO.h"
#include "IAP.h"
#include "TIM.h"
#include "WS2812.h"

uint8_t global_cmd = 0;

extern uint32_t tick_count;
int16_t over_time = 0;
uint8_t recv_buffer[64];
uint8_t send_buffer[64];
//uint8_t cmd_buffer[256];//存储每条数据的命令
uint8_t len_buffer[8];//存储每条数据的长度

uint8_t len_idx = 0;//长度缓冲区的位置
uint8_t cmd_idx = 0;//命令缓冲区的位置
uint8_t recv_idx = 0;//数据缓冲区
uint8_t send_idx = 0;//发送缓存区位置
uint8_t gripper_len = 0; //夹爪返回的数据长度
uint8_t list_back = 0;
//uint8_t list = 0;
uint8_t IAP = 0;
uint8_t read_flag;  
CanTxMsg TxMessage;			     //发送结构体
CanRxMsg RxMessage;				 //接收结构体
uint8_t Into_APP_Flag = 0;
uint8_t Into_BootLoader_Flag = 0;
void CAN_GPIO_Init()
{
	GPIO_InitTypeDef gpio_initstruct = {0};
    
    /* 开启 CAN 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    /* 使能AFIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽复用、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_9;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_initstruct); 
      

    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入/浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_8;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_initstruct); 
}

void CAN_Mode_Init()
{
    CAN_InitTypeDef can_initstruct = {0};
    
    /* 开启 CAN 相关的GPIO外设/端口时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

	/* 配置CAN的工作参数 */
	can_initstruct.CAN_TTCM=DISABLE;                //MCR-TTCM  关闭时间触发通信模式使能
	can_initstruct.CAN_ABOM=DISABLE;			        //MCR-ABOM  自动离线管理 
	can_initstruct.CAN_AWUM=DISABLE;			        //MCR-AWUM  使用自动唤醒模式
	can_initstruct.CAN_NART=DISABLE;			    //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	can_initstruct.CAN_RFLM=DISABLE;			    //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	can_initstruct.CAN_TXFP=DISABLE;			    //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	can_initstruct.CAN_Mode = CAN_Mode_Normal;      //正常工作模式
	can_initstruct.CAN_SJW=CAN_SJW_1tq;		        //BTR-SJW 重新同步跳跃宽度 1个时间单元
	 
	/* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+5)  */
	can_initstruct.CAN_BS1=CAN_BS1_5tq;		        //BTR-TS1 时间段1 占用了5个时间单元
	can_initstruct.CAN_BS2=CAN_BS2_3tq;		        //BTR-TS1 时间段2 占用了3个时间单元	
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB1 = 36 MHz) */
	can_initstruct.CAN_Prescaler =4;		        //BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+5+3)/4=1 Mbps
	
    /* CAN初始化 */
    CAN_Init(CAN1, &can_initstruct);

    /* CAN通信中断使能 */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN_Filter_Config(void)
{
    CAN_FilterInitTypeDef can_filterinitstructure = {0};

    // 过滤器 0，使用 32-bit，掩码模式（IdMask）用于精确匹配标准 ID
    can_filterinitstructure.CAN_FilterNumber = 0;
    can_filterinitstructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filterinitstructure.CAN_FilterScale = CAN_FilterScale_32bit;

    // 对于 32-bit: 标准 ID (11-bit) 放在 FilterIdHigh 的 [15:5] 位
    can_filterinitstructure.CAN_FilterIdHigh = (0x0A << 5) & 0xFFFF; // 要接受的 ID
    can_filterinitstructure.CAN_FilterIdLow  = 0x0000;

    // Mask：在高半字中，ID 占 [15:5]，将这些位全部置 1 表示“必须完全匹配ID”
    // 0x7FF << 5 = 0xFFE0，用作高半字掩码；低半字不关心（设0）
    can_filterinitstructure.CAN_FilterMaskIdHigh = 0xFFE0;
    can_filterinitstructure.CAN_FilterMaskIdLow  = 0x0000;

    can_filterinitstructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    can_filterinitstructure.CAN_FilterActivation = ENABLE;

    CAN_FilterInit(&can_filterinitstructure);
}

void CAN_NVIC_Init()
{
	/* 定义一个 NVIC 结构体 */
    NVIC_InitTypeDef nvic_initstruct = {0};
    
    /* 开启 AFIO 相关的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
      
    /* 配置中断源 */
    nvic_initstruct.NVIC_IRQChannel                     = USB_LP_CAN1_RX0_IRQn;
    /* 配置抢占优先级 */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  0;
    /* 配置子优先级 */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* 使能配置中断通道 */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
}

/**
  * @brief  CAN通信报文内容设置,设置一个数据内容为0-7的数据包
  * @param  TxMessage: 指向要初始化的发送报文结构体
  * @param  TxData: 存放要发送数据的指针，最多只能发8个字节
  * @retval 无
  */
void CAN_SetMsg(CanTxMsg *TxMessage, uint8_t *TxData,uint8_t len,uint16_t ID)
{	  
    uint8_t ubCounter = 0;
	
	TxMessage->StdId=ID;					 	 //使用标准ID
    TxMessage->IDE=CAN_ID_STD;			         //扩展模式
    TxMessage->RTR=CAN_RTR_DATA;				 //发送的是数据
    TxMessage->DLC=len;							 //数据长度为8字节

    /*设置要发送的数据0-7*/
    for (ubCounter = 0; ubCounter < len; ubCounter++)
    {
        TxMessage->Data[ubCounter] = TxData[ubCounter];
    }
}



/**
  * @brief  初始化 Rx Message数据结构体
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval 无
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
    uint8_t ubCounter = 0;


    /*接收数据缓冲区清零*/
    for (ubCounter = 0; ubCounter < RxMessage->DLC; ubCounter++)
    {
        RxMessage->Data[ubCounter] = 0x00;
    }
	
	/*把接收结构体清零*/
    RxMessage->StdId = 0x00;
    RxMessage->ExtId = 0x00;
    RxMessage->IDE = CAN_ID_STD;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
}


/**
  * @brief  CAN 接收缓冲区复位
  * @param  无
  * @retval 无
  */
void CAN_ReadBufferReset(void)
{
    //清除接收结构体和接收数据缓冲区
	Init_RxMes(&RxMessage);
	memset(recv_buffer, 0, 8);
	memset(len_buffer, 0, 8);
	recv_idx = 0;
	cmd_idx = 0;
	len_idx = 0;
	read_flag = 0;
	//send_buffer_len = 0;
	list_back = 0;


}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	read_flag = 0;
	/*从邮箱中读出报文*/
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	if (IAP)
	{
		USER_IAP();
		//CAN_ReadBufferReset();
	}
	
	
	/* 是否是IAP模式下接收数据 */ 
	else if( (RxMessage.Data[0] + 1 == RxMessage.DLC) &&(IAP == 0))
	{
		global_cmd = RxMessage.Data[1];
//		for(int i = 0 ; i < RxMessage.DLC; i++)
//			printf("%02x ",RxMessage.Data[i]);
//		printf("\n\r");
		
		if(RxMessage.Data[1] != 0XB5)
		{	
			for(int i = 0; i < RxMessage.DLC; i++ )//DLC:02  DATA:01 09
				recv_buffer[recv_idx++] = RxMessage.Data[i];
			Select_Cmd(global_cmd);
		}
		
		else			
			Handle_Data(&RxMessage);
	}
		
	else
	{
		uint8_t send_buffer[3]={0x02,RxMessage.Data[1],0x00};
		CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
		CAN_Transmit(CAN1,&TxMessage);
		//printf("error recv");		
	}

 
}

void CAN_Start()
{
	CAN_GPIO_Init();
	CAN_Mode_Init();
	CAN_NVIC_Init();
	CAN_Filter_Config();
}

void Handle_Data(CanRxMsg* data)
{
	//GPIO_Set_Output(GPIOA,GPIO_Pin_0,1);
	len_buffer[len_idx++] = data->Data[0] - 2;//获取当前数据的有效长度,例如07 b5 02 ff ff 07 04 02,实际有效只有ff ff 07 04 02

	//cmd_buffer[cmd_idx++] = data->Data[1];//获取当前数据的命令
	
	uint8_t list = data->Data[2];//全局静态变量,获取还剩多少条数
	
	if(list > list_back)	
		list_back = list;
	
	if(list > 1 && list <= 12)
	{
		for(int i = 3; i < data->DLC; i++)
			recv_buffer[recv_idx++] = data->Data[i];

	}
	
	else if(list == 1)
	{
		for(int i = 3; i < data->DLC; i++)
			recv_buffer[recv_idx++] = data->Data[i];
		
		read_flag = 1;         //接收成功  
		
		uint8_t send_count = 0;
		for(int i = 0; i < list_back; i++)//list_back为当前条数
		{
			 send_count += len_buffer[i];//计算本次需要发送多少数据
		}
		
		//uint8_t send_buffer[3]={list_back,global_cmd,send_count};
		//CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
		//CAN_Transmit(CAN1,&TxMessage);
		
		MODBUS_Send_Data();
		tick_count = 0;
		TIM_Cmd(TIM2, ENABLE);	
		//GPIO_Set_Output(GPIOA,GPIO_Pin_0,0);
		//Default_Return();

	}
	
	else if(list > 12)
	{
		Default_Error_Return();
		//printf("data error");
	}
	
	else
	{
		Default_Error_Return();
		//printf("data error");
		CAN_ReadBufferReset();
	}

}


void Select_Cmd(uint8_t cmd)
{
	switch(cmd)
	{
		case 0x06:
		{
			uint8_t send_buffer[3]={0x02,cmd,0};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);	
			CAN_ReadBufferReset();
			break;	
		}
		
		
		case 0X08:
		{
			Default_Return();
//			printf("IAP");
			IAP = 1;
			TIM_Cmd(TIM2,DISABLE);
			CAN_ReadBufferReset();
			break;
		}
		
		
		case 0x09:
		{
			uint8_t send_buffer[3]={0x02,cmd,0x0B};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);	
			CAN_ReadBufferReset();
			break;			
		}
		
		case 0x61:
		{
			Default_Return();
			uint16_t res;
			switch(recv_buffer[2])
			{
//				case 3:
//				res = GPIO_Pin_3;
//				break;
//				
//				case 4:
//				res = GPIO_Pin_4;
//				break;
				
				case 1://重新给IO排序,实际1对应GPIOA5
				{	
					res = GPIO_Pin_5;
					if (recv_buffer[3])
					GPIO_WriteBit(GPIOA, res, (BitAction)1); // 输出高电平
			 
					else
					GPIO_WriteBit(GPIOA, res, (BitAction)0); // 输出低电平
				}
				break;
				
			    case 2://重新给IO排序,实际2对应GPIOA6
				{	
					res = GPIO_Pin_6;
					if (recv_buffer[3])
					GPIO_WriteBit(GPIOA, res, (BitAction)1); // 输出高电平
			 
					else
					GPIO_WriteBit(GPIOA, res, (BitAction)0); // 输出低电平
				}
				break;
			}
			//GPIO_Set_Output(GPIOA,recv_buffer[2],recv_buffer[3]);
			
			CAN_ReadBufferReset();
			break;
		}
		
		case 0x62:
		{
			uint8_t res;
			switch(recv_buffer[2])
			{
//				case 0:
//				res = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
//				break;
//				
//				case 1:
//				res = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
//				break;
				
				case 1://重新给IO排序,实际1对应GPIOA3
				{
					res = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
					uint8_t send_buffer[4]={0x03,cmd,recv_buffer[2],res};
					CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
					CAN_Transmit(CAN1,&TxMessage);	
				}
				break;
				
			    case 2://重新给IO排序,实际2对应GPIOA4
				{
					res = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
					uint8_t send_buffer[4]={0x03,cmd,recv_buffer[2],res};
					CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
					CAN_Transmit(CAN1,&TxMessage);	
				}
				break;
			}

			CAN_ReadBufferReset();
			break;
		}
		
		case 0xB3:
		{
			uint8_t send_buffer[3]={0x02,cmd,recv_idx};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);
			CAN_ReadBufferReset();
			break;
		}
		
//		case 0xB4:
//		{
//			printf("Print Recv");
//			printf("Recv_len : %d",gripper_len);
//			uint8_t take_lenth = RxMessage.Data[2];//需要获取的缓冲区长度
//			if(gripper_len != 0 && take_lenth <= gripper_len)
//			{
//				UART1_To_CAN1(take_lenth);
//				CAN_ReadBufferReset();
//			}
//			
//			else //若缓冲区无数据或是获取的数据长度大于缓冲区长度
//			{	
//				uint8_t send_buffer[3]={0x02,0xB4,0};
//				CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
//				CAN_Transmit(CAN1,&TxMessage);
//				CAN_ReadBufferReset();
//			}
//			//USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
//			break;
//		}
		
		case 0xB6:
		{
			CAN_ReadBufferReset();
			break;
		}
		
		case 0xB7:
		{
			uint8_t send_buffer[3]= {0x02,cmd,recv_buffer[0]};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);
			CAN_ReadBufferReset();
			break;
		}
		

		
		case 0x0A:
		{
			Default_Return();
			over_time = RxMessage.Data[2] << 8 |RxMessage.Data[3];
			//printf("overtime set to %d ms",over_time);
			CAN_ReadBufferReset();
			break;
		}
		
		
		case 0x71:
		{
			uint8_t res_1;
			uint8_t res_2;
			res_1 = !(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1));//btn1
			res_2 = !(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));//btn2
			uint8_t send_buffer[4]={0x03,cmd,res_1,res_2};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);	
			CAN_ReadBufferReset();
			break;
		}
		
		case 0x72:
		{
			uint8_t res_1;
			uint8_t res_2;
			uint8_t res_3;
			uint8_t res_4;
			res_1 = !(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1));//btn1
			res_2 = !(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));//btn2
			res_3 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
			res_4 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
			uint8_t send_buffer[6]={0x05,cmd,res_3,res_4,res_1,res_2};
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);	
			CAN_ReadBufferReset();
			break;
			
		}
		
		case 0x73:
		{
			Default_Return();
			int32_t Baud_Rate = RxMessage.Data[2] << 24 | RxMessage.Data[3] << 16 | RxMessage.Data[4] << 8 | RxMessage.Data[5];
			MODBUS_USART_ModeConfig(Baud_Rate);
			CAN_ReadBufferReset();
			break;
		}
		
//		case 0x0B:
//		{
//			Default_Return();
//			Into_APP_Flag = 1;
//			break;
//		}
		
		
		case 0x0E:
		{
			Default_Return();
			Into_BootLoader_Flag = 1;
			break;
		}
		
//		case 0x74:
//		{
//			GPIO_ResetBits(GPIOA, GPIO_Pin_8);//关闭485通信
//			CAN_ReadBufferReset();
//			break;
//		}
//		
//		case 0x75:
//		{
//			GPIO_ResetBits(GPIOA, GPIO_Pin_8);//开启485通信
//			CAN_ReadBufferReset();
//			break;
//		}
		
		case 0x0C:
		{
			switch(recv_buffer[2])
			{
				case 1:
				{
					ShowAll_Color(COLOR_GREEN);
					break;
				}
				
				case 2:
				{
					ShowAll_Color(COLOR_BLUE);
					break;
				}
				
				case 3:
				{
					ShowAll_Color(COLOR_RED);
					break;
				}
				
				case 4:
				{
					ShowAll_Color(COLOR_BLACK);
					break;
				}
			}
			
			Default_Return();
			CAN_ReadBufferReset();
			break;
		}
		
		default:
			Default_Error_Return();
			CAN_ReadBufferReset();
		
	}
}


void Default_Return()
{
	uint8_t send_buffer[2]={global_cmd,0x01};
	CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
	CAN_Transmit(CAN1,&TxMessage);	
}

void Default_Error_Return()
{
	uint8_t send_buffer[2]={global_cmd,0x00};
	CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
	CAN_Transmit(CAN1,&TxMessage);	
}

