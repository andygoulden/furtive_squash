// cSpell:words htim

#include "stm32f7xx_it.h"

#include "board.h" // IWYU pragma: export
#include "print_functions/print_functions.h"
// #include "stm32f7xx_ll_tim.h"

extern TIM_HandleTypeDef htim1;

void NMI_Handler(void)
{
  PRINT("!NMI_Handler");
  while (1)
  {
  }
}

void HardFault_Handler(void)
{
  PRINT("!HardFault_Handler");
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  PRINT("!MemManage_Handler");
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  PRINT("!BusFault_Handler");
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  PRINT("!UsageFault_Handler");
  while (1)
  {
  }
}

void DebugMon_Handler(void)
{
}

void TIM1_UP_TIM10_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim1);
}
