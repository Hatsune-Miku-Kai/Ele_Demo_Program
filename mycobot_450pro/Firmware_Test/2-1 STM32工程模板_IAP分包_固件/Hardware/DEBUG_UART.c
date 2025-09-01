#include "DEBUG_UART.h"

int fputc(int ch,FILE *p) 
 
{
 USART_SendData(USART1,(u8)ch);
 
 while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
 
 return ch;
 
}

///**
//  * @brief  ���� DEBUG �����ж�����
//  * @param  ��
//  * @retval ��
//  */
//void DEBUG_NVIC_Config(void)
//{
//    /* ����һ�� NVIC �ṹ�� */
//    NVIC_InitTypeDef nvic_initstruct = {0};
//    
//    /* ���� AFIO ��ص�ʱ�� */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
//      
//    /* �����ж�Դ */
//    nvic_initstruct.NVIC_IRQChannel                     = USART1_IRQn;
//    /* ������ռ���ȼ� */
//    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
//    /* ���������ȼ� */
//    nvic_initstruct.NVIC_IRQChannelSubPriority          =  1;
//    /* ʹ�������ж�ͨ�� */
//    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

//    NVIC_Init(&nvic_initstruct);
//    
//}

/**
 * @brief  ��ʼ������ DEBUG ���� ��IO
 * @param  ��
 * @retval ��
 */
void DEBUG_USART_PinConfig(void)
{
    
    /* ����һ�� GPIO �ṹ�� */
    GPIO_InitTypeDef gpio_initstruct = {0};
    //USART_DeInit(USART1);	//�����ڻָ���Ĭ��״̬
    
    /* ���� DEBUG ��ص�GPIO����/�˿�ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    /*ѡ��Ҫ���Ƶ�GPIO���š�����GPIOģʽΪ ���츴�á�����GPIO����Ϊ50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_9;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_initstruct); 
       
    

    /*ѡ��Ҫ���Ƶ�GPIO���š�����GPIOģʽΪ ��������/�������롢����GPIO����Ϊ50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_10;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_initstruct); 
     
}

/**
 * @brief  ���� DEBUG ���� ģʽ
 * @param  ��
 * @retval ��
 */
void DEBUG_USART_ModeConfig(void)
{
  
	USART_DeInit(USART1);	//�����ڻָ���Ĭ��״̬
    /* ����һ�� USART �ṹ�� */
    USART_InitTypeDef usart_initstruct = {0};
    
    /* ���� DEBUG ��ص�GPIO����/�˿�ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    /* ���ô��ڵĹ������� */
    usart_initstruct.USART_BaudRate                 =  115200;                  //���ò�����
    usart_initstruct.USART_HardwareFlowControl      =  USART_HardwareFlowControl_None;  //����Ӳ��������
    usart_initstruct.USART_Mode                     =  USART_Mode_Tx|USART_Mode_Rx;     //���ù���ģʽ
    usart_initstruct.USART_Parity                   =  USART_Parity_No;                 //����У��λ    
    usart_initstruct.USART_StopBits                 =  USART_StopBits_1;                //����ֹͣλ
    usart_initstruct.USART_WordLength               =  USART_WordLength_8b;             //����֡�����ֳ�
    
    USART_Init(USART1, &usart_initstruct);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�����������ݽ����ж�
    //USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//�����������ݿ����ж�
    
}

void DEBUG_USART_Init(void)
{
    //DEBUG_NVIC_Config();
    
    DEBUG_USART_ModeConfig();

    DEBUG_USART_PinConfig();
    
    USART_Cmd(USART1,ENABLE);

}



/**
 * @brief ����8λ�����麯��
 * @param pusartx��USARTx(x=1,2,3)/UARTx(x=4,5)  
 * @param array:Ҫ���͵�����
 * @param num:�����С
 * @note  ��
 * @retval ��
 */
void USARTX_SendArray(USART_TypeDef *pusartx, uint8_t *array, uint32_t num)
{
    /* �ȴ�������� */
    while (USART_GetFlagStatus(pusartx, USART_FLAG_TC) == RESET);
    
    /* ���Ͷ���ֽ����ݵ� pusartx */
    for (uint32_t i = 0; i < num; i++)
    {
        /* ����һ���ֽ����ݵ� pusartx */
        USART_SendData(pusartx,array[i]);
        
        /* �ȴ��������ݼĴ���Ϊ�� */
        while (USART_GetFlagStatus(pusartx, USART_FLAG_TXE) == RESET);
    }

}

