#include "SYS.h"
#include "DELAY.h"
#include "DEBUG_UART.h"
#include "FLASH.h"
#include "IAP.h"
#include "stm32f10x.h"                  // Device header
#define APP_MAX_LEN  1*1024//APP的每个包大小最大1K

uint32_t last_addr = 0;
uint8_t Head = 0;
uint32_t APP_LEN = 0;
uint8_t CAN_RX_BUF[APP_MAX_LEN] __attribute__ ((at(0X20002000)));//接收缓冲,最大APP_MAX_LEN个字节,起始地址为0X20002000//给主程序的RAM是4k
	
iapfun jump2app; 
uint16_t iapbuf[512];  

//uint32_t first_flash = 0;

//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
uint32_t iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	uint16_t t;
	uint16_t i = 0;
	uint16_t temp;
	uint32_t fwaddr = appxaddr;//当前写入的地址
	u8 *dfu = appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp = (u16)dfu[1]<<8;
		temp += (u16)dfu[0];	  
		dfu+=2;//偏移2个字节
		iapbuf[i++] = temp;	    
		if(i == 512)
		{
			i = 0;
			STMFLASH_Write(fwaddr,iapbuf,512);	
			fwaddr += 1024;//偏移2048  16=2*8.所以要乘以2.
		}
	}
	if(i)
	{
		STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
		fwaddr += i * 2;
	}
	return fwaddr;
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{  
		Write_Deliver_Addr(DELIVER_MESSAGE);
		SCB->VTOR = appxaddr;
		
		
		__set_PRIMASK(1);
		SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL = 0;
		RCC_DeInit();
		for (int i = 0; i < 8; i++)
        {
            NVIC->ICER[i]=0xFFFFFFFF;
            NVIC->ICPR[i]=0xFFFFFFFF;
        }
		__set_PRIMASK(0);
		
		
		//__disable_irq();
		//Peripheral_Deinit();
		
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)

		jump2app();									//跳转到APP.
		
		while(1)
		{
			
		}
	} 
//	else
//		printf("失败!!\r\n");
}		 

void USER_IAP(void)
{									
		if(RxMessage.Data[1] == 0x0B && RxMessage.DLC == 2)
		{
			Flash_FirmWare();
			IAP = 0;
			last_addr = 0;
			Into_APP_Flag = 1;
			return;
		}
			
		if(APP_LEN < APP_MAX_LEN)//新周期内,没有收到任何数据,认为本次数据接收完成.
		{
			for(int i = 0; i < RxMessage.DLC; i++)
			{
				CAN_RX_BUF[APP_LEN] = RxMessage.Data[i];
				APP_LEN++;

			}	
			//	printf("success\n\r");
			//printf("%d\n\r",APP_LEN);
		}
		
		if(APP_LEN  == APP_MAX_LEN )
		{
				Flash_FirmWare();
				uint8_t send_buffer[8]={0x07,0x08,1,1,1,1,1,1};//表示可以烧录完成了,可以继续发送数据包
			//printf("Have Rec one package\n\r");
				CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
				CAN_Transmit(CAN1,&TxMessage);
				//CAN_ReadBufferReset();
		}
		
//		else 
//			printf("error");
		

}
				   
		 

void Flash_FirmWare()
{
	//printf("开始更新固件...\r\n");	
	if( Head || ((*(vu32*)(0X20002000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
	{	 
		Head = 1;
		if(last_addr)
			last_addr = iap_write_appbin(last_addr ,CAN_RX_BUF,APP_LEN);//从上一次结束的地方继续烧写,并将新的结束地址重新赋值给自己
		else
		{
			last_addr = iap_write_appbin(FLASH_APP1_ADDR ,CAN_RX_BUF,APP_LEN);//更新FLASH代码 
		}
  
		APP_LEN = 0;
		//printf("固件更新完成!\r\n");	
		//printf("%x\n\r",last_addr);
	}
	
	else 
	{  
		//printf("非FLASH应用程序!\r\n");
	}

	
	return;
}


void Load_App()
{
//	if(Into_BootLoader_Flag)//RxMessage.DLC == 2 && RxMessage.Data[1] == 0x0B
//	{
//		printf("开始执行FLASH用户代码!!\r\n");
		if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
		{	 
			//printf("成功!!\r\n");
			Head = 0;
			iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
		}
		
		else 
		{
			printf("非FLASH应用程序,无法执行!\r\n");	   
		}
	//}	
}



void Write_Deliver_Addr(uint32_t addr)
{
	uint16_t deliver_message[2];
	deliver_message[0] = addr;
	deliver_message[1] = addr >> 16;
	STMFLASH_Write(DELIVER_ADDR,deliver_message,2);	
}

uint32_t Read_Deliver_Addr()
{
	uint16_t read_addr[2];
	STMFLASH_Read(DELIVER_ADDR,read_addr,2);
	return read_addr[1] << 16 | read_addr[0];
}

void Into_Bootloader(uint32_t appxaddr)
{

	jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
	MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
	jump2app();		
}


void Peripheral_Deinit()
{
	DMA_Cmd(DMA1_Channel6, DISABLE);  // 禁用DMA1通道6
    DMA_Cmd(DMA1_Channel7, DISABLE);  // 禁用DMA1通道7
    DMA_DeInit(DMA1_Channel6);        // 反初始化DMA1通道6
    DMA_DeInit(DMA1_Channel7);        // 反初始化DMA1通道7

    USART_Cmd(USART1, DISABLE);       // 禁用USART1
    USART_Cmd(USART2, DISABLE);       // 禁用USART2
    USART_DeInit(USART1);             // 反初始化USART1
    USART_DeInit(USART2);             // 反初始化USART2


    CAN_DeInit(CAN1);                 // 反初始化CAN1
    CAN_Sleep(CAN1);                  // 使CAN进入睡眠模式


    TIM_Cmd(TIM2, DISABLE);           // 禁用TIM2
    TIM_DeInit(TIM2);                 // 反初始化TIM2

    GPIO_DeInit(GPIOA);               // 反初始化GPIOA
    GPIO_DeInit(GPIOB);               // 反初始化GPIOB


}


