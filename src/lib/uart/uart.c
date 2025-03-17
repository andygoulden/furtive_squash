#include "uart/uart.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_usart.h"

// cSpell:words huart wordlength stopbits hwcontrol advfeature clksource pclk datawidth pushpull stlk

// Buffer for handling string-formatting for printf
#define UART_BUF_LEN 256
static uint8_t uart_buffer[UART_BUF_LEN] = {0};

void uart_init(void) {
  LL_USART_Disable(USART3);

  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
  __HAL_RCC_USART3_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  // Configure pins with alternate functions
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

  LL_USART_Init(USART3, &usart_init);
  LL_USART_Enable(USART3);
}

uint32_t uart_write(uint8_t *data, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    LL_USART_TransmitData8(USART3, data[i]);

    // Wait for Tx FIFO to be empty before sending next byte
    while (LL_USART_IsActiveFlag_TXE(USART3)==false);
  }

  return 0;
}

void uart_str_send(const char *format, ...) {
  memset(uart_buffer, 0, UART_BUF_LEN);

  va_list args;
  va_start(args, format);
  vsnprintf((char *)uart_buffer, UART_BUF_LEN, format, args);
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
