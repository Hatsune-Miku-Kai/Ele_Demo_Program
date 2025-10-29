#include <stm32f4xx.h>
#include "string.h"
#include "485_Init.h"
#include "SEGGER_RTT.h"
#include "stdio.h"

#define HEADER 0xFE

#define USART1_RX_BUF_SIZE 128
#define USART1_TX_BUF_SIZE 128

uint8_t USART1_RX_BUF[USART1_RX_BUF_SIZE]; // 接收缓冲区

uint8_t USART1_TX_BUF[USART1_TX_BUF_SIZE]; // 发送缓冲区

uint8_t Handle_Data[128];

uint8_t CRC1;
uint8_t CRC2;


//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t is_output;  // 1=输出, 0=输入
} IO_Pin;

IO_Pin io_map_input[IO_NUM / 2] = {
	
	//GPIOE
    {GPIOE, GPIO_Pin_2, 0},  // I1
    {GPIOE, GPIO_Pin_3, 0},  // I2
    {GPIOE, GPIO_Pin_4, 0},  // I3
    {GPIOE, GPIO_Pin_5, 0},  // I4
	
	
	//GPIOB
    {GPIOB, GPIO_Pin_6, 0},	 // I5
	
	//GPIOC             
    {GPIOC, GPIO_Pin_13,0}, // I6
    {GPIOC, GPIO_Pin_14,0}, // I7
    {GPIOC, GPIO_Pin_15,0}, // I8
    {GPIOC, GPIO_Pin_0, 0},  // I9
    {GPIOC, GPIO_Pin_1, 0},  // I10
    {GPIOC, GPIO_Pin_2, 0},  // I11
    {GPIOC, GPIO_Pin_3, 0},  // I12

	//GPIOA             
    {GPIOA, GPIO_Pin_1, 0},  // I13
    {GPIOA, GPIO_Pin_2, 0},  // I14
    {GPIOA, GPIO_Pin_3, 0},  // I15
    {GPIOA, GPIO_Pin_4, 0}  // I16
};


IO_Pin io_map_output[IO_NUM / 2] =
{
	//GPIOA Output
    {GPIOA, GPIO_Pin_7, 1},  // O1
	
	//GPIOC Output
    {GPIOC, GPIO_Pin_4, 1},  // O2
    {GPIOC, GPIO_Pin_5, 1},  // O3
	
	//GPIOB Output
    {GPIOB, GPIO_Pin_0, 1},  // O4
    {GPIOB, GPIO_Pin_1, 1},  // O5

	//GPIOE Output
    {GPIOE, GPIO_Pin_7, 1},  // O6
    {GPIOE, GPIO_Pin_8, 1},  // O7
    {GPIOE, GPIO_Pin_9, 1},  // O8
    {GPIOE, GPIO_Pin_10, 1},  // O9
    {GPIOE, GPIO_Pin_11, 1},  // O10
    {GPIOE, GPIO_Pin_12, 1},  // O11
    {GPIOE, GPIO_Pin_13, 1},  // O12
    {GPIOE, GPIO_Pin_14, 1},  // O13
    {GPIOE, GPIO_Pin_15, 1},  // O14
	
	//GPIOB Output
	{GPIOB, GPIO_Pin_10, 1},  // O15
    {GPIOB, GPIO_Pin_11, 1}  // O16
};

void USART1_Init(int BAUD_RATE)
{
	
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    USART_DeInit(USART1);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 		//使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);		//使能USART1时钟
    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 	//GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	 //GPIOA10复用为USART1
    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; 	//GPIOA10与GPIOA9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				//推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//上拉
    GPIO_Init(GPIOA,&GPIO_InitStructure); 						//初始化PA9，PA10
    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = BAUD_RATE;					//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
    USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;				//收发模式
    USART_Init(USART1, &USART_InitStructure); 	//初始化串口1

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	    //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器、
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn; //嵌套通道为DMA2_Stream5_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级为 2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级为 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //通道中断使能
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn ;//串口1发送中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;   //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  	//开启串口空闲中断
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  	// 开启串口DMA接收
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  	// 开启串口DMA接收
    /* 配置串口DMA接收*/
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  					// 开启DMA时钟
    DMA_DeInit(DMA2_Stream5);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;		//DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_RX_BUF;		//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = USART1_RX_BUF_SIZE;						//数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //高等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不开启FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream5, ENABLE); //使能DMA2_Stream5通道

    DMA_DeInit(DMA2_Stream7);    //初始化DMA Stream
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);//等待DMA可配置
    /* 配置DMA2 Stream7，USART1发送 */
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;            //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)USART1_TX_BUF;      //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //存储器到外设模式
    DMA_InitStructure.DMA_BufferSize         = USART1_TX_BUF_SIZE;       //数据传输量
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //使用普通模式
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         //中等优先级
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //外设突发单次传输
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);                             //初始化DMA Stream7

    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);							//DMA2传输完成中断
    DMA_Cmd(DMA2_Stream7, DISABLE);											//不使能
    USART_Cmd(USART1, ENABLE);  //使能串口1
}


void Handle_USART1_Data(uint8_t* data, uint8_t len)
{

	
	for(int i = 0; i < len; i++)
	{
		if(data[i] == HEADER && data[i + 1] == HEADER)
		{
			uint8_t cmd_len = data[i + 2];
			uint8_t cmd = data[i + 3];
			
			CRC1 = data[cmd_len + 1];
			CRC2 = data[cmd_len + 2];
			
			uint8_t temp[cmd_len + 1];
			memcpy(temp, data + i, cmd_len + 1);//将命令进行校验
			
			uint16_t crc = Check_CRC_Serial(temp, len - 2 , 1);
			
            if(CRC1 != (crc >> 8) && CRC2 != crc )
            {
				memset(data, 0, len);
			}
			else
			{
				memcpy(Handle_Data, data + 4, cmd_len - 3);//将没有帧头,长度,命令和校验码的纯数据存入
				Handle_USART1_Cmd(cmd, Handle_Data, cmd_len - 3);
			}
			
		}
	}
}
	



