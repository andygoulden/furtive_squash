#include "peripheral_init.h"

#include <stdbool.h>
#include <stdint.h>

#include "board.h"
#include "stm32f7xx_ll_crc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_i2c.h"
#include "stm32f7xx_ll_rng.h"

// cSpell:words nopull pushpull pulldown nvic exti hrng outputdata polylength coef initvalue outdata addressingmode dualaddress nomask generalcall
// cSpell:words nostretch periph periphclk pclk clksource opendrain analogfilter ownaddress tcxo

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

  /* Configure three coloured LEDs on Nucleo */
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

  /* Configure GPIO pin : USB_PowerSwitchOn_Pin */
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = USB_PowerSwitchOn_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &init_struct);
    LL_GPIO_ResetOutputPin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin);
  }

  /* Configure GPIO pin : USB_OverCurrent_Pin */
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = USB_OverCurrent_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(USB_OverCurrent_GPIO_Port, &init_struct);
  }

  /* SX1262 pins (SPI AF pins in `stm32f7xx_hal_msp.c`)
   * See `doc/ADR.md:Pin mapping` for the mapping of silkscreen to peripheral bank/pin
   */
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_NSS_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_NSS_Port, &init_struct);
    LL_GPIO_ResetOutputPin(SX1262_NSS_Port, SX1262_NSS_Pin);
  }

  // DIO1 is a configurable IO which can be used for Tx done; configure as normal input for now
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_DIO1_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_DIO1_Port, &init_struct);
  }
  // DIO2 is a configurable IO which can be used for Rx done; configure as normal input for now
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_DIO2_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_DIO2_Port, &init_struct);
  }
  // DIO3 is a pin on the SX1262, but is hard-wired to a TCXO on the module - pin not connected to STM

  /* Configure SX1262 reset pin */
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_RST_Pin, .Mode = LL_GPIO_MODE_OUTPUT, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_PUSHPULL, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_NSS_Port, &init_struct);
    LL_GPIO_ResetOutputPin(SX1262_RST_Port, SX1262_NSS_Pin);
  }

  /* Configure SX1262 busy pin */
  {
    LL_GPIO_InitTypeDef init_struct = {.Pin = SX1262_BUSY_Pin, .Mode = LL_GPIO_MODE_INPUT, .Pull = LL_GPIO_PULL_NO};
    LL_GPIO_Init(SX1262_BUSY_Port, &init_struct);
  }

  return 0;
}

void rng_init(void) { LL_RNG_Enable(RNG); }

void crc_init(void) {
  __HAL_RCC_CRC_CLK_ENABLE();
  LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_32B);
  LL_CRC_SetPolynomialCoef(CRC, LL_CRC_DEFAULT_CRC32_POLY);
  LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);
  LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);
  LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
}

void i2c1_init(void) {
  __HAL_RCC_I2C1_CONFIG(RCC_I2C1CLKSOURCE_PCLK1);

  __HAL_RCC_GPIOB_CLK_ENABLE();

  {
    LL_GPIO_InitTypeDef init_struct = {
        .Pin = GPIO_PIN_8, .Mode = LL_GPIO_MODE_ALTERNATE, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_OPENDRAIN, .Pull = LL_GPIO_PULL_NO, .Alternate = LL_GPIO_AF_4};
    LL_GPIO_Init(GPIOB, &init_struct);
  }
  {
    LL_GPIO_InitTypeDef init_struct = {
        .Pin = GPIO_PIN_9, .Mode = LL_GPIO_MODE_ALTERNATE, .Speed = LL_GPIO_SPEED_FREQ_LOW, .OutputType = LL_GPIO_OUTPUT_OPENDRAIN, .Pull = LL_GPIO_PULL_NO, .Alternate = LL_GPIO_AF_4};
    LL_GPIO_Init(GPIOB, &init_struct);
  }

  __HAL_RCC_I2C1_CLK_ENABLE();

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
