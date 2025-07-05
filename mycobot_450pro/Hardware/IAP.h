#ifndef __IAP_H__
#define __IAP_H__
#include "SYS.h"  
#include "CAN.h"
#include "TIM.h"

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.

#define FLASH_APP1_ADDR		0x08003000 	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
#define DELIVER_ADDR 		0x0800FFFC		//����0X08000000~0X08002FFF�Ŀռ�ΪIAPʹ��

extern uint8_t  read_flag; 
extern CanTxMsg TxMessage;			     //���ͽṹ��
extern CanRxMsg RxMessage;				 //���սṹ��
extern uint8_t IAP;



void USER_IAP(void);
void iap_load_app(u32 appxaddr);			//ִ��flash�����app����
uint32_t iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin
void Load_App(void);//��ת�³���
void Flash_FirmWare(void); //��д�̼�
uint32_t Read_Deliver_Addr(void);
void Write_Deliver_Addr(uint32_t addr);
void Into_Bootloader(uint32_t addr);
void Peripheral_Deinit(void);
#endif

