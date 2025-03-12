#ifndef FURTIVE_SQUASH_RADIO_H
#define FURTIVE_SQUASH_RADIO_H

#include <stdint.h>

uint32_t furtive_squash_radio_init();
uint32_t furtive_squash_radio_transmit_packet(const uint8_t *data_ptr, uint8_t data_length);
uint32_t furtive_squash_radio_receive_blocking(uint8_t *data_ptr, uint8_t max_data_size);

#endif // FURTIVE_SQUASH_RADIO_H
