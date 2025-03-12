#ifndef __HAL_MX_H
#define __HAL_MX_H

// cSpell:words heth hrng hpcd

#include "stm32f7xx_hal.h"  // IWYU pragma: export

extern CRC_HandleTypeDef hcrc;
extern I2C_HandleTypeDef hi2c1;
extern RNG_HandleTypeDef hrng;

void HAL_Delay_CPU(uint32_t Delay);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_CRC_Init(void);
void MX_I2C1_Init(void);
void MX_RNG_Init(void);

#endif  // __HAL_MX_H
