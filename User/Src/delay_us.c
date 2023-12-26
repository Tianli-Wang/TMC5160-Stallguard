//
// Created by 16336 on 2023/10/23.
//

#include "delay_us.h"
#include "stm32f4xx_hal.h"

void Delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(Delay_us_Handle, 0);
    __HAL_TIM_ENABLE(Delay_us_Handle);
    while (__HAL_TIM_GET_COUNTER(Delay_us_Handle) < us);

    __HAL_TIM_DISABLE(Delay_us_Handle);
}