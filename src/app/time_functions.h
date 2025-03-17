#ifndef __TIME_FUNCTIONS_H
#define __TIME_FUNCTIONS_H

#include <stdint.h>

uint32_t tick_init(void);
uint32_t get_tick(void);
void delay(uint32_t period);
void cpu_delay(uint32_t duration_ms);
void system_clock_config(void);

#endif // __TIME_FUNCTIONS_H
