//
// Created by 16336 on 2023/12/15.
//

#include "tmc2209.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

#define RX_BUFFER_SIZE 128  // 例如，可以定义为128，或者根据需要的大小调整
uint8_t rxBuffer[RX_BUFFER_SIZE];  // 创建一个相应大小的数组来接收数据


void tmc2209_writeInt(TMC2209TypeDef *tmc2209, uint8_t address, int32_t value)
{
  static uint8_t data[8];

  data[0] = 0x05;
  data[1] = tmc2209->slaveAddress;
  data[2] = address | TMC_WRITE_BIT;
  data[3] = (value >> 24) & 0xFF;
  data[4] = (value >> 16) & 0xFF;
  data[5] = (value >> 8 ) & 0xFF;
  data[6] = (value      ) & 0xFF;
  data[7] = Calc_CRC8(data, 7);

  sendCommandViaUSART2(data, 8);
}

int32_t tmc2209_readInt(TMC2209TypeDef *tmc2209, uint8_t address)
{
  static uint8_t data[8] = { 0 };

  data[0] = 0x05;
  data[1] = tmc2209->slaveAddress;
  data[2] = address;
  data[3] = Calc_CRC8(data, 3);

  sendCommandViaUSART2(data, 4);

  // Byte 0: Sync nibble correct?
  if (data[0] != 0x05)
    return 0;

  // Byte 1: Master address correct?
  if (data[1] != 0xFF)
    return 0;

  // Byte 2: Address correct?
  if (data[2] != address)
    return 0;

  // Byte 7: CRC correct?
  if (data[7] != Calc_CRC8(data, 7))
    return 0;

  return ((uint32_t)data[3] << 24) | ((uint32_t)data[4] << 16) | (data[5] << 8) | data[6];
}

//使用HAL_UART_Transmit_IT()发送命令，这是中断模式的发送函数。
void sendCommandViaUSART2(uint8_t* command, uint16_t size) {
  if (HAL_UART_Transmit_IT(&huart1, command, size) != HAL_OK) {
    // Transmission Error
  }
  // 启用USART2接收中断
  HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE);
}

//使用中断来接收数据，并在接收完成后通过USART1转发。
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    // USART2接收完毕，将数据发送到USART1
    HAL_UART_Transmit_IT(&huart1, rxBuffer, RX_BUFFER_SIZE);

    // 可以重新启动USART2接收
    HAL_UART_Receive_IT(&huart2, rxBuffer, RX_BUFFER_SIZE);
  }
}

void tmc2209_init(TMC2209TypeDef *tmc2209, uint8_t channel, uint8_t slaveAddress)
{
  tmc2209->channel = channel;
  tmc2209->slaveAddress = slaveAddress;
}

uint8_t Calc_CRC8(uint8_t *data, uint8_t length) {
  int i, j;
  uint8_t crc = 0;
  uint8_t currentByte;

  for (i = 0; i < length; i++) {
    currentByte = data[i];
    for (j = 0; j < 8; j++) {
      if ((crc >> 7) ^ (currentByte & 0x01)) {
        crc = (crc << 1) ^ 0x07;
      } else {
        crc <<= 1;
      }
      currentByte >>= 1;
    }
  }
  return crc;
}
