#include "time_functions.h"

#include <stdint.h>


uint32_t tick_init(void) {
  // TODO: move systick setup here
  return 0;
}

// void Delay_CPU(uint32_t Delay) {
//   uint32_t tickstart = HAL_GetTick();
//   uint32_t wait = Delay;

//   /* Add a freq to guarantee minimum wait */
//   if (wait < HAL_MAX_DELAY) {
//     wait += (uint32_t)(uwTickFreq);
//   }

//   while ((HAL_GetTick() - tickstart) < wait) {
//   }
// }

// void Delay(uint32_t period) {
//   if (rtosStarted()) {
//     vTaskDelay(period);
//   } else {
//     HAL_Delay_CPU(period);
//   }
// }

// void PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//   if (htim->Instance == TIM1) {
//     HAL_IncTick();
//   }
// }
