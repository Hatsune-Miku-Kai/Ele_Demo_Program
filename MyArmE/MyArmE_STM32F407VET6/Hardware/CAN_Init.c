#include <stm32f4xx.h>
#include "CAN_Init.h"

void CAN1_Config(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    CAN_InitTypeDef         CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    // 1. ����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // CAN1: PA11(RX), PA12(TX)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // 2. ���� GPIO (PA11 CAN1_RX, PA12 CAN1_TX)
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ��λ CAN1
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);

    // 4. CAN ��������
    CAN_InitStructure.CAN_TTCM = DISABLE;        // ʱ�䴥��ͨ�Źر�
    CAN_InitStructure.CAN_ABOM = ENABLE;         // �Զ����߹���
    CAN_InitStructure.CAN_AWUM = ENABLE;         // �Զ�����
    CAN_InitStructure.CAN_NART = DISABLE;        // �����Զ��ش�
    CAN_InitStructure.CAN_RFLM = DISABLE;        // FIFO��ʱ���Ǿ�����
    CAN_InitStructure.CAN_TXFP = DISABLE;        // ���ȼ��ɱ�ʶ������
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // ����ģʽ

    // 5. ���ò����� (APB1=42MHz, CANCLK=42MHz)
    // BaudRate = 42MHz / (Prescaler * (1 + BS1 + BS2))
    // �趨Ϊ 500kbps
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 6;   // 42MHz / (6*16) = 500kbps
    CAN_Init(CAN1, &CAN_InitStructure);

    // 6. ���ù����� (��������ID)
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

    // 7. NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 8. ʹ�� FIFO0 ��Ϣ�Һ��ж�
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

// ==================== CAN1 �����ж� ====================
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;

    // һ���԰� FIFO0 ���
    while (CAN_MessagePending(CAN1, CAN_FIFO0) > 0)
    {
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

        // ����: ���� StdId �ж����ĸ�����ķ���
        // if (RxMessage.StdId == 0x181) { motor1_speed = RxMessage.Data[0]; }
    }

    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}

// ==================== CAN1 ���ͺ��� ====================
void CAN1_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    CanTxMsg TxMessage;
    uint8_t i;

    TxMessage.StdId = id;              // ��׼֡ID
    TxMessage.ExtId = 0x00;
    TxMessage.IDE = CAN_Id_Standard;   // ��׼֡
    TxMessage.RTR = CAN_RTR_Data;      // ����֡
    TxMessage.DLC = len;               // ���ݳ���

    for(i = 0; i < len; i++)
        TxMessage.Data[i] = data[i];

    CAN_Transmit(CAN1, &TxMessage);
}
