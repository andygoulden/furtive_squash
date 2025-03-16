#ifndef __PERIPHERAL_INIT_H
#define __PERIPHERAL_INIT_H

#include <stdint.h>

uint32_t gpio_init(void);
void crc_init(void);
void i2c1_init(void);
void rng_init(void);

#endif // __PERIPHERAL_INIT_H
