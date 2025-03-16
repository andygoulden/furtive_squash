#include "FreeRTOS.h" // IWYU pragma: export
#include "task.h"
#include <stdint.h>

#include "board.h"
#include "fs_freertos.h"
#include "hal_mx.h"
#include "print_functions/print_functions.h"
// #include "sx126x/furtive_squash_radio.h"
#include "uart/uart.h"

// #include "stm32f7xx_hal.h"
// #include "core_cm7.h"
#include "peripheral_init.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_rcc.h"


// cSpell:words voltagescaling oscillatortype pllsource pllm plln pllp pllq clocktype hclk sysclk pclk sysclksource pllclk ahbclk clangd nvic cfgr systick clksource ppre prioritygroup syscfg hpre
// cSpell:words presc prescaler

// __IO uint32_t uwTick;
// uint32_t uwTickPrio = (1UL << __NVIC_PRIO_BITS);        /* Invalid PRIO */
// HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT; /* 1KHz */

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
    //       // HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
    //       LL_GPIO_TogglePin(GPIOB, LD2_Pin);
    //       radio_status = furtive_squash_radio_transmit_packet(test_message, sizeof(test_message));
    //       PRINTF("Tx returned %d", radio_status);
    //       // HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
    //       LL_GPIO_TogglePin(GPIOB, LD2_Pin);
    //     }
    // #endif

    // #ifdef RX_MODE
    //     if (radio_status == 0) {
    //       // HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
    //       LL_GPIO_TogglePin(GPIOB, LD3_Pin);
    //       uint8_t rx_buf[sizeof(test_message)] = {0};
    //       radio_status = furtive_squash_radio_receive_blocking(rx_buf, sizeof(test_message));
    //       PRINTF("Rx returned %d", radio_status);
    //       // HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
    //       LL_GPIO_TogglePin(GPIOB, LD3_Pin);
    //     }
    // #endif

    // HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
    LL_GPIO_TogglePin(GPIOB, LD1_Pin);
    vTaskDelay(led_interval_ms);
  }
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    while(1);
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    while(1);
  }
}

int main(void) {

  // // TODO: move this to peripheral_init
  // /* Set Interrupt Group Priority */
  // NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  // // HAL_InitTick(TICK_INT_PRIORITY);
  // /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
  // SysTick_Config(SystemCoreClock / (1000U / uwTickFreq));
  // // HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0U);
  // NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TICK_INT_PRIORITY, 0U));
  // uwTickPrio = TICK_INT_PRIORITY;

  // /* Init the low level hardware */
  // // HAL_MspInit();
  // __HAL_RCC_PWR_CLK_ENABLE();
  // __HAL_RCC_SYSCFG_CLK_ENABLE();

  // // HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  // NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

  SystemClock_Config();
  // // RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  // // RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  // /** Configure LSE Drive Capability
  //  */
  // // HAL_PWR_EnableBkUpAccess();
  // SET_BIT(PWR->CR1, PWR_CR1_DBP);
  // /** Configure the main internal regulator output voltage
  //  */
  // __HAL_RCC_PWR_CLK_ENABLE();
  // __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  // /** Initializes the RCC Oscillators according to the specified parameters
  //  * in the RCC_OscInitTypeDef structure.
  //  */
  // // RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  // // RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  // // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  // // RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  // // RCC_OscInitStruct.PLL.PLLM = 4;
  // // RCC_OscInitStruct.PLL.PLLN = 72;
  // // RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  // // RCC_OscInitStruct.PLL.PLLQ = 3;
  // // if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
  // //   Error_Handler();
  // // }
  // LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  // LL_RCC_HSE_Enable();
  // LL_RCC_PLL_Enable();
  // __HAL_RCC_PLL_CONFIG(RCC_PLLSOURCE_HSE, 4, 72, RCC_PLLP_DIV2, 3);
  // // LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_2, 72, LL_RCC_PLLP_DIV_2);

  // // /** Initializes the CPU, AHB and APB buses clocks
  // //  */
  // // RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  // // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  // // RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  // // RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  // // RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  // // if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
  // //   Error_Handler();
  // // }
  // // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);
  // // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV16 << 3));
  // // MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_SYSCLK_DIV1);
  // // __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
  // // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV2);
  // // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_HCLK_DIV1) << 3));
  // // SystemCoreClock = HSE_VALUE >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];
  // __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
  // LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  // LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  // LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  // MX_GPIO_Init();
  gpio_init();

  // MX_CRC_Init();
  crc_init();

  // MX_I2C1_Init();
  i2c1_init();

  // MX_RNG_Init();
  rng_init();

  uart_init();

  PRINT("\r\n\r\nUART initialised");

  // // Quick light flash to show pre-RTOS init success
  // // HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
  // LL_GPIO_TogglePin(GPIOB, LD1_Pin);
  // HAL_Delay(100);
  // // HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
  // LL_GPIO_TogglePin(GPIOB, LD2_Pin);
  // HAL_Delay(100);
  // // HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
  // LL_GPIO_TogglePin(GPIOB, LD3_Pin);
  // HAL_Delay(100);

  start_freertos();

  // Will never get here
  while (1)
    ;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
}
