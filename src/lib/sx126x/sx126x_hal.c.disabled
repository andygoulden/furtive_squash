#include "sx126x_hal.h"

#include <stdbool.h>
#include <stdint.h>

#include "board.h"
#include "print_functions/print_functions.h"
#include "spi/spi.h"
#include "stm32f7xx_hal_gpio.h"

/**
 * Implementations of functions declared in driver library
 */

/**
 * Radio data transfer - write
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [in] data             Pointer to the buffer to be transmitted
 * @param [in] data_length      Buffer size to be transmitted
 *
 * @returns Operation status
 */
sx126x_hal_status_t sx126x_hal_write(const void *context, const uint8_t *command, const uint16_t command_length, const uint8_t *data, const uint16_t data_length) {
  uint32_t write_return = 0;
  if (write_return == 0) {
    write_return = radio_spi_write((uint8_t *)command, command_length, false);
  }
  if ((write_return == 0) && (data_length != 0)) {
    write_return = radio_spi_write((uint8_t *)data, data_length, true);
  }

  return ((write_return == 0) ? SX126X_HAL_STATUS_OK : SX126X_HAL_STATUS_ERROR);
}

/**
 * Radio data transfer - read
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [in] data             Pointer to the buffer to be received
 * @param [in] data_length      Buffer size to be received
 *
 * @returns Operation status
 */
sx126x_hal_status_t sx126x_hal_read(const void *context, const uint8_t *command, const uint16_t command_length, uint8_t *data, const uint16_t data_length) {
  uint32_t write_return = 0;
  write_return = radio_spi_write((uint8_t *)command, command_length, false);
  write_return = radio_spi_read(data, data_length, true);

  return ((write_return == 0) ? SX126X_HAL_STATUS_OK : SX126X_HAL_STATUS_ERROR);
}

/**
 * Reset the radio
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context Radio implementation parameters
 *
 * @returns Operation status
 */
sx126x_hal_status_t sx126x_hal_reset(const void *context) {
  sx126x_hal_status_t hal_status = SX126X_HAL_STATUS_OK;

  HAL_GPIO_WritePin(SX1262_RST_Port, SX1262_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(SX1262_RST_Port, SX1262_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(10); // must be low at least 100us for reset
  HAL_GPIO_WritePin(SX1262_RST_Port, SX1262_RST_Pin, GPIO_PIN_SET);

  return hal_status;
}

/**
 * Wake the radio up.
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context Radio implementation parameters
 *
 * @returns Operation status
 */
sx126x_hal_status_t sx126x_hal_wakeup(const void *context) {
  sx126x_hal_status_t hal_status = SX126X_HAL_STATUS_OK;

  // SX1262 wakes up on falling edge of NSS
  HAL_GPIO_WritePin(SX1262_NSS_Port, SX1262_NSS_Pin, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(SX1262_NSS_Port, SX1262_NSS_Pin, GPIO_PIN_SET);

  return hal_status;
}