uint16_t Check_CRC_Serial(uint8_t *data, uint16_t length, uint8_t is_high_first)
{
    uint16_t temp = 0xFFFF;
    uint16_t i, k = 0;

    while (k < length) // 要计算总字节数
    {
        temp ^= data[k]; // 输入要检验字节，异或运算结果
        i = 0;
        while (i < 8) // 单字节位移数
        {
            if ((temp & 0x01) == 0) // 检查运算结果最后位是否为零
            {
                temp >>= 1; // 向右移一位
            }
            else
            {
                temp >>= 1;       // 向右移一位
                temp ^= 0xA001;   // 异或运算结果
            }
            i++;
        }
        k++; // 下一个校验字节
    }

    if (!is_high_first)
        temp = (temp >> 8) | (temp << 8);

    return temp;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET) 	//空闲中断触发
	{
		USART1->SR;
		USART1->DR;	
    	DMA_Cmd(DMA2_Stream5, DISABLE);  					   /* 暂时关闭dma，数据尚未处理 */
    	uint16_t Recv_Len = USART1_RX_BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);/* 获取接收到的数据长度 单位为字节*/
		
//		uint8_t test[5] = {0xaa,0x55,0xaa,0x55,0xaa};
//		DMA_Send_Data(test, 5);
//		printf("%d",Recv_Len );
//		printf("\n\r");
//			for(int i = 0 ; i < Recv_Len;i++)
//			{
//				printf("%02x",USART1_RX_BUF[i]);
//			}
//			printf(" ");
		
        DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
        DMA_SetCurrDataCounter(DMA2_Stream5, USART1_RX_BUF_SIZE);
        DMA_Cmd(DMA2_Stream5, ENABLE);
    	USART_ReceiveData(USART1);   	
	
		Handle_USART1_Data(USART1_RX_BUF, Recv_Len);
    }

  	if(USART_GetFlagStatus(USART1,USART_IT_TXE) == SET)	//串口发送完成
  	{
    	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
 	}
}


void DMA2_Stream7_IRQHandler(void)
{
	//清除标志
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
	{
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7); //清除DMA2_Steam7传输完成标志
   		DMA_Cmd(DMA2_Stream7,DISABLE);				//关闭使能
    	USART_ITConfig(USART1,USART_IT_TC,ENABLE);  //打开串口发送完成中断
	}
}



void DMA_Send_Data(uint8_t *data, uint16_t len)
{
	memcpy(USART1_TX_BUF, data, len);				//复制数据到DMA发送缓存区
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);	//确保DMA可以被设置
	DMA_SetCurrDataCounter(DMA2_Stream7, len);			//设置数据传输长度
	DMA_Cmd(DMA2_Stream7,ENABLE);						//打开DMA数据流，开始发送
}


//void DMA2_Stream7_IRQHandler(void)
//{
//    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
//    {
//        // 清除中断标志
//        DMA_ClearITPendingBit(DMA2_Stream7, DMA_FLAG_TCIF7);
//        DMA_Cmd(DMA2_Stream7, DISABLE);
//        SEGGER_RTT_printf(0, "DMA Transfer Complete!\n");
//    }
//}


void IO_Write(uint8_t index, uint8_t state)
{
    if (index >= IO_NUM) return;
    IO_Pin *io = &io_map_output[index - 1];

    if (io->is_output)
    {
        if (state)
			GPIO_WriteBit(io->port,io->pin,state);
        else
            GPIO_WriteBit(io->port, io->pin,state);  // 标准库输出置低
    }
}

uint8_t IO_Read(uint8_t index)
{
    if (index >= IO_NUM) return 0;
    IO_Pin *io = &io_map_input[index - 1];

    // 标准库读取单个位
    return GPIO_ReadInputDataBit(io->port, io->pin);
}


void Handle_USART1_Cmd(uint8_t cmd, uint8_t* data, uint8_t len)
{
	switch(cmd)
	{
		case 0x01:
		{	
			if(len != 2 )
				return;
			
			uint8_t crc1;
			uint8_t crc2;
			uint8_t pin = data[0];
			uint8_t status = data[1];
			
			IO_Write(pin, status);
			
			uint8_t temp[5] = {HEADER, HEADER, 0x02, cmd, 01};
			uint16_t crc = Check_CRC_Serial(temp, sizeof(temp), 1);
			crc1 = crc >> 8;
			crc2 = crc;
			
			uint8_t Return_Data[7] = {HEADER, HEADER, 0x04, cmd, 01, crc1, crc2};
			memcpy(USART1_TX_BUF, Return_Data, 8);
			DMA_Send_Data(USART1_TX_BUF, 7);
			break;
		}
		
		case 0x02:
		{
			
			if(len != 1)
				return;
			uint8_t crc1;
			uint8_t crc2;
			
			uint8_t pin = data[0];
			uint8_t res = IO_Read(pin);
			uint8_t temp[6] = {HEADER, HEADER, 0x05, cmd, pin, res};
			uint16_t crc = Check_CRC_Serial(temp, sizeof(temp), 1);
			crc1 = crc >> 8;
			crc2 = crc;
			
			uint8_t Return_Data[8] = {HEADER, HEADER, 0x05, cmd, pin, res, crc1, crc2};
			memcpy(USART1_TX_BUF, Return_Data, 8);
			DMA_Send_Data(USART1_TX_BUF, 8);
			break;
		}
		
		default:
			break;
	}
}

