#include "stm32f407xx.h"
#include <string.h>

/*
 * SPI2 PIN CONFIGURATION
 *
 * PB13 -> SCLK
 * PB14 -> MISO
 * PB15 -> MOSI
 *
 * LOOPBACK:
 * PB15 (MOSI) ---- PB14 (MISO)
 *
 * USER BUTTON:
 * PA0
 *
 * LED:
 * PD12
 */

SPI_Handle_t SPI2handle;

/********************************************************************
 * Global Buffers
 ********************************************************************/

char user_data[] = "N";
char read_buffer[2];

/********************************************************************
 * Delay
 ********************************************************************/

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i++);
}

/********************************************************************
 * SPI2 GPIO Initialization
 ********************************************************************/

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	/*
	 * SCLK -> PB13
	 */
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	/*
	 * MOSI -> PB15
	 */
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	/*
	 * MISO -> PB14
	 */
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_Init(&SPIPins);
}

/********************************************************************
 * USER BUTTON INIT
 * PA0
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

	/*
	 * EXTI0 IRQ Configuration
	 */
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0,
			               NVIC_IRQ_PR15);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0,
			                ENABLE);
}

/********************************************************************
 * SPI2 Peripheral Initialization
 ********************************************************************/

void SPI2_Init(void)
{
	SPI2handle.pSPIx = SPI2;

	SPI2handle.SPIConfig.SPI_BusConfig =
			SPI_BUS_CONFIG_FD;

	SPI2handle.SPIConfig.SPI_DeviceMode =
			SPI_DEVICE_MODE_MASTER;

	SPI2handle.SPIConfig.SPI_SclkSpeed =
			SPI_SCLK_SPEED_DIV16;

	SPI2handle.SPIConfig.SPI_DFF =
			SPI_DFF_8BITS;

	SPI2handle.SPIConfig.SPI_CPOL =
			SPI_CPOL_LOW;

	SPI2handle.SPIConfig.SPI_CPHA =
			SPI_CPHA_LOW;

	SPI2handle.SPIConfig.SPI_SSM =
			SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}

/********************************************************************
 * LED Initialization
 ********************************************************************/

void GPIO_LEDInit(void)
{
	GPIO_Handle_t GPIOLED;

	GPIOLED.pGPIOx = GPIOD;

	GPIOLED.GPIO_PinConfig.GPIO_PinNumber =
			GPIO_PIN_NO_12;

	GPIOLED.GPIO_PinConfig.GPIO_PinMode =
			GPIO_MODE_OUT;

	GPIOLED.GPIO_PinConfig.GPIO_PinSpeed =
			GPIO_SPEED_FAST;

	GPIOLED.GPIO_PinConfig.GPIO_PinPuPdControl =
			GPIO_NO_PUPD;

	GPIO_Init(&GPIOLED);
}

/********************************************************************
 * MAIN
 ********************************************************************/

int main(void)
{
	/*
	 * LED Init
	 */
	GPIO_LEDInit();

	/*
	 * Button Init
	 */
	Button_Init();

	/*
	 * SPI GPIO Init
	 */
	SPI2_GPIOInits();

	/*
	 * SPI Peripheral Init
	 */
	SPI2_Init();

	/*
	 * Enable SPI2 Interrupt in NVIC
	 */
	SPI_IRQInterruptConfig(IRQ_NO_SPI2,
			               ENABLE);

	SPI_IRQPriorityConfig(IRQ_NO_SPI2,
			              0);

	/*
	 * Software Slave Management Enable
	 */
	SPI_SSIConfig(SPI2,
			      ENABLE);

	/*
	 * Hardware NSS Disable
	 */
	SPI_SSOEConfig(SPI2,
			       DISABLE);

	/*
	 * Enable SPI Peripheral
	 */
	SPI_PeripheralControl(SPI2,
			              ENABLE);

	while(1)
	{

	}
}

/********************************************************************
 * EXTI0 IRQ Handler
 * Triggered when USER BUTTON pressed
 ********************************************************************/

void EXTI0_IRQHandler(void)
{
	/*
	 * Clear EXTI Pending Bit
	 */
	GPIO_IRQHandling(GPIO_PIN_NO_0);

	/*
	 * Small Debounce Delay
	 */
	delay();

	/*
	 * Start SPI Interrupt Transmission + Reception
	 */
	SPI_SendDataReceiveIT(&SPI2handle,
			              (uint8_t*)user_data,
			              (uint8_t*)read_buffer,
			              strlen(user_data));
}

/********************************************************************
 * SPI2 IRQ Handler
 ********************************************************************/

void SPI2_IRQHandler(void)
{
	SPI_IRQHandling(&SPI2handle);
}

/********************************************************************
 * SPI Callback Function
 ********************************************************************/

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,
		                          uint8_t AppvEv)
{
	/*
	 * Reception Complete
	 */
	if(AppvEv == SPI_EVENT_RX_CMPLT)
	{
		/*
		 * Null Terminate
		 */
		read_buffer[strlen(user_data)] = '\0';

		/*
		 * Turn ON LED
		 */
		GPIO_WriteToOutputPin(GPIOD,
				              GPIO_PIN_NO_12,
				              SET);

		/*
		 * Check read_buffer in debugger
		 */
	}
}
