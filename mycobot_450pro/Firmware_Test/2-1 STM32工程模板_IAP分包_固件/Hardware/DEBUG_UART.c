#include "DEBUG_UART.h"

int fputc(int ch,FILE *p) 
 
{
 USART_SendData(USART1,(u8)ch);
 
 while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
 
 return ch;
 
}

///**
//  * @brief  配置 DEBUG 串口中断配置
//  * @param  无
//  * @retval 无
//  */
//void DEBUG_NVIC_Config(void)
//{
//    /* 定义一个 NVIC 结构体 */
//    NVIC_InitTypeDef nvic_initstruct = {0};
//    
//    /* 开启 AFIO 相关的时钟 */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
//      
//    /* 配置中断源 */
//    nvic_initstruct.NVIC_IRQChannel                     = USART1_IRQn;
//    /* 配置抢占优先级 */
//    nvic_initstruct.NVIC_IRQChannelPreemptionPriority   =  1;
//    /* 配置子优先级 */
//    nvic_initstruct.NVIC_IRQChannelSubPriority          =  1;
//    /* 使能配置中断通道 */
//    nvic_initstruct.NVIC_IRQChannelCmd                  =  ENABLE;

//    NVIC_Init(&nvic_initstruct);
//    
//}

/**
 * @brief  初始化控制 DEBUG 串口 的IO
 * @param  无
 * @retval 无
 */
void DEBUG_USART_PinConfig(void)
{
    
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
    //USART_DeInit(USART1);	//将串口恢复到默认状态
    
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽复用、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_9;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_initstruct); 
       
    

    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入/浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_10;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_initstruct); 
     
}

/**
 * @brief  配置 DEBUG 串口 模式
 * @param  无
 * @retval 无
 */
void DEBUG_USART_ModeConfig(void)
{
  
	USART_DeInit(USART1);	//将串口恢复到默认状态
    /* 定义一个 USART 结构体 */
    USART_InitTypeDef usart_initstruct = {0};
    
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    /* 配置串口的工作参数 */
    usart_initstruct.USART_BaudRate                 =  115200;                  //配置波特率
    usart_initstruct.USART_HardwareFlowControl      =  USART_HardwareFlowControl_None;  //配置硬件流控制
    usart_initstruct.USART_Mode                     =  USART_Mode_Tx|USART_Mode_Rx;     //配置工作模式
    usart_initstruct.USART_Parity                   =  USART_Parity_No;                 //配置校验位    
    usart_initstruct.USART_StopBits                 =  USART_StopBits_1;                //配置停止位
    usart_initstruct.USART_WordLength               =  USART_WordLength_8b;             //配置帧数据字长
    
    USART_Init(USART1, &usart_initstruct);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启串口数据接收中断
    //USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//开启串口数据空闲中断
    
}

void DEBUG_USART_Init(void)
{
    //DEBUG_NVIC_Config();
    
    DEBUG_USART_ModeConfig();

    DEBUG_USART_PinConfig();
    
    USART_Cmd(USART1,ENABLE);

}



/**
 * @brief 发送8位的数组函数
 * @param pusartx：USARTx(x=1,2,3)/UARTx(x=4,5)  
 * @param array:要发送的数组
 * @param num:数组大小
 * @note  无
 * @retval 无
 */
void USARTX_SendArray(USART_TypeDef *pusartx, uint8_t *array, uint32_t num)
{
    /* 等待发送完成 */
    while (USART_GetFlagStatus(pusartx, USART_FLAG_TC) == RESET);
    
    /* 发送多个字节数据到 pusartx */
    for (uint32_t i = 0; i < num; i++)
    {
        /* 发送一个字节数据到 pusartx */
        USART_SendData(pusartx,array[i]);
        
        /* 等待发送数据寄存器为空 */
        while (USART_GetFlagStatus(pusartx, USART_FLAG_TXE) == RESET);
    }

}

