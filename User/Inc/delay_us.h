//
// Created by 16336 on 2023/10/23.
//

#ifndef PZT_F405_DELAY_US_H
#define PZT_F405_DELAY_US_H

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim7;

#define Delay_us_Handle (&htim7)

void Delay_us(uint16_t us);
#endif //PZT_F405_DELAY_US_H
