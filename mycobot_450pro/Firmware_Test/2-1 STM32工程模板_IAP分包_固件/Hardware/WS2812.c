#include "stm32f10x.h"   // Device header
#include "WS2812.h"


void delay(__IO uint32_t nCount) {
    for(; nCount != 0; nCount--);
}

// 高电平时间长 → 表示 1
void One(void)
{
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;

    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
}

// 高电平时间短 → 表示 0
void Zero(void)
{
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;
    GPIOB->ODR |= DATA_PIN;

    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
    GPIOB->ODR &= ~DATA_PIN;
}

// 复位信号
void Reset(void)
{
    for (int i = 0; i < 64; i++) {
        GPIOB->ODR &= ~DATA_PIN;
    }
}

// 发一颗灯的 24bit 数据 (GRB 顺序)
void Send24bit(uint8_t G, uint8_t R, uint8_t B)
{
    for (int i = 7; i >= 0; i--) {
        if (G & (1 << i)) One(); else Zero();
    }
    for (int i = 7; i >= 0; i--) {
        if (R & (1 << i)) One(); else Zero();
    }
    for (int i = 7; i >= 0; i--) {
        if (B & (1 << i)) One(); else Zero();
    }
}

// 同时设置所有灯的颜色
void ShowAll_RGB(uint8_t G, uint8_t R, uint8_t B)
{
    for (int n = 0; n < LED_COUNT; n++) {
        Send24bit(G, R, B);
    }
    Reset();
}


void ShowAll_Color(WS2812_Color color)
{
    switch (color)
    {
        case COLOR_RED:     ShowAll_RGB(0, 255, 0); break;
        case COLOR_GREEN:   ShowAll_RGB(255, 0, 0); break;
        case COLOR_BLUE:    ShowAll_RGB(0, 0, 255); break;
        case COLOR_WHITE:   ShowAll_RGB(255, 255, 255); break;
        case COLOR_YELLOW:  ShowAll_RGB(255, 255, 0); break;
        case COLOR_CYAN:    ShowAll_RGB(255, 0, 255); break;
        case COLOR_MAGENTA: ShowAll_RGB(0, 255, 255); break;
        default:            ShowAll_RGB(0, 0, 0); break;  // BLACK
    }
}

