#include "peripheral_init.h"

#include <stdbool.h>
#include <stdint.h>

#include "board.h"
// #include "stm32f7xx_hal.h"
#include "stm32f7xx_ll_crc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_i2c.h"
#include "stm32f7xx_ll_rng.h"

// cSpell:words nopull pushpull pulldown nvic exti hrng outputdata polylength coef initvalue outdata addressingmode dualaddress nomask generalcall nostretch periph periphclk pclk clksource opendrain analogfilter ownaddress

// static inline uint32_t gpio_mask_to_position(uint32_t mask) {
//   switch (mask) {
//   case GPIO_PIN_0:
//     return 0;
//   case GPIO_PIN_1:
//     return 1;
//   case GPIO_PIN_2:
//     return 2;
//   case GPIO_PIN_3:
//     return 3;
//   case GPIO_PIN_4:
//     return 4;
//   case GPIO_PIN_5:
//     return 5;
//   case GPIO_PIN_6:
//     return 6;
//   case GPIO_PIN_7:
//     return 7;
//   case GPIO_PIN_8:
//     return 8;
//   case GPIO_PIN_9:
//     return 9;
//   case GPIO_PIN_10:
//     return 10;
//   case GPIO_PIN_11:
//     return 11;
//   case GPIO_PIN_12:
//     return 12;
//   case GPIO_PIN_13:
//     return 13;
//   case GPIO_PIN_14:
//     return 14;
//   case GPIO_PIN_15:
//     return 15;
//   }
//   return 0xFFFF;
// }

// static void gpio_config_output(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t OutputType, uint32_t Speed, uint32_t Pull, GPIO_PinState init_state) {
//   if (init_state != GPIO_PIN_RESET) {
//     LL_GPIO_SetOutputPin(GPIOx, Pin);
//   } else {
//     LL_GPIO_ResetOutputPin(GPIOx, Pin);
//   }

//   LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_OUTPUT);
//   LL_GPIO_SetPinOutputType(GPIOx, Pin, OutputType);
//   LL_GPIO_SetPinSpeed(GPIOx, Pin, Speed);
//   LL_GPIO_SetPinPull(GPIOx, Pin, Pull);
// }

// static void gpio_config_input(GPIO_TypeDef *GPIOx, uint32_t Pin) {}

uint32_t gpio_init(void) {
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
  // HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // }
  //
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = LD1_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(LD1_GPIO_Port, &init_struct);
    LL_GPIO_ResetOutputPin(LD1_GPIO_Port, LD1_Pin);
  }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = LD2_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(LD2_GPIO_Port, &init_struct);
    LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);
  }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = LD3_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(LD2_GPIO_Port, &init_struct);
    LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD3_Pin);
  }

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  // HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //   HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = USB_PowerSwitchOn_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &init_struct);
    LL_GPIO_ResetOutputPin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin);
  }

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = USB_OverCurrent_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(USB_OverCurrent_GPIO_Port, &init_struct);
  }

  /* SX1262 pins (SPI AF pins in `stm32f7xx_hal_msp.c`)
   * See `doc/ADR.md:Pin mapping` for the mapping of silkscreen to peripheral bank/pin
   */
  // HAL_GPIO_WritePin(SX1262_NSS_Port, SX1262_NSS_Pin, GPIO_PIN_RESET);
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_NSS_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //   HAL_GPIO_Init(SX1262_NSS_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_NSS_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_NSS_Port, &init_struct);
    LL_GPIO_ResetOutputPin(SX1262_NSS_Port, SX1262_NSS_Pin);
  }
    // // DIO1 is a configurable IO which can be used for Tx done; configure as interrupt
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
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_DIO1_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   HAL_GPIO_Init(SX1262_DIO1_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_DIO1_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_DIO1_Port, &init_struct);
  }
  // DIO2 is a configurable IO which can be used for Rx done; configure as normal input for now
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_DIO2_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   HAL_GPIO_Init(SX1262_DIO2_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_DIO2_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_DIO2_Port, &init_struct);
  }
  // HAL_GPIO_WritePin(SX1262_RST_Port, SX1262_RST_Pin, GPIO_PIN_RESET);
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_RST_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //   HAL_GPIO_Init(SX1262_RST_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_RST_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_NSS_Port, &init_struct);
    LL_GPIO_ResetOutputPin(SX1262_RST_Port, SX1262_NSS_Pin);
  }
  // {
  //   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //   GPIO_InitStruct.Pin = SX1262_BUSY_Pin;
  //   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //   GPIO_InitStruct.Pull = GPIO_NOPULL;
  //   HAL_GPIO_Init(SX1262_BUSY_Port, &GPIO_InitStruct);
  // }
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_BUSY_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_BUSY_Port, &init_struct);
  }

  return 0;
}

void rng_init(void) {
  // hrng.Instance = RNG;
  // if (HAL_RNG_Init(&hrng) != HAL_OK) {
  //   Error_Handler();
  // }
  LL_RNG_Enable(RNG);
}

void crc_init(void) {
  // hcrc.Instance = CRC;
  // hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  // hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  // hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  // hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  // hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  // if (HAL_CRC_Init(&hcrc) != HAL_OK) {
  //   Error_Handler();
  // }
  __HAL_RCC_CRC_CLK_ENABLE();
  LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_32B);
  LL_CRC_SetPolynomialCoef(CRC, LL_CRC_DEFAULT_CRC32_POLY);
  LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);
  LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);
  LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
}

void i2c1_init(void) {
  // hi2c1.Instance = I2C1;
  // hi2c1.Init.Timing = 0x00808CD2;
  // hi2c1.Init.OwnAddress1 = 0;
  // hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  // hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  // hi2c1.Init.OwnAddress2 = 0;
  // hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  // hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  // hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  // if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
  //   Error_Handler();
  // }
  // GPIO_InitTypeDef GPIO_InitStruct = {0};
  // RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  // if(hi2c->Instance==I2C1)
  // {
    // PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    // PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    // if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    // {
    //   while(1);
    // }
    __HAL_RCC_I2C1_CONFIG(RCC_I2C1CLKSOURCE_PCLK1);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    // GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    {
      LL_GPIO_InitTypeDef init_struct = {.Pin = GPIO_PIN_8, .Mode = LL_GPIO_MODE_ALTERNATE, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_OPENDRAIN, .Pull = LL_GPIO_PULL_NO, . Alternate=LL_GPIO_AF_4};
      LL_GPIO_Init(GPIOB, &init_struct);
    }
    {
      LL_GPIO_InitTypeDef init_struct = {.Pin = GPIO_PIN_9, .Mode = LL_GPIO_MODE_ALTERNATE, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_OPENDRAIN, .Pull = LL_GPIO_PULL_NO, . Alternate=LL_GPIO_AF_4};
      LL_GPIO_Init(GPIOB, &init_struct);
    }
    __HAL_RCC_I2C1_CLK_ENABLE();
  // }

  LL_I2C_InitTypeDef i2c_init = {
    .PeripheralMode = LL_I2C_MODE_I2C,
    .Timing = 0x00808CD2,
    .AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE,
    .DigitalFilter = 0,
    .OwnAddress1 = 0,
    .TypeAcknowledge = LL_I2C_ACK,
    .OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT,
  };
  LL_I2C_Init(I2C1, &i2c_init);
}
