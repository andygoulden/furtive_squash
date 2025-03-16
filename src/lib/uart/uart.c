#include "uart/uart.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "board.h" // IWYU pragma: export
// #include "stm32f7xx_hal.h" // IWYU pragma: export
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_usart.h"

// cSpell:words huart wordlength stopbits hwcontrol advfeature clksource pclk datawidth

UART_HandleTypeDef huart3;

// static const uint32_t default_timeout = 1000;

#define UART_BUF_LEN 256
static uint8_t uart_buffer[UART_BUF_LEN] = {0};

void uart_init(void) {
  // { // MSP init
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  //   PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  //   PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  //   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  //   {
  //     while(1);
  //   }
    
  //   __HAL_RCC_USART3_CLK_ENABLE();
    
  //   __HAL_RCC_GPIOD_CLK_ENABLE();
    
  //   GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  //   GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  //   HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  // }

  // { // uart with HAL
  //   huart3.Instance = USART3;
  //   huart3.Init.BaudRate = 115200;
  //   huart3.Init.WordLength = UART_WORDLENGTH_8B;
  //   huart3.Init.StopBits = UART_STOPBITS_1;
  //   huart3.Init.Parity = UART_PARITY_NONE;
  //   huart3.Init.Mode = UART_MODE_TX_RX;
  //   huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  //   huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  //   huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  //   huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  //   if (HAL_UART_Init(&huart3) != HAL_OK) {
  //     while(1);
  //   }
  // }

  LL_USART_Disable(USART3);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
  __HAL_RCC_USART3_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  {
    LL_GPIO_InitTypeDef init_struct = {
        .Pin = STLK_RX_Pin, .Mode = LL_GPIO_MODE_ALTERNATE, .Speed = GPIO_SPEED_FREQ_VERY_HIGH, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO, .Alternate = LL_GPIO_AF_7};
    LL_GPIO_Init(GPIOD, &init_struct);
  }
  {
    LL_GPIO_InitTypeDef init_struct = {
        .Pin = STLK_TX_Pin, .Mode = LL_GPIO_MODE_ALTERNATE, .Speed = GPIO_SPEED_FREQ_VERY_HIGH, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO, .Alternate = LL_GPIO_AF_7};
    LL_GPIO_Init(GPIOD, &init_struct);
  }

  __HAL_RCC_USART3_CONFIG(RCC_USART3CLKSOURCE_PCLK1);

  LL_USART_InitTypeDef usart_init = {0};
  LL_USART_StructInit(&usart_init);
  usart_init.BaudRate = 115200;
  //     .BaudRate = 115200,
  //     .DataWidth = LL_USART_DATAWIDTH_8B,
  //     .StopBits = LL_USART_STOPBITS_1,
  //     .Parity = LL_USART_PARITY_NONE,
  //     .TransferDirection = LL_USART_DIRECTION_TX_RX,
  //     .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
  //     .OverSampling = LL_USART_OVERSAMPLING_16,
  // };
  LL_USART_Init(USART3, &usart_init);
  LL_USART_Enable(USART3);
  LL_USART_TransmitData8(USART3, 'a');
  
  // UART_HandleTypeDef huart3;
  // huart3.Instance = USART3;
  // uint8_t test_data[] = {'h','e','y'};
  // HAL_UART_Transmit(&huart3, test_data, sizeof(test_data), 1000);
}

uint32_t uart_write(uint8_t *data, uint32_t len) {
  // uint32_t error_code = HAL_UART_Transmit(&huart3, data, len, default_timeout);
  for (uint32_t i = 0; i < len; i++) {
    LL_USART_TransmitData8(USART3, data[i]);
    while (LL_USART_IsActiveFlag_TXE(USART3)==false);
  }

  // return error_code;
  return 0;
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
