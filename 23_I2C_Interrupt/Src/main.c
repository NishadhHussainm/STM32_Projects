/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : STM32F407 I2C Interrupt Example
 *
 * USER BUTTON -> PA0
 * I2C1_SCL    -> PB6
 * I2C1_SDA    -> PB7
 *
 * On button press:
 * STM32 sends "Nishadh" using I2C interrupt
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <string.h>

#include "stm32f407xx.h"

#define MY_ADDR        0x61
#define SLAVE_ADDR     0x68

I2C_Handle_t I2C1Handle;

/********************************************************************
 * Global Data
 ********************************************************************/

uint8_t tx_data[] = "Nishadh";

/********************************************************************
 * Delay
 ********************************************************************/

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i++);
}

/********************************************************************
 * Button Init
 * PA0 -> User Button
 ********************************************************************/

void Button_Init(void)
{
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOA;

	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&GPIOBtn);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PR15);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);
}

/********************************************************************
 * PB6 -> SCL
 * PB7 -> SDA
 ********************************************************************/

void I2C1_GPIOInit(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;

	I2CPins.GPIO_PinConfig.GPIO_PinMode =
			GPIO_MODE_ALTFN;

	I2CPins.GPIO_PinConfig.GPIO_PinOPtype =
			GPIO_OP_TYPE_OD;

	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl =
			GPIO_NO_PUPD;

	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode =
			4;

	I2CPins.GPIO_PinConfig.GPIO_PinSpeed =
			GPIO_SPEED_FAST;

	/*
	 * SCL -> PB6
	 */
	I2CPins.GPIO_PinConfig.GPIO_PinNumber =
			GPIO_PIN_NO_6;

	GPIO_Init(&I2CPins);

	/*
	 * SDA -> PB7
	 */
	I2CPins.GPIO_PinConfig.GPIO_PinNumber =
			GPIO_PIN_NO_7;

	GPIO_Init(&I2CPins);
}

/********************************************************************
 * I2C1 Init
 ********************************************************************/

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;

	I2C1Handle.I2C_Config.I2C_AckControl =
			I2C_ACK_ENABLE;

	I2C1Handle.I2C_Config.I2C_DeviceAddress =
			MY_ADDR;

	I2C1Handle.I2C_Config.I2C_FMDutyCycle =
			I2C_FM_DUTY_2;

	I2C1Handle.I2C_Config.I2C_SCLSpeed =
			I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}

/********************************************************************
 * Main
 ********************************************************************/

int main(void)
{
	/*
	 * Button Init
	 */
	Button_Init();

	/*
	 * I2C GPIO Init
	 */
	I2C1_GPIOInit();

	/*
	 * I2C Peripheral Init
	 */
	I2C1_Inits();

	/*
	 * Enable I2C Peripheral
	 */
	I2C_PeripheralControl(I2C1, ENABLE);

	/*
	 * Enable ACK
	 */
	I2C_ManageACKing(I2C1, ENABLE);

	/*
	 * Enable I2C Interrupts
	 */
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);

	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	while(1)
	{

	}
}

/********************************************************************
 * Button Interrupt Handler
 ********************************************************************/

void EXTI0_IRQHandler(void)
{
	/*
	 * Clear Pending Bit
	 */
	GPIO_IRQHandling(GPIO_PIN_NO_0);

	/*
	 * Debounce Delay
	 */
	delay();

	/*
	 * Send "Nishadh" using I2C Interrupt
	 */
	while(I2C_MasterSendDataIT(&I2C1Handle,
			                   tx_data,
			                   strlen((char*)tx_data),
			                   SLAVE_ADDR,
			                   I2C_DISABLE_SR)
			                   != I2C_READY);
}

/********************************************************************
 * I2C Event IRQ Handler
 ********************************************************************/

void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}

/********************************************************************
 * I2C Error IRQ Handler
 ********************************************************************/

void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}

/********************************************************************
 * I2C Application Callback
 ********************************************************************/

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,
		                          uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		/*
		 * Transmission Complete
		 */
	}
}
