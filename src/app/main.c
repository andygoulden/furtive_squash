#include "FreeRTOS.h" // IWYU pragma: export
#include "task.h"
#include <stdint.h>

#include "board.h"
#include "fs_freertos.h"
#include "peripheral_init.h"
#include "print_functions/print_functions.h"
#include "stm32f7xx_ll_gpio.h"
// #include "sx126x/furtive_squash_radio.h"
#include "time_functions.h"
#include "uart/uart.h"

// cSpell:words voltagescaling oscillatortype pllsource pllm plln pllp pllq clocktype hclk sysclk pclk sysclksource pllclk ahbclk clangd nvic cfgr
// cSpell:words systick clksource ppre prioritygroup syscfg hpre presc prescaler

#ifdef CLANGD_FIXES
#define TX_MODE
#define RX_MODE
#endif
const uint8_t test_message[] = {0x01, 0x02, 0x03, 0x04};

void systemTask(void *parameters) {
  (void)parameters;

  PRINT("System task started");

  // uint32_t radio_status = furtive_squash_radio_init();
  // PRINTF("Radio init returned %u", radio_status);

  const uint32_t led_interval_ms = 1000;
  while (1) {
    // #ifdef TX_MODE
    //     if (radio_status == 0) {
    //       LL_GPIO_TogglePin(GPIOB, LD2_Pin);
    //       radio_status = furtive_squash_radio_transmit_packet(test_message, sizeof(test_message));
    //       PRINTF("Tx returned %d", radio_status);
    //       LL_GPIO_TogglePin(GPIOB, LD2_Pin);
    //     }
    // #endif

    // #ifdef RX_MODE
    //     if (radio_status == 0) {
    //       LL_GPIO_TogglePin(GPIOB, LD3_Pin);
    //       uint8_t rx_buf[sizeof(test_message)] = {0};
    //       radio_status = furtive_squash_radio_receive_blocking(rx_buf, sizeof(test_message));
    //       PRINTF("Rx returned %d", radio_status);
    //       LL_GPIO_TogglePin(GPIOB, LD3_Pin);
    //     }
    // #endif

    LL_GPIO_TogglePin(GPIOB, LD1_Pin);
    vTaskDelay(led_interval_ms);
  }
}

int main(void) {
  system_clock_config();

  gpio_init();
  crc_init();
  i2c1_init();
  rng_init();
  uart_init();

  PRINT("\r\n\r\nUART initialised");

  // Quick light flash to show pre-RTOS init success
  const uint32_t led_duration_ms = 1000;
  LL_GPIO_TogglePin(GPIOB, LD1_Pin);
  delay(led_duration_ms);
  LL_GPIO_TogglePin(GPIOB, LD2_Pin);
  delay(led_duration_ms);
  LL_GPIO_TogglePin(GPIOB, LD3_Pin);
  delay(led_duration_ms);

  start_freertos();

  // Will never get here
  while (1);
}
