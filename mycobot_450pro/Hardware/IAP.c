#include "SYS.h"
#include "DELAY.h"
#include "DEBUG_UART.h"
#include "FLASH.h"
#include "IAP.h"
#include "stm32f10x.h"                  // Device header
#define APP_MAX_LEN  1*1024//APP��ÿ������С���2K

uint32_t last_addr = 0;
uint8_t Head = 0;
uint32_t APP_LEN = 0;
uint8_t CAN_RX_BUF[APP_MAX_LEN] __attribute__ ((at(0X20002000)));//���ջ���,���APP_MAX_LEN���ֽ�,��ʼ��ַΪ0X20002000//���������RAM��4k
	
iapfun jump2app; 
uint16_t iapbuf[512];  

//uint32_t first_flash = 0;

//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
uint32_t iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	uint16_t t;
	uint16_t i = 0;
	uint16_t temp;
	uint32_t fwaddr = appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu = appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp = (u16)dfu[1]<<8;
		temp += (u16)dfu[0];	  
		dfu+=2;//ƫ��2���ֽ�
		iapbuf[i++] = temp;	    
		if(i == 512)
		{
			i = 0;
			STMFLASH_Write(fwaddr,iapbuf,512);	
			fwaddr += 1024;//ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)
	{
		STMFLASH_Write(fwaddr,iapbuf,i);//������һЩ�����ֽ�д��ȥ.  
		fwaddr += i;
	}
	return fwaddr;
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{  
		
//		if(first_flash == FLASH_APP1_ADDR)
//		{
//			Write_Deliver_Addr(FLASH_BASE);
//			first_flash = Read_Deliver_Addr();
//			printf("%x\n\r",first_flash);
//		}
//		
//		else if(first_flash == FLASH_BASE)
//		{
//			Write_Deliver_Addr(FLASH_APP1_ADDR);
//			first_flash = Read_Deliver_Addr();
//			printf("%x\n\r",first_flash);
//		}

//		if(first_flash != FLASH_APP1_ADDR && first_flash != FLASH_BASE)
//		{
//			Write_Deliver_Addr(FLASH_APP1_ADDR);
//			first_flash = Read_Deliver_Addr();
//			printf("%x\n\r",first_flash);
//		}
		Peripheral_Deinit();
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		__disable_irq();
		__set_FAULTMASK(1);
		__enable_irq();
		jump2app();									//��ת��APP.
	}
	else
		printf("ʧ��!!\r\n");
}		 

void USER_IAP(void)
{									
		if(RxMessage.Data[1] == 0x0C && RxMessage.DLC == 2)
		{

			Flash_FirmWare();
			IAP = 0;
			last_addr = 0;
			return;
		}
			
		if(APP_LEN < APP_MAX_LEN)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
		{
			for(int i = 0; i < RxMessage.DLC; i++)
			{
				CAN_RX_BUF[APP_LEN] = RxMessage.Data[i];
				APP_LEN++;
			}	
				printf("success\n\r");
			printf("%d\n\r",APP_LEN);
		}
		
		else 
			printf("error");
		
		if(APP_LEN  == APP_MAX_LEN )
		{
			Flash_FirmWare();
			uint8_t send_buffer[8]={0x07,0x08,1,1,1,1,1,1};//��ʾ������¼�����,���Լ����������ݰ�
			printf("Have Rec one package\n\r");
			CAN_SetMsg(&TxMessage,send_buffer,sizeof(send_buffer),SERVO_ID);
			CAN_Transmit(CAN1,&TxMessage);
			CAN_ReadBufferReset();
		}

}
				   
		 

void Flash_FirmWare()
{
	printf("��ʼ���¹̼�...\r\n");	
	if( Head || ((*(vu32*)(0X20002000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
	{	 
		Head = 1;
		if(last_addr)
			last_addr = iap_write_appbin(last_addr ,CAN_RX_BUF,APP_LEN);//����һ�ν����ĵط�������д,�����µĽ�����ַ���¸�ֵ���Լ�
		else
		{
			last_addr = iap_write_appbin(FLASH_APP1_ADDR ,CAN_RX_BUF,APP_LEN);//����FLASH���� 
		}
  
		APP_LEN = 0;
		printf("�̼��������!\r\n");	
		printf("%x\n\r",last_addr);
	}
	
	else 
	{  
		printf("��FLASHӦ�ó���!\r\n");
	}

	
	return;
}


void Load_App()
{
	if(RxMessage.DLC == 2 && RxMessage.Data[1] == 0x0B )
	{
		printf("��ʼִ��FLASH�û�����!!\r\n");
		if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
		{	 
			printf("�ɹ�!!\r\n");
			//Close_All();
			iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			Head = 0;
		}
		
		else 
		{
			printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");	   
		}
	}	
}



void Write_Deliver_Addr(uint32_t addr)
{
	uint16_t deliver_addr[2];
	deliver_addr[0] = addr;
	deliver_addr[1] = addr >> 16;
	STMFLASH_Write(DELIVER_ADDR,deliver_addr,2);	
}

uint32_t Read_Deliver_Addr()
{
	uint16_t read_addr[2];
	STMFLASH_Read(DELIVER_ADDR,read_addr,2);
	return read_addr[1] << 16 | read_addr[0];
}

void Into_Bootloader(uint32_t appxaddr)
{
	__disable_irq();
	__set_FAULTMASK(1);
	jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
	MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
	jump2app();		
}


void Peripheral_Deinit()
{
	DMA_Cmd(DMA1_Channel6, DISABLE);  // ����DMA1ͨ��6
    DMA_Cmd(DMA1_Channel7, DISABLE);  // ����DMA1ͨ��7
    DMA_DeInit(DMA1_Channel6);        // ����ʼ��DMA1ͨ��6
    DMA_DeInit(DMA1_Channel7);        // ����ʼ��DMA1ͨ��7

    USART_Cmd(USART1, DISABLE);       // ����USART1
    USART_Cmd(USART2, DISABLE);       // ����USART2
    USART_DeInit(USART1);             // ����ʼ��USART1
    USART_DeInit(USART2);             // ����ʼ��USART2


    CAN_DeInit(CAN1);                 // ����ʼ��CAN1
    CAN_Sleep(CAN1);                  // ʹCAN����˯��ģʽ


    TIM_Cmd(TIM2, DISABLE);           // ����TIM2
    TIM_DeInit(TIM2);                 // ����ʼ��TIM2

    GPIO_DeInit(GPIOA);               // ����ʼ��GPIOA
    GPIO_DeInit(GPIOB);               // ����ʼ��GPIOB


}


