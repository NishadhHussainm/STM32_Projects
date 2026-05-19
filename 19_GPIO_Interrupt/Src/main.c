/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : GPIO Interrupt Example
 ******************************************************************************
 */

#include <stdint.h>
#include <string.h>

#include "stm32f407xx.h"

void delay(void)
{
	for(volatile uint32_t i = 0 ; i < 500000 ; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed, GPIOBtn;

	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GPIOBtn,0,sizeof(GPIOBtn));

	/*****************************************************************
	 * LED CONFIGURATION : PD12
	 *****************************************************************/

	GPIO_PeriClockControl(GPIOD, ENABLE);

	GpioLed.pGPIOx = GPIOD;

	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GpioLed);

	/*****************************************************************
	 * BUTTON CONFIGURATION : PA0
	 *****************************************************************/

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIOBtn.pGPIOx = GPIOA;

	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	/*****************************************************************
	 * IRQ CONFIGURATION
	 *****************************************************************/

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PR0);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);

    while(1)
    {

    }
}

/*****************************************************************
 * ISR
 *****************************************************************/

void EXTI0_IRQHandler(void)
{
	delay();

	GPIO_IRQHandling(GPIO_PIN_NO_0);

	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}
