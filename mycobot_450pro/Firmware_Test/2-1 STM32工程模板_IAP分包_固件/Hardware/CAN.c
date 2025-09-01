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
//uint8_t cmd_buffer[256];//�洢ÿ�����ݵ�����
uint8_t len_buffer[8];//�洢ÿ�����ݵĳ���

uint8_t len_idx = 0;//���Ȼ�������λ��
uint8_t cmd_idx = 0;//���������λ��
uint8_t recv_idx = 0;//���ݻ�����
uint8_t send_idx = 0;//���ͻ�����λ��
uint8_t gripper_len = 0; //��צ���ص����ݳ���
uint8_t list_back = 0;
//uint8_t list = 0;
uint8_t IAP = 0;
uint8_t read_flag;  
CanTxMsg TxMessage;			     //���ͽṹ��
CanRxMsg RxMessage;				 //���սṹ��
uint8_t Into_APP_Flag = 0;
uint8_t Into_BootLoader_Flag = 0;
void CAN_GPIO_Init()
{
	GPIO_InitTypeDef gpio_initstruct = {0};
    
    /* ���� CAN ��ص�GPIO����/�˿�ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    /* ʹ��AFIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
    
    /*ѡ��Ҫ���Ƶ�GPIO���š�����GPIOģʽΪ ���츴�á�����GPIO����Ϊ50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_9;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_initstruct); 
      

    /*ѡ��Ҫ���Ƶ�GPIO���š�����GPIOģʽΪ ��������/�������롢����GPIO����Ϊ50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_8;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_initstruct); 
}

void CAN_Mode_Init()
{
    CAN_InitTypeDef can_initstruct = {0};
    
    /* ���� CAN ��ص�GPIO����/�˿�ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

	/* ����CAN�Ĺ������� */
	can_initstruct.CAN_TTCM=DISABLE;                //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	can_initstruct.CAN_ABOM=DISABLE;			        //MCR-ABOM  �Զ����߹��� 
	can_initstruct.CAN_AWUM=DISABLE;			        //MCR-AWUM  ʹ���Զ�����ģʽ
	can_initstruct.CAN_NART=DISABLE;			    //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	can_initstruct.CAN_RFLM=DISABLE;			    //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	can_initstruct.CAN_TXFP=DISABLE;			    //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	can_initstruct.CAN_Mode = CAN_Mode_Normal;      //��������ģʽ
	can_initstruct.CAN_SJW=CAN_SJW_1tq;		        //BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ
	 
	/* ss=1 bs1=5 bs2=3 λʱ����Ϊ(1+5+3) �����ʼ�Ϊʱ������tq*(1+3+5)  */
	can_initstruct.CAN_BS1=CAN_BS1_5tq;		        //BTR-TS1 ʱ���1 ռ����5��ʱ�䵥Ԫ
	can_initstruct.CAN_BS2=CAN_BS2_3tq;		        //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ	
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB1 = 36 MHz) */
	can_initstruct.CAN_Prescaler =4;		        //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+5+3)/4=1 Mbps
	
    /* CAN��ʼ�� */
    CAN_Init(CAN1, &can_initstruct);

    /* CANͨ���ж�ʹ�� */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN_Filter_Config(void)
{
    CAN_FilterInitTypeDef can_filterinitstructure = {0};

    // ������ 0��ʹ�� 32-bit������ģʽ��IdMask�����ھ�ȷƥ���׼ ID
    can_filterinitstructure.CAN_FilterNumber = 0;
    can_filterinitstructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filterinitstructure.CAN_FilterScale = CAN_FilterScale_32bit;

    // ���� 32-bit: ��׼ ID (11-bit) ���� FilterIdHigh �� [15:5] λ
    can_filterinitstructure.CAN_FilterIdHigh = (0x0A << 5) & 0xFFFF; // Ҫ���ܵ� ID
    can_filterinitstructure.CAN_FilterIdLow  = 0x0000;

    // Mask���ڸ߰����У�ID ռ [15:5]������Щλȫ���� 1 ��ʾ��������ȫƥ��ID��
    // 0x7FF << 5 = 0xFFE0�������߰������룻�Ͱ��ֲ����ģ���0��
    can_filterinitstructure.CAN_FilterMaskIdHigh = 0xFFE0;
    can_filterinitstructure.CAN_FilterMaskIdLow  = 0x0000;

    can_filterinitstructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    can_filterinitstructure.CAN_FilterActivation = ENABLE;

    CAN_FilterInit(&can_filterinitstructure);
}

void CAN_NVIC_Init()
{
	/* ����һ�� NVIC �ṹ�� */
    NVIC_InitTypeDef nvic_initstruct = {0};
    
    /* ���� AFIO ��ص�ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
      
    /* �����ж�Դ */
    nvic_initstruct.NVIC_IRQChannel                     = USB_LP_CAN1_RX0_IRQn;
    /* ������ռ���ȼ� */
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  0;
    /* ���������ȼ� */
    nvic_initstruct.NVIC_IRQChannelSubPriority          =  0;
    /* ʹ�������ж�ͨ�� */
    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

    NVIC_Init(&nvic_initstruct);
}

