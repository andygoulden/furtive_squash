// cSpell:words decrip heth hrng hpcd voltagescaling oscillatortype pllsource pllm plln pllp pllq clocktype hclk sysclk pclk sysclksource pllclk ahbclk tickstart outputdata rmii crcpad iphdr
// cSpell:words addressingmode dualaddress nomask generalcall nostretch analogfilter itface vbus gpioa gpiob gpioc gpiod gpioe gpiof gpiog gpioh nopull htim pulldown exti nvic

#include "hal_mx.h"

#include "FreeRTOS.h" // IWYU pragma: export
#include "board.h"
#include "fs_freertos.h"
#include "print_functions/print_functions.h"
#include "stm32f7xx_hal.h"
#include "task.h"

CRC_HandleTypeDef hcrc;
I2C_HandleTypeDef hi2c1;
RNG_HandleTypeDef hrng;

void Error_Handler(void) {
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

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
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

void HAL_Delay_CPU(uint32_t Delay) {
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY) {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait) {
  }
}

void HAL_Delay(uint32_t Delay) {
  if (rtosStarted()) {
    vTaskDelay(Delay);
  } else {
    HAL_Delay_CPU(Delay);
  }
}

void MX_CRC_Init(void) {
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK) {
    Error_Handler();
  }
}

void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00808CD2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
    Error_Handler();
  }
}

void MX_RNG_Init(void) {
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK) {
    Error_Handler();
  }
}

void MX_GPIO_Init(void) {
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);
  }

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);
  }

  /* SX1262 pins (SPI AF pins in `stm32f7xx_hal_msp.c`)
   * See `doc/ADR.md:Pin mapping` for the mapping of silkscreen to peripheral bank/pin
   */
  HAL_GPIO_WritePin(SX1262_NSS_Port, SX1262_NSS_Pin, GPIO_PIN_RESET);
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SX1262_NSS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SX1262_NSS_Port, &GPIO_InitStruct);
  }
  // // DIO1 is a configurable IO which can be used for Tx done; configure as interrupt
  // HAL_GPIO_WritePin(SX1262_DIO1_Port, SX1262_DIO1_Pin, GPIO_PIN_RESET);
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_DIO1_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  //   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  //   HAL_GPIO_Init(SX1262_DIO1_Port, &GPIO_InitStruct);

  //   // Enable interrupt for pins 15-10
  //   HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  //   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  // }
  // // DIO2 is a configurable IO which can be used for Rx done; configure as interrupt
  // HAL_GPIO_WritePin(SX1262_DIO2_Port, SX1262_DIO2_Pin, GPIO_PIN_RESET);
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_DIO2_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  //   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  //   HAL_GPIO_Init(SX1262_DIO2_Port, &GPIO_InitStruct);

  //   // Enable interrupt for pins 15-10
  //   HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  //   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  // }
  // DIO1 is a configurable IO which can be used for Tx done; configure as normal input for now
  HAL_GPIO_WritePin(SX1262_DIO1_Port, SX1262_DIO1_Pin, GPIO_PIN_RESET);
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SX1262_DIO1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SX1262_DIO1_Port, &GPIO_InitStruct);
  }
  // DIO2 is a configurable IO which can be used for Rx done; configure as normal input for now
  HAL_GPIO_WritePin(SX1262_DIO2_Port, SX1262_DIO2_Pin, GPIO_PIN_RESET);
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SX1262_DIO2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SX1262_DIO2_Port, &GPIO_InitStruct);
  }
  HAL_GPIO_WritePin(SX1262_RST_Port, SX1262_RST_Pin, GPIO_PIN_RESET);
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SX1262_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SX1262_RST_Port, &GPIO_InitStruct);
  }
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SX1262_BUSY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SX1262_BUSY_Port, &GPIO_InitStruct);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
}
