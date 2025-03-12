#include "FreeRTOS.h" // IWYU pragma: export
#include "task.h"
#include <stdint.h>


#include "board.h"
#include "fs_freertos.h"
#include "hal_mx.h"
#include "print_functions/print_functions.h"
#include "sx126x/furtive_squash_radio.h"
#include "uart/uart.h"

// cSpell:words voltagescaling oscillatortype pllsource pllm plln pllp pllq clocktype hclk sysclk pclk sysclksource pllclk ahbclk clangd

#ifdef CLANGD_FIXES
#define TX_MODE
#define RX_MODE
#endif
const uint8_t test_message[] = {0x01, 0x02, 0x03, 0x04};

void systemTask(void *parameters) {
  (void)parameters;

  PRINT("System task started");

  uint32_t radio_status = furtive_squash_radio_init();
  PRINTF("Radio init returned %u", radio_status);

  const uint32_t led_interval_ms = 1000;
  while (1) {
#ifdef TX_MODE
    if (radio_status == 0) {
      HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
      radio_status = furtive_squash_radio_transmit_packet(test_message, sizeof(test_message));
      PRINTF("Tx returned %d", radio_status);
      HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
    }
#endif

#ifdef RX_MODE
    if (radio_status == 0) {
      HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
      uint8_t rx_buf[sizeof(test_message)] = {0};
      radio_status = furtive_squash_radio_receive_blocking(rx_buf, sizeof(test_message));
      PRINTF("Rx returned %d", radio_status);
      HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
    }
#endif

    HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
    HAL_Delay(led_interval_ms);
  }
}

int main(void) {
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_CRC_Init();
  MX_I2C1_Init();
  MX_RNG_Init();

  uart_init();

  PRINT("\r\n\r\nUART initialised");

  // Quick light flash to show pre-RTOS init success
  HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
  HAL_Delay(100);
  HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
  HAL_Delay(100);
  HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
  HAL_Delay(100);

  start_freertos();

  // Will never get here
  while (1)
    ;
}
