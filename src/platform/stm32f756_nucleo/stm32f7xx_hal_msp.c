#include "board.h"

// cSpell:words syscfg heth gpioa gpiob gpioc gpiod gpioe gpiof gpiog rmii nopull mdio periph periphclk clksource pclk hrng stlk hpcd vbus nvic hspi miso mosi huart

void HAL_MspInit(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
  if(hcrc->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_ENABLE();
  }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc)
{
  if(hcrc->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_DISABLE();
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hi2c->Instance==I2C1)
  {
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_I2C1_CLK_ENABLE();
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    __HAL_RCC_I2C1_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);
  }
}

void HAL_RNG_MspInit(RNG_HandleTypeDef* hrng)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hrng->Instance==RNG)
  {
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }

    __HAL_RCC_RNG_CLK_ENABLE();
  }
}

void HAL_RNG_MspDeInit(RNG_HandleTypeDef* hrng)
{
  if(hrng->Instance==RNG)
  {
    __HAL_RCC_RNG_CLK_DISABLE();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI1)
  {
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    // See `doc/ADR.md:Pin mapping`
    GPIO_InitStruct.Pin = SX1262_SCK_Pin|SX1262_MISO_Pin|SX1262_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SX1262_MISO_Port, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI1)
  {
    __HAL_RCC_SPI1_CLK_DISABLE();

    HAL_GPIO_DeInit(SX1262_MISO_Port, SX1262_SCK_Pin|SX1262_MISO_Pin|SX1262_MOSI_Pin);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  // GPIO_InitTypeDef GPIO_InitStruct = {0};
  // RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  // if(huart->Instance==USART3)
  // {
    // PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    // PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    // if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    // {
    //   while(1);
    // }

    // __HAL_RCC_USART3_CLK_ENABLE();

    // __HAL_RCC_GPIOD_CLK_ENABLE();

    // GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    // HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  // }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);
  }
}
