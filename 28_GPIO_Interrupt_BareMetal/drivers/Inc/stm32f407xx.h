/*
 * stm32f407xx.h
 *
 *  Created on: May 18, 2026
 *      Author: DELL
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

/*********************************************************************
 *                  GENERIC MACROS
 *********************************************************************/

#define __vo                 volatile

#define ENABLE               1U
#define DISABLE              0U
#define SET                  ENABLE
#define RESET                DISABLE

#define GPIO_PIN_SET         SET
#define GPIO_PIN_RESET       RESET

/*********************************************************************
 *                  NVIC ISERx REGISTER ADDRESSES
 *********************************************************************/

#define NVIC_ISER0           ((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1           ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2           ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3           ((__vo uint32_t*)0xE000E10C)

/*********************************************************************
 *                  NVIC ICERx REGISTER ADDRESSES
 *********************************************************************/

#define NVIC_ICER0           ((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1           ((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2           ((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3           ((__vo uint32_t*)0xE000E18C)

/*********************************************************************
 *                  NVIC PRIORITY REGISTER
 *********************************************************************/

#define NVIC_PR_BASE_ADDR    ((__vo uint8_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED    4U

/*********************************************************************
 *                  BASE ADDRESSES OF FLASH AND SRAM
 *********************************************************************/

#define FLASH_BASEADDR       0x08000000U
#define SRAM1_BASEADDR       0x20000000U
#define SRAM2_BASEADDR       0x2001C000U
#define ROM_BASEADDR         0x1FFF0000U

#define SRAM                 SRAM1_BASEADDR

/*********************************************************************
 *                  PERIPHERAL BASE ADDRESSES
 *********************************************************************/

#define PERIPH_BASEADDR      0x40000000U

#define APB1PERIPH_BASEADDR  PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR  0x40010000U
#define AHB1PERIPH_BASEADDR  0x40020000U
#define AHB2PERIPH_BASEADDR  0x50000000U

/*********************************************************************
 *                  GPIO BASE ADDRESSES
 *********************************************************************/

#define GPIOA_BASEADDR       (AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR       (AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR       (AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR       (AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR       (AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR       (AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR       (AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR       (AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR       (AHB1PERIPH_BASEADDR + 0x2000)

#define RCC_BASEADDR         (AHB1PERIPH_BASEADDR + 0x3800)

/*********************************************************************
 *                  APB1 PERIPHERAL BASE ADDRESSES
 *********************************************************************/

#define I2C1_BASEADDR        (APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR        (APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR        (APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR        (APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR        (APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADDR      (APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR      (APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR       (APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR       (APB1PERIPH_BASEADDR + 0x5000)

/*********************************************************************
 *                  APB2 PERIPHERAL BASE ADDRESSES
 *********************************************************************/

#define SPI1_BASEADDR        (APB2PERIPH_BASEADDR + 0x3000)

#define USART1_BASEADDR      (APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR      (APB2PERIPH_BASEADDR + 0x1400)

#define EXTI_BASEADDR        (APB2PERIPH_BASEADDR + 0x3C00)
#define SYSCFG_BASEADDR      (APB2PERIPH_BASEADDR + 0x3800)

/*********************************************************************
 *                  GPIO REGISTER DEFINITION STRUCTURE
 *********************************************************************/

typedef struct
{
    __vo uint32_t MODER;          /* 0x00 */
    __vo uint32_t OTYPER;         /* 0x04 */
    __vo uint32_t OSPEEDR;        /* 0x08 */
    __vo uint32_t PUPDR;          /* 0x0C */
    __vo uint32_t IDR;            /* 0x10 */
    __vo uint32_t ODR;            /* 0x14 */
    __vo uint32_t BSRR;           /* 0x18 */
    __vo uint32_t LCKR;           /* 0x1C */
    __vo uint32_t AFR[2];         /* 0x20 */

}GPIO_RegDef_t;

/*********************************************************************
 *                  RCC REGISTER DEFINITION STRUCTURE
 *********************************************************************/

