/*
 * 01_spi_tx_testings.c
 *
 *  Created on: May 19, 2026
 *      Author: Nish
 */

/*********************************************************************
 * SPI2 LOOPBACK CONNECTION
 *
 * PB15 (MOSI) -------- PB14 (MISO)
 *
 * AF5 -> SPI2
 *
 * PB13 -> SCLK
 * PB14 -> MISO
 * PB15 -> MOSI
 * PB12 -> NSS
 *********************************************************************/

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"

#include <string.h>

/*********************************************************************
 * Delay Function
 *********************************************************************/

void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

/*********************************************************************
 * SPI2 GPIO Initialization
 *
 * Configure:
 * PB13 -> SPI2_SCLK
 * PB14 -> SPI2_MISO
 * PB15 -> SPI2_MOSI
 * PB12 -> Software NSS
 *********************************************************************/

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins, NSSpin;

	/*
	 * SPI2 GPIO Configuration
	 */
	SPIPins.pGPIOx = GPIOB;

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	/*
	 * PB13 -> SCLK
	 */
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	/*
	 * PB15 -> MOSI
	 */
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	/*
	 * PB14 -> MISO
	 */
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIO_Init(&SPIPins);

	/*
	 * PB12 -> NSS Pin Configuration
	 */
	NSSpin.pGPIOx = GPIOB;

	NSSpin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	NSSpin.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	NSSpin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	NSSpin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	NSSpin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&NSSpin);

	/*
	 * Keep NSS HIGH initially
	 */
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, SET);
}

/*********************************************************************
 * SPI2 Peripheral Initialization
 *********************************************************************/

void SPI2_Init(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;

	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	/*
	 * Initialize SPI2 Peripheral
	 */
	SPI_Init(&SPI2handle);
}

/*********************************************************************
 * Global Buffers
 *********************************************************************/

char user_data[] = "Nishadh";

char read_buffer[10];

/*********************************************************************
 * Main Function
 *********************************************************************/

int main(void)
{
	/*
	 * Initialize SPI2 GPIO Pins
	 */
	SPI2_GPIOInits();

	/*
	 * Initialize SPI2 Peripheral
	 */
	SPI2_Init();

	/*
	 * Set SSI Bit
	 *
	 * Prevents MODF Error
	 */
	SPI_SSIConfig(SPI2, ENABLE);

	/*
	 * Disable Hardware NSS Output
	 */
	SPI_SSOEConfig(SPI2, DISABLE);

	/*
	 * Enable SPI2 Peripheral
	 */
	SPI_PeripheralControl(SPI2, ENABLE);

	delay();

	/*****************************************************************
	 * Start SPI Loopback Communication
	 *****************************************************************/

	/*
	 * Pull NSS LOW
	 * Select Slave
	 */
	GPIO_WriteToOutputPin(GPIOB,
			              GPIO_PIN_NO_12,
						  RESET);

	/*
	 * Transmit and Receive Data
	 *
	 * MOSI -> MISO Loopback
	 */
	SPI_TransmitReceive(SPI2,
			            (uint8_t*)user_data,
						(uint8_t*)read_buffer,
						strlen(user_data));

	/*
	 * Pull NSS HIGH
	 * Deselect Slave
	 */
	GPIO_WriteToOutputPin(GPIOB,
			              GPIO_PIN_NO_12,
						  SET);

	/*
	 * Add NULL Character
	 */
	read_buffer[strlen(user_data)] = '\0';

	/*
	 * Infinite Loop
	 */
	while(1);

	return 0;
}
