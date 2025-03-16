#include "furtive_squash_radio.h"

#include <stddef.h>
#include <string.h>

#include "board.h"
#include "print_functions/print_functions.h"
#include "spi/spi.h"
#include "stm32f7xx_hal.h"
#include "sx126x.h"
#include "sx126x_hal.h"
#include "sx126x_regs.h"

#ifndef VERBOSE_PRINTS
#define VERBOSE_PRINTS 1
#endif

// cSpell:words stdby xosc lora ldro symb SYNCWORDBASEADDRESS syncword CRCSEEDBASEADDRESS dcdc tcxo

static bool radio_initialised = false;
const uint32_t frequency_Hz = 433000000;
/*
 * See SX1261/2 Datasheet Table 13-21, "PA Operating Modes with Optimal
 * Settings"
 * Mode   | Output Power | paDutyCycle | hpMax | deviceSel | paLut | Value in SetTxParams
 * :---   | :----------- | :---------- | :---- | :-------- | :---- | :-------------------
 * SX1262 | +14 dBm      | 0x02        | 0x02  | 0x00      | 0x01  | +22 dBm
 */
const sx126x_pa_cfg_params_t pa_cfg_params_14dBm = {.pa_duty_cycle = 0x02, .hp_max = 0x02, .device_sel = 0x00, .pa_lut = 0x01};
const int8_t pwr_in_dbm = +22;
const sx126x_ramp_time_t ramp_time = SX126X_RAMP_3400_US;

const sx126x_mod_params_lora_t lora_modulation_params = {.sf = SX126X_LORA_SF5, .bw = SX126X_LORA_BW_500, .cr = SX126X_LORA_CR_4_5, .ldro = 0};

sx126x_pkt_params_lora_t lora_packet_params = {.preamble_len_in_symb = 12, .header_type = SX126X_LORA_PKT_EXPLICIT, .pld_len_in_bytes = 0, .crc_is_on = 1, .invert_iq_is_on = 0};

// const uint8_t sync_word[8] = {0x50, 0x68, 0x3a, 0xef, 0x5a, 0xba, 0x60, 0xef};
const uint8_t sync_word[4] = {0xAA, 0xAA, 0xAA, 0x6C};

const uint32_t timeout_ms = 0;
const uint32_t tx_wait_timeout_ms = 10000;
const uint32_t rx_wait_timeout_ms = 10000;
const uint32_t busy_wait_timeout_ms = 10000;
// const uint32_t tcxo_timeout = 0x000080;
const uint32_t tcxo_timeout = 0x100000;

#define BUFFER_SIZE 256
static uint8_t tx_address = 0;
static uint8_t rx_address = 0;
// static uint8_t tx_buffer[BUFFER_SIZE] = {0};
static uint8_t rx_buffer[BUFFER_SIZE] = {0};

const uint8_t lora_sync_word[] = {0x14, 0x24};

static uint32_t wait_for_busy(uint32_t timeout_ms) {
  uint32_t status = 1;

  uint32_t tick_start = HAL_GetTick();
  bool keep_waiting = true;
  while (keep_waiting) {
    if (HAL_GPIO_ReadPin(SX1262_BUSY_Port, SX1262_BUSY_Pin) == GPIO_PIN_RESET) {
      keep_waiting = false;
      PRINT("Busy went low");
      status = 0;
    } else if (HAL_GetTick() > (tick_start + timeout_ms)) {
      keep_waiting = false;
      PRINT("Timed out");
    } else {
      HAL_Delay(1);
    }
  }

  return status;
}

// static sx126x_status_t check_spi_read(bool print_output) {
//   sx126x_status_t status = SX126X_STATUS_OK;

//   status = wait_for_busy(tx_wait_timeout_ms);

//   uint8_t reg_bytes[1] = {0xFF};
//   if (status == SX126X_STATUS_OK) {
//     status = sx126x_read_register(NULL, SX126X_REG_CRCSEEDBASEADDRESS, reg_bytes, 1);

//     if (print_output) {
//       PRINTC("Read register bytes: ");
//       PRINT_HEX(reg_bytes, sizeof(reg_bytes));
//       PRINT(" (should be 0x1D)");
//     }
//   }

//   // CRC initial value MSB should be 0x1D
//   if (status == SX126X_STATUS_OK) {
//     if (reg_bytes[0] == 0x1D) {
//       status = SX126X_STATUS_OK;
//     } else {
//       status = SX126X_STATUS_ERROR;
//     }
//   }

