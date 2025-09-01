#ifndef __DEBUG_UART_H_
#define __DEBUG_UART_H_
#include <stdio.h>
#include "stm32f10x.h"                  // Device header

int fputc(int ch, FILE *f);
void DEBUG_USART_Init(void);
#endif
