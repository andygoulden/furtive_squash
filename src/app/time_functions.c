#include "time_functions.h"

#include <stdint.h>

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
  /** Configure LSE Drive Capability
   */
  // HAL_PWR_EnableBkUpAccess();
  LL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  {

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 72;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 3;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
      while (1)
        ;
    }
  }

  {

    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
      while (1)
        ;
    }
  }

  tick_init();
}

// TODO: pasted from main

// // TODO: move this to peripheral_init
// /* Set Interrupt Group Priority */
// NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
// // HAL_InitTick(TICK_INT_PRIORITY);
// /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
// SysTick_Config(SystemCoreClock / (1000U / uwTickFreq));
// // HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0U);
// NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TICK_INT_PRIORITY, 0U));
// uwTickPrio = TICK_INT_PRIORITY;

// /* Init the low level hardware */
// // HAL_MspInit();
// __HAL_RCC_PWR_CLK_ENABLE();
// __HAL_RCC_SYSCFG_CLK_ENABLE();

// // HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
// NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
// // RCC_OscInitTypeDef RCC_OscInitStruct = {0};
// // RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
// /** Configure LSE Drive Capability
//  */
// // HAL_PWR_EnableBkUpAccess();
// SET_BIT(PWR->CR1, PWR_CR1_DBP);
// /** Configure the main internal regulator output voltage
//  */
// __HAL_RCC_PWR_CLK_ENABLE();
// __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
// /** Initializes the RCC Oscillators according to the specified parameters
//  * in the RCC_OscInitTypeDef structure.
//  */
// // RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
// // RCC_OscInitStruct.HSEState = RCC_HSE_ON;
// // RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
// // RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
// // RCC_OscInitStruct.PLL.PLLM = 4;
// // RCC_OscInitStruct.PLL.PLLN = 72;
// // RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
// // RCC_OscInitStruct.PLL.PLLQ = 3;
// // if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
// //   Error_Handler();
// // }
// LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
// LL_RCC_HSE_Enable();
// LL_RCC_PLL_Enable();
// __HAL_RCC_PLL_CONFIG(RCC_PLLSOURCE_HSE, 4, 72, RCC_PLLP_DIV2, 3);
// // LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_2, 72, LL_RCC_PLLP_DIV_2);

// // /** Initializes the CPU, AHB and APB buses clocks
// //  */
// // RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
// // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
// // RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
// // RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
// // RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
// // if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
// //   Error_Handler();
// // }
// // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);
// // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV16 << 3));
// // MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_SYSCLK_DIV1);
// // __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
// // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV2);
// // MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_HCLK_DIV1) << 3));
// // SystemCoreClock = HSE_VALUE >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];
// __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
// LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
// LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
// LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

// TODO: main-paste ends

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

uint32_t tick_init() {
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  __HAL_RCC_TIM1_CLK_ENABLE();
  
  LL_RCC_ClocksTypeDef clocks_config;
  LL_RCC_GetSystemClocksFreq(&clocks_config);
  
  uint32_t prescaler_value = (uint32_t)((clocks_config.PCLK2_Frequency / 1000000U) - 1U);
  /*
  htim1.Init.Period = (1000000U / 1000U) - 1U;
  htim1.Init.Prescaler = uwPrescalerValue;
  htim1.Init.ClockDivision = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  */
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
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, TICK_INT_PRIORITY, 0U);
  uwTickPrio = TICK_INT_PRIORITY;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  return 0;
}

uint32_t get_tick(void) {
  return tick_value;
}

void cpu_delay(uint32_t duration_ms) {
  uint32_t tick_end = tick_value + duration_ms;
  while (tick_value < tick_end);
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//   if (htim->Instance == TIM1) {
//     // HAL_IncTick();
//     tick_value += tick_frequency;
//   }
// }

extern TIM_HandleTypeDef htim1;
void TIM1_UP_TIM10_IRQHandler(void) {
  tick_value += tick_frequency;
  CLEAR_BIT(TIM1->SR, LL_TIM_SR_UIF);

  // htim1.Instance = TIM1;
  // HAL_TIM_IRQHandler(&htim1);
  // if ((TIM1->SR & LL_TIM_SR_UIF) == LL_TIM_SR_UIF) {
  // }
  // if ((itflag & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE)) {
  //   if ((itsource & (TIM_IT_UPDATE)) == (TIM_IT_UPDATE)) {
  //     __HAL_TIM_CLEAR_FLAG(TIM1, TIM_FLAG_UPDATE);

  //     HAL_TIM_PeriodElapsedCallback(htim);
  //   }
  // }
}
