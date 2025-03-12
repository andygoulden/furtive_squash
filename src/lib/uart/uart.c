#include "uart/uart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "board.h" // IWYU pragma: export
#include "stm32f7xx_hal.h" // IWYU pragma: export

// cSpell:words huart wordlength stopbits hwcontrol advfeature

UART_HandleTypeDef huart3;

static const uint32_t default_timeout = 1000;

#define UART_BUF_LEN 256
static uint8_t uart_buffer[UART_BUF_LEN] = {0};

void uart_init(void) {
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK) {
    while(1);
  }
}

uint32_t uart_write(uint8_t *data, uint32_t len) {
  uint32_t error_code = HAL_UART_Transmit(&huart3, data, len, default_timeout);

  return error_code;
}

void uart_str_send(const char *format, ...) {
  memset(uart_buffer, 0, UART_BUF_LEN);

  va_list args;
  va_start(args, format);
  vsprintf((char *)uart_buffer, format, args);
  va_end(args);

  if (strlen((char *)uart_buffer) <= UART_BUF_LEN) {
    uart_write(uart_buffer, strlen((char *)uart_buffer));
  }
}

void uart_print_bytes(uint8_t *data_ptr, uint32_t data_len) {
  for (uint32_t i = 0; i < data_len; i++) {
    uart_str_send("%02X", data_ptr[i]);
  }
}
