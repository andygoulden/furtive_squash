#include "time_functions.h"

#include "FreeRTOS.h" // IWYU pragma: export
#include "task.h"
#include <stdint.h>

#include "fs_freertos.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_tim.h"

// cSpell:words nvic voltagescaling oscillatortype pllsource pllm plln pllp pllq clocktype
// cSpell:words hclk sysclk pclk sysclksource pllclk ahbclk prioritygroup systick syscfg
// cSpell:words clksource cfgr ppre hpre presc prescaler tickstart htim countermode
// cSpell:words autoreload clockdivision itflag itsource

volatile uint32_t tick_value;
uint32_t tick_priority = (1UL << __NVIC_PRIO_BITS);         /* Invalid PRIO */
HAL_TickFreqTypeDef tick_frequency = HAL_TICK_FREQ_DEFAULT; /* 1KHz */

void system_clock_config(void) {
  LL_PWR_EnableBkUpAccess();

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  // Configure oscillators
  __HAL_RCC_HSE_CONFIG(RCC_HSE_ON);
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET);
  __HAL_RCC_PLL_DISABLE();
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET);
  __HAL_RCC_PLL_CONFIG(RCC_PLLSOURCE_HSE, 4, 72, RCC_PLLP_DIV2, 3);
  __HAL_RCC_PLL_ENABLE();
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET);

  // Configure clocks
  if (FLASH_LATENCY_2 > __HAL_FLASH_GET_LATENCY()) {
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);
  }
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV16 << 3));
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_SYSCLK_DIV1);
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
  while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_SYSCLKSOURCE_PLLCLK << RCC_CFGR_SWS_Pos));
  if (FLASH_LATENCY_2 < __HAL_FLASH_GET_LATENCY()) {
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);
  }
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV2);
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_HCLK_DIV1) << 3));

  LL_RCC_ClocksTypeDef clocks;
  LL_RCC_GetSystemClocksFreq(&clocks);
  SystemCoreClock = clocks.SYSCLK_Frequency >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];

  tick_init();
}

uint32_t tick_init() {
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  __HAL_RCC_TIM1_CLK_ENABLE();

  LL_RCC_ClocksTypeDef clocks_config;
  LL_RCC_GetSystemClocksFreq(&clocks_config);

  uint32_t prescaler_value = (uint32_t)((clocks_config.PCLK2_Frequency / 1000000U) - 1U);

  LL_TIM_InitTypeDef tim_init = {
      .Prescaler = prescaler_value,
      .CounterMode = LL_TIM_COUNTERMODE_UP,
      .Autoreload = (1000000U / 1000U) - 1U,
      .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
      .RepetitionCounter = 0,
  };
  LL_TIM_Init(TIM1, &tim_init);

  SET_BIT(TIM1->DIER, LL_TIM_DIER_UIE);
  LL_TIM_EnableCounter(TIM1);
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  NVIC_SetPriority(TIM1_UP_TIM10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TICK_INT_PRIORITY, 0U));

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

  return 0;
}

uint32_t get_tick(void) { return tick_value; }

// Delay function which can be used whether FreeRTOS has started or not
void delay(uint32_t period) {
  if (rtosStarted()) {
    // Sleep task while waiting
    vTaskDelay(period);
  } else {
    // Spin-lock CPU while waiting
    cpu_delay(period);
  }
}

void cpu_delay(uint32_t duration_ms) {
  uint32_t tick_end = tick_value + duration_ms;
  while (tick_value < tick_end);
}

// IRQ handler function names defined in start-up assembly script
void TIM1_UP_TIM10_IRQHandler(void) {
  tick_value += tick_frequency;
  CLEAR_BIT(TIM1->SR, LL_TIM_SR_UIF);
}