typedef struct
{
    __vo uint32_t CR;             /* 0x00 */
    __vo uint32_t PLLCFGR;        /* 0x04 */
    __vo uint32_t CFGR;           /* 0x08 */
    __vo uint32_t CIR;            /* 0x0C */

    __vo uint32_t AHB1RSTR;       /* 0x10 */
    __vo uint32_t AHB2RSTR;       /* 0x14 */
    __vo uint32_t AHB3RSTR;       /* 0x18 */
    uint32_t RESERVED0;           /* 0x1C */

    __vo uint32_t APB1RSTR;       /* 0x20 */
    __vo uint32_t APB2RSTR;       /* 0x24 */

    uint32_t RESERVED1[2];        /* 0x28 - 0x2C */

    __vo uint32_t AHB1ENR;        /* 0x30 */
    __vo uint32_t AHB2ENR;        /* 0x34 */
    __vo uint32_t AHB3ENR;        /* 0x38 */
    uint32_t RESERVED2;           /* 0x3C */

    __vo uint32_t APB1ENR;        /* 0x40 */
    __vo uint32_t APB2ENR;        /* 0x44 */

    uint32_t RESERVED3[2];        /* 0x48 - 0x4C */

    __vo uint32_t AHB1LPENR;      /* 0x50 */
    __vo uint32_t AHB2LPENR;      /* 0x54 */
    __vo uint32_t AHB3LPENR;      /* 0x58 */
    uint32_t RESERVED4;           /* 0x5C */

    __vo uint32_t APB1LPENR;      /* 0x60 */
    __vo uint32_t APB2LPENR;      /* 0x64 */

    uint32_t RESERVED5[2];        /* 0x68 - 0x6C */

    __vo uint32_t BDCR;           /* 0x70 */
    __vo uint32_t CSR;            /* 0x74 */

    uint32_t RESERVED6[2];        /* 0x78 - 0x7C */

    __vo uint32_t SSCGR;          /* 0x80 */
    __vo uint32_t PLLI2SCFGR;     /* 0x84 */
    __vo uint32_t PLLSAICFGR;     /* 0x88 */
    __vo uint32_t DCKCFGR;        /* 0x8C */

}RCC_RegDef_t;

/*********************************************************************
 *                  EXTI REGISTER DEFINITION STRUCTURE
 *********************************************************************/

typedef struct
{
    __vo uint32_t IMR;            /* 0x00 */
    __vo uint32_t EMR;            /* 0x04 */
    __vo uint32_t RTSR;           /* 0x08 */
    __vo uint32_t FTSR;           /* 0x0C */
    __vo uint32_t SWIER;          /* 0x10 */
    __vo uint32_t PR;             /* 0x14 */

}EXTI_RegDef_t;

/*********************************************************************
 *                  SYSCFG REGISTER DEFINITION STRUCTURE
 *********************************************************************/

typedef struct
{
    __vo uint32_t MEMRMP;         /* 0x00 */
    __vo uint32_t PMC;            /* 0x04 */
    __vo uint32_t EXTICR[4];      /* 0x08 - 0x14 */
    uint32_t RESERVED[2];         /* 0x18 - 0x1C */
    __vo uint32_t CMPCR;          /* 0x20 */

}SYSCFG_RegDef_t;

/*********************************************************************
 *                  PERIPHERAL DEFINITIONS
 *********************************************************************/

#define GPIOA      ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB      ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC      ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD      ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE      ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF      ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG      ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH      ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI      ((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC        ((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI       ((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG     ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

/*********************************************************************
 *                  GPIO CLOCK ENABLE MACROS
 *********************************************************************/

#define GPIOA_PCLK_EN()     (RCC->AHB1ENR |= (1U << 0))
#define GPIOB_PCLK_EN()     (RCC->AHB1ENR |= (1U << 1))
#define GPIOC_PCLK_EN()     (RCC->AHB1ENR |= (1U << 2))
#define GPIOD_PCLK_EN()     (RCC->AHB1ENR |= (1U << 3))
#define GPIOE_PCLK_EN()     (RCC->AHB1ENR |= (1U << 4))
#define GPIOF_PCLK_EN()     (RCC->AHB1ENR |= (1U << 5))
#define GPIOG_PCLK_EN()     (RCC->AHB1ENR |= (1U << 6))
#define GPIOH_PCLK_EN()     (RCC->AHB1ENR |= (1U << 7))
#define GPIOI_PCLK_EN()     (RCC->AHB1ENR |= (1U << 8))

/*********************************************************************
 *                  GPIO CLOCK DISABLE MACROS
 *********************************************************************/

#define GPIOA_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 0))
#define GPIOB_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 1))
#define GPIOC_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 2))
#define GPIOD_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 3))
#define GPIOE_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 4))
#define GPIOF_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 5))
#define GPIOG_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 6))
#define GPIOH_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 7))
#define GPIOI_PCLK_DI()     (RCC->AHB1ENR &= ~(1U << 8))

/*********************************************************************
 *                  USART CLOCK ENABLE MACROS
 *********************************************************************/

#define USART1_PCLK_EN()    (RCC->APB2ENR |= (1U << 4))
#define USART2_PCLK_EN()    (RCC->APB1ENR |= (1U << 17))
#define USART3_PCLK_EN()    (RCC->APB1ENR |= (1U << 18))
#define UART4_PCLK_EN()     (RCC->APB1ENR |= (1U << 19))
#define UART5_PCLK_EN()     (RCC->APB1ENR |= (1U << 20))
#define USART6_PCLK_EN()    (RCC->APB2ENR |= (1U << 5))

/*********************************************************************
 *                  SYSCFG CLOCK ENABLE MACROS
 *********************************************************************/

