#ifndef __WS2812_H_
#define __WS2812_H_

#include "stm32f10x.h"

#define LED_COUNT   30    // 灯珠数量
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

// 基础函数
void delay(__IO uint32_t nCount);
void One(void);
void Zero(void);
void Reset(void);

// 数据发送
void Send24bit(uint8_t G, uint8_t R, uint8_t B);
void ShowAll_RGB(uint8_t G, uint8_t R, uint8_t B);
void ShowAll_Color(WS2812_Color color);

// 单独控制第 n 颗灯珠
void ShowOne(uint16_t index, uint8_t R, uint8_t G, uint8_t B);

// （以前的接口可以保留或删除，看你是否还要用）
void Black(void);
void Green(void);
void Red(void);
void Blue(void);





#endif