//   return status;
// }

static sx126x_status_t furtive_squash_radio_set_packet_length(uint8_t payload_length_bytes) {
  sx126x_status_t status = SX126X_STATUS_OK;

  lora_packet_params.pld_len_in_bytes = payload_length_bytes;
  status = sx126x_set_lora_pkt_params(NULL, &lora_packet_params);

  return status;
}

// Configure radio for particular frequency
static sx126x_status_t furtive_squash_radio_configure() {
  sx126x_status_t status = SX126X_STATUS_OK;

  // Set protocol to LoRa®
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_pkt_type(NULL, SX126X_PKT_TYPE_LORA);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Calibrate image
  sx126x_cal_mask_t calibration_mask = SX126X_CAL_IMAGE;
  if (status == SX126X_STATUS_OK) {
    status = sx126x_cal(NULL, calibration_mask);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set RF frequency
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_rf_freq(NULL, frequency_Hz);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Configure power amplifier
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_pa_cfg(NULL, &pa_cfg_params_14dBm);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set transmit parameters
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_tx_params(NULL, pwr_in_dbm, ramp_time);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set LoRa modulation parameters
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_lora_mod_params(NULL, &lora_modulation_params);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set fallback mode (after Rx or Tx, go to STDBY_RC mode)
  if (status == SX126X_STATUS_OK) {
    sx126x_fallback_modes_t fallback_mode = SX126X_FALLBACK_STDBY_RC;
    status = sx126x_set_rx_tx_fallback_mode(NULL, fallback_mode);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set packet parameters
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_lora_pkt_params(NULL, &lora_packet_params);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set sync word (FSK mode)
  if (status == SX126X_STATUS_OK) {
    status = sx126x_write_register(NULL, SX126X_REG_SYNCWORDBASEADDRESS, sync_word, sizeof(sync_word));
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set Tx clamp configuration to better accommodate antenna mismatch
  // (see section "15.2 Better Resistance of the SX1262 Tx to Antenna Mismatch" in data sheet)
  if (status == SX126X_STATUS_OK) {
    uint8_t regRead = 0;
    status = sx126x_read_register(NULL, SX126X_REG_TX_CLAMP_CFG, &regRead, sizeof(regRead));
    regRead |= (0x0F << 1);
    status = sx126x_write_register(NULL, SX126X_REG_TX_CLAMP_CFG, &regRead, sizeof(regRead));
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  return status;
}

static void print_errors(sx126x_errors_mask_t errors_mask) {
  if (errors_mask & SX126X_ERRORS_RC64K_CALIBRATION)
    PRINT("SX126X_ERRORS_RC64K_CALIBRATION");
  if (errors_mask & SX126X_ERRORS_RC13M_CALIBRATION)
    PRINT("SX126X_ERRORS_RC13M_CALIBRATION");
  if (errors_mask & SX126X_ERRORS_PLL_CALIBRATION)
    PRINT("SX126X_ERRORS_PLL_CALIBRATION");
  if (errors_mask & SX126X_ERRORS_ADC_CALIBRATION)
    PRINT("SX126X_ERRORS_ADC_CALIBRATION");
  if (errors_mask & SX126X_ERRORS_IMG_CALIBRATION)
    PRINT("SX126X_ERRORS_IMG_CALIBRATION");
  if (errors_mask & SX126X_ERRORS_XOSC_START)
    PRINT("SX126X_ERRORS_XOSC_START");
  if (errors_mask & SX126X_ERRORS_PLL_LOCK)
    PRINT("SX126X_ERRORS_PLL_LOCK");
  if (errors_mask & SX126X_ERRORS_PA_RAMP)
    PRINT("SX126X_ERRORS_PA_RAMP");
}

// Initialise the furtive squash radio
uint32_t furtive_squash_radio_init() {
  sx126x_status_t status = SX126X_STATUS_OK;

  if (radio_initialised == false) {
    radio_spi_init();

    if (status == SX126X_STATUS_OK) {
      status = (sx126x_status_t)sx126x_hal_reset(NULL);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    if (status == SX126X_STATUS_OK) {
      status = wait_for_busy(busy_wait_timeout_ms);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    // // Go to STDBY_RC mode
    // if (status == SX126X_STATUS_OK) {
    //   status = sx126x_set_standby(NULL, SX126X_STANDBY_CFG_RC);
    //   PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    // }

    // // Set regulator mode
    // if (status == SX126X_STATUS_OK) {
    //   status = sx126x_set_reg_mode(NULL, SX126X_REG_MODE_DCDC);
    //   PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    // }

    // // Set DIO3 as temperature-compensated crystal oscillator control
    // // DIO3 isn't broken out of the module, it's set up as TCXO control on the module
    // if (status == SX126X_STATUS_OK) {
    //   status = sx126x_set_dio3_as_tcxo_ctrl(NULL, SX126X_TCXO_CTRL_1_7V, tcxo_timeout);
    //   PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    // }

    if (status == SX126X_STATUS_OK) {
      status = wait_for_busy(busy_wait_timeout_ms);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    // Clear device errors
    if (status == SX126X_STATUS_OK) {
      status = sx126x_clear_device_errors(NULL);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    // // Calibrate (everything except image, which is done in the configure function)
    // sx126x_cal_mask_t calibration_mask = SX126X_CAL_RC64K | SX126X_CAL_RC13M | SX126X_CAL_PLL | SX126X_CAL_ADC_PULSE | SX126X_CAL_ADC_BULK_N | SX126X_CAL_ADC_BULK_P;
    // if (status == SX126X_STATUS_OK) {
    //   status = sx126x_cal(NULL, calibration_mask);
    //   PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    // }

    // Set base address for buffers
    if (status == SX126X_STATUS_OK) {
      status = sx126x_set_buffer_base_address(NULL, tx_address, rx_address);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    // Enable IRQ
    if (status == SX126X_STATUS_OK) {
      status = sx126x_set_dio_irq_params(NULL, SX126X_IRQ_RX_DONE, SX126X_IRQ_NONE, SX126X_IRQ_RX_DONE, SX126X_IRQ_NONE);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    // // Set sync word
    // /*
    //  * Differentiate the LoRa® signal for Public or Private Network
    //  * Set to 0x3444 for Public Network
    //  * Set to 0x1424 for Private Network
    //  */
    // if (status == SX126X_STATUS_OK) {
    //   status = sx126x_write_register(NULL, SX126X_REG_LR_SYNCWORD, lora_sync_word, sizeof(lora_sync_word));
    //   // sx126x_write_register(NULL, SX126X_REG_SYNCWORDBASEADDRESS, sync_word, sizeof(sync_word));
    //   PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    // }

    if (status == SX126X_STATUS_OK) {
      status = furtive_squash_radio_configure();
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    // Check for errors
    if (status == SX126X_STATUS_OK) {
      sx126x_errors_mask_t errors = 0;
      status = sx126x_get_device_errors(NULL, &errors);
      PRINTF("Device error mask: %04X", errors);
      print_errors(errors);
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }

    if (status == SX126X_STATUS_OK) {
      radio_initialised = true;
      PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    }
  }

  PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  return (uint32_t)status;
}

uint32_t furtive_squash_radio_transmit_packet(const uint8_t *data_ptr, uint8_t data_length) {
  sx126x_status_t status = SX126X_STATUS_OK;

  PRINTF("Transmitting %d bytes...", data_length);

  furtive_squash_radio_set_packet_length(data_length);

  // // Enable Tx done IRQ and map it to DIO1
  // if (status == SX126X_STATUS_OK) {
  //   status = sx126x_set_dio_irq_params(NULL, SX126X_IRQ_TX_DONE, SX126X_IRQ_TX_DONE, SX126X_IRQ_TX_DONE, SX126X_IRQ_NONE);
  //   PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  // }

  // Write message into tx buffer
  if (status == SX126X_STATUS_OK) {
    status = sx126x_write_buffer(NULL, tx_address, data_ptr, data_length);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Enable transmit, specifying timeout
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_tx(NULL, timeout_ms);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Wait for the IRQ TxDone or Timeout (poll for now)
  if (status == SX126X_STATUS_OK) {
    uint32_t tick_start = HAL_GetTick();
    bool keep_waiting = true;
    while (keep_waiting) {
      sx126x_chip_status_t chip_status = {0};
      status = sx126x_get_status(NULL, &chip_status);
      if (chip_status.cmd_status == SX126X_CMD_STATUS_CMD_TX_DONE) {
        keep_waiting = false;
        PRINT("Status is Tx done");
      } else if (HAL_GetTick() > (tick_start + tx_wait_timeout_ms)) {
        keep_waiting = false;
        status = SX126X_STATUS_ERROR;
        PRINTF("Timed out waiting for Tx done (status: %d; mode: %d)", chip_status.cmd_status, chip_status.chip_mode);
      } else {
        // PRINTF("DIO1:%08X", HAL_GPIO_ReadPin(SX1262_DIO1_Port, SX1262_DIO1_Pin));
        HAL_Delay(1);
      }
    }
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Clear the IRQ TxDone flag
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_dio_irq_params(NULL, SX126X_IRQ_NONE, SX126X_IRQ_NONE, SX126X_IRQ_NONE, SX126X_IRQ_NONE);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  return (uint32_t)status;
}

uint32_t furtive_squash_radio_receive_blocking(uint8_t *data_ptr, uint8_t max_data_size) {
  sx126x_status_t status = SX126X_STATUS_OK;

  // Enable Rx done IRQ and map it to DIO2
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_dio_irq_params(NULL, SX126X_IRQ_RX_DONE, SX126X_IRQ_NONE, SX126X_IRQ_RX_DONE, SX126X_IRQ_NONE);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Set sync word
  /*
   * Differentiate the LoRa® signal for Public or Private Network
   * Set to 0x3444 for Public Network
   * Set to 0x1424 for Private Network
   */
  if (status == SX126X_STATUS_OK) {
    status = sx126x_write_register(NULL, SX126X_REG_LR_SYNCWORD, lora_sync_word, sizeof(lora_sync_word));
    // sx126x_write_register(NULL, SX126X_REG_SYNCWORDBASEADDRESS, sync_word,
    // sizeof(sync_word));
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Enable receive, specifying timeout
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_rx(NULL, timeout_ms);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Wait for the IRQ RxDone or Timeout (poll for now, but it is an interrupt)
  if (status == SX126X_STATUS_OK) {
    uint32_t tick_start = HAL_GetTick();
    bool keep_waiting = true;
    while (keep_waiting) {
      if (HAL_GPIO_ReadPin(SX1262_DIO2_Port, SX1262_DIO2_Pin) == GPIO_PIN_SET) {
        keep_waiting = false;
        PRINT("DIO2 went high");
      } else if (HAL_GetTick() > (tick_start + rx_wait_timeout_ms)) {
        keep_waiting = false;
        status = SX126X_STATUS_ERROR;
        PRINT("Timed out waiting for DIO pin");
      } else {
        HAL_Delay(1);
      }
    }
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  sx126x_irq_mask_t irq_status = 0;
  if (status == SX126X_STATUS_OK) {
    sx126x_get_irq_status(NULL, &irq_status);
    if (irq_status & SX126X_IRQ_CRC_ERROR) {
      PRINTF("ERROR: CRC error after packet receive (IRQ mask: %04X)", irq_status);
      status = SX126X_STATUS_ERROR;
    }
  }

  // Clear the IRQ RxDone flag
  if (status == SX126X_STATUS_OK) {
    status = sx126x_set_dio_irq_params(NULL, SX126X_IRQ_NONE, SX126X_IRQ_NONE, SX126X_IRQ_NONE, SX126X_IRQ_NONE);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
  }

  // Get receive buffer status
  sx126x_rx_buffer_status_t rx_buffer_status = {0};
  if (status == SX126X_STATUS_OK) {
    status = sx126x_get_rx_buffer_status(NULL, &rx_buffer_status);
    PRINTF("furtive_squash_radio.c:%d, status=%u", __LINE__, status);
    if (status == SX126X_STATUS_OK) {
      PRINTF("Got rx buffer status; payload length: %d, buffer start: %d", rx_buffer_status.pld_len_in_bytes, rx_buffer_status.buffer_start_pointer);
    }
  }

  // Read the buffer
  if (status == SX126X_STATUS_OK) {
    if (rx_buffer_status.pld_len_in_bytes <= max_data_size) {

      status = sx126x_read_buffer(NULL, rx_buffer_status.buffer_start_pointer, data_ptr, rx_buffer_status.pld_len_in_bytes);
    } else {
      PRINTF("ERROR: received payload length bigger than buffer (%d > %d)", rx_buffer_status.pld_len_in_bytes, max_data_size);
      status = SX126X_STATUS_ERROR;
    }
  }

  if (status == SX126X_STATUS_OK) {
    PRINTF("Read buffer: %02X...", rx_buffer[0]);
  }

  return (uint32_t)status;
}
