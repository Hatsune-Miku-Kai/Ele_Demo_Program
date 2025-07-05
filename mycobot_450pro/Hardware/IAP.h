#ifndef __IAP_H__
#define __IAP_H__
#include "SYS.h"  
#include "CAN.h"
#include "TIM.h"

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define FLASH_APP1_ADDR		0x08003000 	//第一个应用程序起始地址(存放在FLASH)
#define DELIVER_ADDR 		0x0800FFFC		//保留0X08000000~0X08002FFF的空间为IAP使用

extern uint8_t  read_flag; 
extern CanTxMsg TxMessage;			     //发送结构体
extern CanRxMsg RxMessage;				 //接收结构体
extern uint8_t IAP;



void USER_IAP(void);
void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
uint32_t iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
void Load_App(void);//跳转新程序
void Flash_FirmWare(void); //烧写固件
uint32_t Read_Deliver_Addr(void);
void Write_Deliver_Addr(uint32_t addr);
void Into_Bootloader(uint32_t addr);
void Peripheral_Deinit(void);
#endif

