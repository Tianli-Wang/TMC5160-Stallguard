//
// Created by 16336 on 2023/12/15.
//

#ifndef TMC_STALLGUARD_TMC2209_H
#define TMC_STALLGUARD_TMC2209_H

#endif //TMC_STALLGUARD_TMC2209_H

#include "stm32f4xx_hal.h"

#define TMC_WRITE_BIT 0x80

typedef struct {
    uint8_t channel;
    uint8_t slaveAddress;
    uint8_t status;
}TMC2209TypeDef;


uint8_t tmc2209_CRC8(uint8_t *data, uint8_t length);
void tmc2209_writeInt(TMC2209TypeDef *tmc2209, uint8_t address, int32_t value);
void tmc2209_init(TMC2209TypeDef *tmc2209, uint8_t channel, uint8_t slaveAddress);
uint8_t Calc_CRC8(uint8_t *data, uint8_t length);
int32_t tmc2209_readInt(TMC2209TypeDef *tmc2209, uint8_t address);
void sendCommandViaUSART2(uint8_t* command, uint16_t size);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);