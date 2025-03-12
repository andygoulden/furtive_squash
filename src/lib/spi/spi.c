#include "spi/spi.h"

#include <stdbool.h>
#include <stdint.h>

#include "board.h"
#include "print_functions/print_functions.h" // IWYU pragma: export
#include "stm32f7xx_hal.h"                   // IWYU pragma: export

// cSpell:words hspi prescaler baudrateprescaler firstbit timode crccalculation nssp printc

SPI_HandleTypeDef hspi1;

static const uint32_t default_timeout = 1000;

static inline void nss_write(bool state) { HAL_GPIO_WritePin(SX1262_NSS_Port, SX1262_NSS_Pin, (state ? GPIO_PIN_SET : GPIO_PIN_RESET)); }

void radio_spi_init(void) {
  // Set chip select high
  nss_write(true);

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK) {
    while (1)
      ;
  }
}

uint32_t radio_spi_write(uint8_t *data_pointer, uint32_t data_length, bool toggle_nss) {
  nss_write(false);

  uint32_t return_code = HAL_SPI_Transmit(&hspi1, data_pointer, data_length, default_timeout);

  if (toggle_nss) {
    nss_write(true);
  }

  return return_code;
}

uint32_t radio_spi_read(uint8_t *data_pointer, uint32_t data_length, bool toggle_nss) {
  nss_write(false);

  uint32_t return_code = HAL_SPI_Receive(&hspi1, data_pointer, data_length, default_timeout);

  if (toggle_nss) {
    nss_write(true);
  }

  return return_code;
}
