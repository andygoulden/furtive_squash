#include "fs_freertos.h"

#include <stdint.h>

#include "FreeRTOS.h"
#include "board.h"
#include "task.h"
#include "stm32f7xx_ll_gpio.h"


static bool _rtosInitialised = false;

static StaticTask_t systemTaskBuffer;
static StackType_t systemTaskStack[configMINIMAL_STACK_SIZE] = {0};
static TaskHandle_t systemTaskHandle;

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize);

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

bool rtosStarted(void)
{
  return _rtosInitialised;
}

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void start_freertos()
{
  systemTaskHandle = xTaskCreateStatic(
      systemTask, "system", configMINIMAL_STACK_SIZE, (void*)1,
      tskIDLE_PRIORITY + 1, systemTaskStack, &systemTaskBuffer);

  if (systemTaskHandle != NULL)
  {
    LL_GPIO_TogglePin(GPIOB, LD1_Pin);
    LL_GPIO_TogglePin(GPIOB, LD2_Pin);
    LL_GPIO_TogglePin(GPIOB, LD3_Pin);
    // HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
    // HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
    // HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
  }

  _rtosInitialised = true;

  vTaskStartScheduler();
}
