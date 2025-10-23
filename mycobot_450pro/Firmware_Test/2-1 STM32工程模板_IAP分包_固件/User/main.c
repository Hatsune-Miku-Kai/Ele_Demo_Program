#include "stm32f10x.h"                  // Device header
#include "CAN.h"
#include "DEBUG_UART.h"
#include "UART.h"
#include "GPIO.h"
#include "IAP.h"
#include "FLASH.h"
#include "TIM.h"
#include "SEGGER_RTT.h"
#include "WS2812.h"

extern int16_t over_time;//超时时间,默认1s
extern uint8_t Into_APP_Flag;
extern uint8_t Into_BootLoader_Flag;

#define IWDG_TIMEOUT_MS     3000    // 看门狗超时 ≈ 3 s
#define EARLY_WRITE_MS      2800    // 提前写入时间

volatile uint32_t no_feed_ms = 0;   // 记录多久没喂狗
volatile uint8_t  msg_written = 0;  // 防止多次写入

// 前向声明
static void IWDG_Init_3s(void);

int main(void)
{
    __disable_irq();
    SCB->VTOR = 0x08003000;
    SysTick_Config(SystemCoreClock/1000);

    over_time = 5000;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    CAN_Start();
	
    MODBUS_USART_Init();
	DMA_Config();
	
    DEBUG_USART_Init();
    EXTI_PB1_PB11_Start();

    TIM2_Init();
    TIM3_Init();

    __enable_irq();

	
    IWDG_Init_3s();

    ShowAll_Color(COLOR_YELLOW);
	Write_Deliver_Addr(DELIVER_MESSAGE);

    while (1)
    {
        // ---- 正常业务逻辑 ----
        if (Into_APP_Flag) {
            Into_APP_Flag = 0;
            Load_App();
        }

        if (Into_BootLoader_Flag) {
            Into_BootLoader_Flag = 0;
            Into_Bootloader(FLASH_BASE);
        }

		
        // 提前写入标志：超过 2.8 s 没喂狗
        if (!msg_written && no_feed_ms >= EARLY_WRITE_MS) 
		{
            __disable_irq();
            Write_Deliver_Addr(BOOTLOADER_MESSAGE);
            __enable_irq();
            msg_written = 1;
            // 不再喂狗，让 IWDG 自行复位
        }

        // 如果程序正常执行，持续喂狗
        if (!msg_written) 
		{
            IWDG_ReloadCounter();
            no_feed_ms = 0;
        }
    }
}



// 看门狗初始化
static void IWDG_Init_3s(void)
{
    // LSI 约 40kHz，3s = 40k * 3 = 120000 计数
    // 256 分频 -> 120000 / 256 ≈ 469
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(469);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

