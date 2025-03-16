#ifndef __BOARD_H
#define __BOARD_H

// cSpell:words gpioa gpiob gpioc gpiod gpioe gpiof gpiog gpioh rmii mdio stlk vbus miso mosi

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h" // IWYU pragma: export

#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB

// See `doc/ADR.md:Pin mapping`
#define SX1262_SCK_Port  GPIOA
#define SX1262_SCK_Pin   GPIO_PIN_5
#define SX1262_MISO_Port GPIOA
#define SX1262_MISO_Pin  GPIO_PIN_6
#define SX1262_MOSI_Port GPIOA
#define SX1262_MOSI_Pin  GPIO_PIN_7
#define SX1262_NSS_Port  GPIOF
#define SX1262_NSS_Pin   GPIO_PIN_12
#define SX1262_DIO1_Port GPIOF
#define SX1262_DIO1_Pin  GPIO_PIN_13
#define SX1262_DIO2_Port GPIOF
#define SX1262_DIO2_Pin  GPIO_PIN_14
#define SX1262_RST_Port  GPIOE
#define SX1262_RST_Pin   GPIO_PIN_11
#define SX1262_BUSY_Port GPIOE
#define SX1262_BUSY_Pin  GPIO_PIN_9

#ifdef __cplusplus
}
#endif

#endif // __BOARD_H
