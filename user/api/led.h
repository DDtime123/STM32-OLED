#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED_ON GPIOA->ODR |= (1 << 0);
#define LED_OFF GPIOA->ODR &= ~(1 << 0);
#define LED_TRUN GPIOA->ODR ^= (1 << 0);

void Led_Init(void);


#endif