/**
  * @brief  CANͨ�ű�����������,����һ����������Ϊ0-7�����ݰ�
  * @param  TxMessage: ָ��Ҫ��ʼ���ķ��ͱ��Ľṹ��
  * @param  TxData: ���Ҫ�������ݵ�ָ�룬���ֻ�ܷ�8���ֽ�
  * @retval ��
  */
void CAN_SetMsg(CanTxMsg *TxMessage, uint8_t *TxData,uint8_t len,uint16_t ID)
{	  
    uint8_t ubCounter = 0;
	
	TxMessage->StdId=ID;					 	 //ʹ�ñ�׼ID
    TxMessage->IDE=CAN_ID_STD;			         //��չģʽ
    TxMessage->RTR=CAN_RTR_DATA;				 //���͵�������
    TxMessage->DLC=len;							 //���ݳ���Ϊ8�ֽ�

    /*����Ҫ���͵�����0-7*/
    for (ubCounter = 0; ubCounter < len; ubCounter++)
    {
        TxMessage->Data[ubCounter] = TxData[ubCounter];
    }
}



/**
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval ��
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
    uint8_t ubCounter = 0;


    /*�������ݻ���������*/
    for (ubCounter = 0; ubCounter < RxMessage->DLC; ubCounter++)
    {
        RxMessage->Data[ubCounter] = 0x00;
    }
	
	/*�ѽ��սṹ������*/
    RxMessage->StdId = 0x00;
    RxMessage->ExtId = 0x00;
    RxMessage->IDE = CAN_ID_STD;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
}


/**
  * @brief  CAN ���ջ�������λ
  * @param  ��
  * @retval ��
  */
void CAN_ReadBufferReset(void)
{
    //������սṹ��ͽ������ݻ�����
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
	/*�������ж�������*/
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	if (IAP)
	{
		USER_IAP();
		//CAN_ReadBufferReset();
	}
	
	
	/* �Ƿ���IAPģʽ�½������� */ 
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
	len_buffer[len_idx++] = data->Data[0] - 2;//��ȡ��ǰ���ݵ���Ч����,����07 b5 02 ff ff 07 04 02,ʵ����Чֻ��ff ff 07 04 02

	//cmd_buffer[cmd_idx++] = data->Data[1];//��ȡ��ǰ���ݵ�����
	
	uint8_t list = data->Data[2];//ȫ�־�̬����,��ȡ��ʣ��������
	
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
		
		read_flag = 1;         //���ճɹ�  
		
		uint8_t send_count = 0;
		for(int i = 0; i < list_back; i++)//list_backΪ��ǰ����
		{
			 send_count += len_buffer[i];//���㱾����Ҫ���Ͷ�������
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
				
				case 1://���¸�IO����,ʵ��1��ӦGPIOA5
				{	
					res = GPIO_Pin_5;
					if (recv_buffer[3])
					GPIO_WriteBit(GPIOA, res, (BitAction)1); // ����ߵ�ƽ
			 
					else
					GPIO_WriteBit(GPIOA, res, (BitAction)0); // ����͵�ƽ
				}
				break;
				
			    case 2://���¸�IO����,ʵ��2��ӦGPIOA6
				{	
					res = GPIO_Pin_6;
					if (recv_buffer[3])
					GPIO_WriteBit(GPIOA, res, (BitAction)1); // ����ߵ�ƽ
			 
					else
					GPIO_WriteBit(GPIOA, res, (BitAction)0); // ����͵�ƽ
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
				
				case 1://���¸�IO����,ʵ��1��ӦGPIOA3
				{
					res = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
					uint8_t send_buffer[4]={0x03,cmd,recv_buffer[2],res};
					CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
					CAN_Transmit(CAN1,&TxMessage);	
				}
				break;
				
			    case 2://���¸�IO����,ʵ��2��ӦGPIOA4
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
//			uint8_t take_lenth = RxMessage.Data[2];//��Ҫ��ȡ�Ļ���������
//			if(gripper_len != 0 && take_lenth <= gripper_len)
//			{
//				UART1_To_CAN1(take_lenth);
//				CAN_ReadBufferReset();
//			}
//			
//			else //�������������ݻ��ǻ�ȡ�����ݳ��ȴ��ڻ���������
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
//			GPIO_ResetBits(GPIOA, GPIO_Pin_8);//�ر�485ͨ��
//			CAN_ReadBufferReset();
//			break;
//		}
//		
//		case 0x75:
//		{
//			GPIO_ResetBits(GPIOA, GPIO_Pin_8);//����485ͨ��
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