#define SYSCFG_PCLK_EN()    (RCC->APB2ENR |= (1U << 14))

/*********************************************************************
 *                  GPIO RESET MACROS
 *********************************************************************/

#define GPIOA_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 0)); (RCC->AHB1RSTR &= ~(1U << 0)); }while(0)
#define GPIOB_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 1)); (RCC->AHB1RSTR &= ~(1U << 1)); }while(0)
#define GPIOC_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 2)); (RCC->AHB1RSTR &= ~(1U << 2)); }while(0)
#define GPIOD_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 3)); (RCC->AHB1RSTR &= ~(1U << 3)); }while(0)
#define GPIOE_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 4)); (RCC->AHB1RSTR &= ~(1U << 4)); }while(0)
#define GPIOF_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 5)); (RCC->AHB1RSTR &= ~(1U << 5)); }while(0)
#define GPIOG_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 6)); (RCC->AHB1RSTR &= ~(1U << 6)); }while(0)
#define GPIOH_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 7)); (RCC->AHB1RSTR &= ~(1U << 7)); }while(0)
#define GPIOI_REG_RESET()   do{ (RCC->AHB1RSTR |= (1U << 8)); (RCC->AHB1RSTR &= ~(1U << 8)); }while(0)

/*********************************************************************
 *                  I2C CLOCK DISABLE MACROS
 *********************************************************************/

#define I2C1_PCLK_DI()      (RCC->APB1ENR &= ~(1U << 21))
#define I2C2_PCLK_DI()      (RCC->APB1ENR &= ~(1U << 22))
#define I2C3_PCLK_DI()      (RCC->APB1ENR &= ~(1U << 23))

/*********************************************************************
 *                  SPI CLOCK DISABLE MACROS
 *********************************************************************/

#define SPI1_PCLK_DI()      (RCC->APB2ENR &= ~(1U << 12))
#define SPI2_PCLK_DI()      (RCC->APB1ENR &= ~(1U << 14))
#define SPI3_PCLK_DI()      (RCC->APB1ENR &= ~(1U << 15))

/*********************************************************************
 *                  USART CLOCK DISABLE MACROS
 *********************************************************************/

#define USART1_PCLK_DI()    (RCC->APB2ENR &= ~(1U << 4))
#define USART2_PCLK_DI()    (RCC->APB1ENR &= ~(1U << 17))
#define USART3_PCLK_DI()    (RCC->APB1ENR &= ~(1U << 18))

#define UART4_PCLK_DI()     (RCC->APB1ENR &= ~(1U << 19))
#define UART5_PCLK_DI()     (RCC->APB1ENR &= ~(1U << 20))

#define USART6_PCLK_DI()    (RCC->APB2ENR &= ~(1U << 5))

/*********************************************************************
 *                  SYSCFG CLOCK DISABLE MACROS
 *********************************************************************/

#define SYSCFG_PCLK_DI()    (RCC->APB2ENR &= ~(1U << 14))
/*********************************************************************
 *                  GPIO PORT CODE MACRO
 *********************************************************************/

#define GPIO_BASEADDR_TO_CODE(x)   ((x == GPIOA) ? 0 : \
                                    (x == GPIOB) ? 1 : \
                                    (x == GPIOC) ? 2 : \
                                    (x == GPIOD) ? 3 : \
                                    (x == GPIOE) ? 4 : \
                                    (x == GPIOF) ? 5 : \
                                    (x == GPIOG) ? 6 : \
                                    (x == GPIOH) ? 7 : 8)

/*********************************************************************
 *                  IRQ NUMBERS
 *********************************************************************/

#define IRQ_NO_EXTI0         6
#define IRQ_NO_EXTI1         7
#define IRQ_NO_EXTI2         8
#define IRQ_NO_EXTI3         9
#define IRQ_NO_EXTI4         10
#define IRQ_NO_EXTI9_5       23
#define IRQ_NO_EXTI15_10     40

/*********************************************************************
 *                  NVIC PRIORITY LEVELS
 *********************************************************************/

#define NVIC_IRQ_PR0         0
#define NVIC_IRQ_PR1         1
#define NVIC_IRQ_PR2         2
#define NVIC_IRQ_PR3         3
#define NVIC_IRQ_PR4         4
#define NVIC_IRQ_PR5         5
#define NVIC_IRQ_PR6         6
#define NVIC_IRQ_PR7         7
#define NVIC_IRQ_PR8         8
#define NVIC_IRQ_PR9         9
#define NVIC_IRQ_PR10        10
#define NVIC_IRQ_PR11        11
#define NVIC_IRQ_PR12        12
#define NVIC_IRQ_PR13        13
#define NVIC_IRQ_PR14        14
#define NVIC_IRQ_PR15        15

#include "stm32f407xx_gpio.h"

#endif /* INC_STM32F407XX_H_ */
