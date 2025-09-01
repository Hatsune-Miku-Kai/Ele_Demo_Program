#include <stm32f4xx.h>
#include "CAN_Init.h"

void CAN1_Config(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    CAN_InitTypeDef         CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    // 1. 开启时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // CAN1: PA11(RX), PA12(TX)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // 2. 配置 GPIO (PA11 CAN1_RX, PA12 CAN1_TX)
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 复位 CAN1
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);

    // 4. CAN 基本配置
    CAN_InitStructure.CAN_TTCM = DISABLE;        // 时间触发通信关闭
    CAN_InitStructure.CAN_ABOM = ENABLE;         // 自动离线管理
    CAN_InitStructure.CAN_AWUM = ENABLE;         // 自动唤醒
    CAN_InitStructure.CAN_NART = DISABLE;        // 允许自动重传
    CAN_InitStructure.CAN_RFLM = DISABLE;        // FIFO满时覆盖旧数据
    CAN_InitStructure.CAN_TXFP = DISABLE;        // 优先级由标识符决定
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // 正常模式

    // 5. 设置波特率 (APB1=42MHz, CANCLK=42MHz)
    // BaudRate = 42MHz / (Prescaler * (1 + BS1 + BS2))
    // 设定为 500kbps
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 6;   // 42MHz / (6*16) = 500kbps
    CAN_Init(CAN1, &CAN_InitStructure);

    // 6. 配置过滤器 (接收所有ID)
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    // 7. NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 高优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 8. 使能 FIFO0 消息挂号中断
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

// ==================== CAN1 接收中断 ====================
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;

    // 一次性把 FIFO0 清空
    while (CAN_MessagePending(CAN1, CAN_FIFO0) > 0)
    {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

        // 例如: 根据 StdId 判断是哪个电机的反馈
        // if (RxMessage.StdId == 0x181) { motor1_speed = RxMessage.Data[0]; }
    }

    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}

// ==================== CAN1 发送函数 ====================
void CAN1_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    CanTxMsg TxMessage;
    uint8_t i;

    TxMessage.StdId = id;              // 标准帧ID
    TxMessage.ExtId = 0x00;
    TxMessage.IDE = CAN_Id_Standard;   // 标准帧
    TxMessage.RTR = CAN_RTR_Data;      // 数据帧
    TxMessage.DLC = len;               // 数据长度

    for(i = 0; i < len; i++)
        TxMessage.Data[i] = data[i];

    CAN_Transmit(CAN1, &TxMessage);
}
