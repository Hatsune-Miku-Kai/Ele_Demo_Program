#ifndef __WS2812_H_
#define __WS2812_H_

#include "stm32f10x.h"

#define LED_COUNT   30    // ��������
#define DATA_PIN    GPIO_Pin_2

typedef enum {
    COLOR_BLACK = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_WHITE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_MAGENTA
} WS2812_Color;

// ��������
void delay(__IO uint32_t nCount);
void One(void);
void Zero(void);
void Reset(void);

// ���ݷ���
void Send24bit(uint8_t G, uint8_t R, uint8_t B);
void ShowAll_RGB(uint8_t G, uint8_t R, uint8_t B);
void ShowAll_Color(WS2812_Color color);

// �������Ƶ� n �ŵ���
void ShowOne(uint16_t index, uint8_t R, uint8_t G, uint8_t B);

// ����ǰ�Ľӿڿ��Ա�����ɾ���������Ƿ�Ҫ�ã�
void Black(void);
void Green(void);
void Red(void);
void Blue(void);





#endif
