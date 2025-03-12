#ifndef SPI_H
#define SPI_H

#include <stdbool.h>
#include <stdint.h>

void radio_spi_init(void);
uint32_t radio_spi_write(uint8_t *data_pointer, uint32_t data_length, bool toggle_nss);
uint32_t radio_spi_read(uint8_t *data_ptr, uint32_t data_length, bool toggle_nss);

#endif // SPI_H
