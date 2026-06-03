#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

/* PB9  -> TRIG OUTPUT
 * PB10 -> ECHO INPUT
 * PD12 -> LED OUTPUT
 */

#define __NOP() __asm__ volatile ("nop")

uint32_t get_distance(void);
void delay_us(uint32_t us);
void gpio_init(void);

void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

uint32_t dist;

int main(void)
{
	gpio_init();

	while(1)
	{
		dist = get_distance();

		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);

		delay();
	}

	return 0;
}

uint32_t get_distance(void)
{
	uint32_t distance = 0;

	/* Trigger Pulse */
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_9, GPIO_PIN_RESET);
	delay_us(5);

	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_9, GPIO_PIN_SET);
	delay_us(10);

	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_9, GPIO_PIN_RESET);

	/* Wait for Echo HIGH */
	uint32_t timeout = 1000000;

	while((GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_10) == GPIO_PIN_RESET) &&
	      (timeout > 0))
	{
		timeout--;
	}

	if(timeout == 0)
	{
		return 0;
	}

	/* Measure Echo Pulse Width */
	uint32_t echo_duration = 0;

	timeout = 1000000;

	while((GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_10) == GPIO_PIN_SET) &&
	      (timeout > 0))
	{
		echo_duration++;
		timeout--;
	}

	/* Distance in cm */
	distance = echo_duration / 60;

	return distance;
}

void delay_us(uint32_t us)
{
	volatile uint32_t count = us * 21;

	while(count--)
	{
		__NOP();
	}
}

void gpio_init(void)
{
	GPIO_Handle_t gpio_trig, gpio_echo, gpio_led;

	memset(&gpio_trig, 0, sizeof(gpio_trig));
	memset(&gpio_echo, 0, sizeof(gpio_echo));
	memset(&gpio_led, 0, sizeof(gpio_led));

	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_PeriClockControl(GPIOD, ENABLE);

	/* TRIG PB9 */
	gpio_trig.pGPIOx = GPIOB;
	gpio_trig.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	gpio_trig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_trig.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	gpio_trig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpio_trig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&gpio_trig);

	/* ECHO PB10 */
	gpio_echo.pGPIOx = GPIOB;
	gpio_echo.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	gpio_echo.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpio_echo.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpio_echo.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&gpio_echo);

	/* LED PD12 */
	gpio_led.pGPIOx = GPIOD;
	gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_led.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&gpio_led);
}
