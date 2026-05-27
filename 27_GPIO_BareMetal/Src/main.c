#include "stm32f407xx_gpio.h"
#include <string.h>

/*
 * HC-SR04 Connections
 *
 * PB9  ---- TRIG  (OUTPUT)
 * PB10 ---- ECHO  (INPUT)
 *
 * LED
 * PD12 ---- LED OUTPUT
 */

#define __NOP() __asm__ volatile ("nop")

volatile uint32_t distance;

/*********************************************************************
 * Delay
 *********************************************************************/

void delay(void)
{
    for(volatile uint32_t i = 0; i < 500000; i++);
}

/*********************************************************************
 * Microsecond Delay
 *********************************************************************/

void delay_us(uint32_t us)
{
    volatile uint32_t count = us * 21;

    while(count--)
    {
        __NOP();
    }
}

/*********************************************************************
 * GPIO Initialization
 *********************************************************************/

void gpio_init(void)
{
    GPIO_Handle_t gpio_trig;
    GPIO_Handle_t gpio_echo;
    GPIO_Handle_t gpio_led;

    memset(&gpio_trig,0,sizeof(gpio_trig));
    memset(&gpio_echo,0,sizeof(gpio_echo));
    memset(&gpio_led,0,sizeof(gpio_led));

    /********************************************************
     * Enable Clocks
     ********************************************************/

    GPIO_PeriClockControl(GPIOB, ENABLE);
    GPIO_PeriClockControl(GPIOD, ENABLE);

    /********************************************************
     * PB9 -> TRIG OUTPUT
     ********************************************************/

    gpio_trig.pGPIOx = GPIOB;

    gpio_trig.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    gpio_trig.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    gpio_trig.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    gpio_trig.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio_trig.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&gpio_trig);

    /********************************************************
     * PB10 -> ECHO INPUT
     ********************************************************/

    gpio_echo.pGPIOx = GPIOB;

    gpio_echo.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
    gpio_echo.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    gpio_echo.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /*
     * Pulldown avoids floating
     */
    gpio_echo.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;

    GPIO_Init(&gpio_echo);

    /********************************************************
     * PD12 -> LED OUTPUT
     ********************************************************/

    gpio_led.pGPIOx = GPIOD;

    gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    gpio_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&gpio_led);
}

/*********************************************************************
 * Ultrasonic Distance Function
 *********************************************************************/

uint32_t get_distance(void)
{
    uint32_t echo_duration = 0;
    uint32_t timeout = 1000000;

    /********************************************************
     * Send 10us Trigger Pulse
     ********************************************************/

    GPIO_WriteToOutputPin(GPIOB,
                          GPIO_PIN_NO_9,
                          GPIO_PIN_RESET);

    delay_us(5);

    GPIO_WriteToOutputPin(GPIOB,
                          GPIO_PIN_NO_9,
                          GPIO_PIN_SET);

    delay_us(10);

    GPIO_WriteToOutputPin(GPIOB,
                          GPIO_PIN_NO_9,
                          GPIO_PIN_RESET);

    /********************************************************
     * Wait for Echo HIGH
     ********************************************************/

    while((GPIO_ReadFromInputPin(GPIOB,
                                 GPIO_PIN_NO_10)
                                 == GPIO_PIN_RESET)
                                 && timeout > 0)
    {
        timeout--;
    }

    if(timeout == 0)
    {
        return 0;
    }

    /********************************************************
     * Measure HIGH duration
     ********************************************************/

    timeout = 1000000;

    while((GPIO_ReadFromInputPin(GPIOB,
                                 GPIO_PIN_NO_10)
                                 == GPIO_PIN_SET)
                                 && timeout > 0)
    {
        echo_duration++;
        timeout--;
    }

    if(timeout == 0)
    {
        return 0;
    }

    /********************************************************
     * Approximate Distance Conversion
     ********************************************************/

    return (echo_duration / 58);
}

/*********************************************************************
 * Main Function
 *********************************************************************/

int main(void)
{
    gpio_init();

    while(1)
    {
        distance = get_distance();

        /*
         * Blink LED
         */

        GPIO_ToggleOutputPin(GPIOD,
                             GPIO_PIN_NO_12);

        delay();
    }

    return 0;
}
