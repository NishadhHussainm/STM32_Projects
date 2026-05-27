/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: May 19, 2026
 *      Author: Nish
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
/*
 * Peripheral Clock setup
 */
/********************************************************************
 * @fn                  - SPI_PERICLOCLCONTROL
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx ==  SPI1)
			SPI1_PCLK_EN();
		else if(pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if(pSPIx == SPI3)
			SPI3_PCLK_EN();
	}
	else
	{
		if(pSPIx ==  SPI1)
			SPI1_PCLK_DI();
		else if(pSPIx == SPI2)
			SPI2_PCLK_DI();
		else if(pSPIx == SPI3)
			SPI3_PCLK_DI();
	}
}


/*
 * Init and De-init
 */
/********************************************************************
 * @fn                  - SPI_Init
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	// enable peripheral clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// configure SPI_CR1 register

	uint32_t temp = 0;

	// configure device mode
	temp |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	// configure bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		// BIDIMODE should be cleared
		temp &= ~(1 << 15);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// BIDIMODE should be set
		temp |= (1 << 15);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// BIDIMODE should be cleared
		temp &= ~(1 << 15);
		// RXONLY bit must be set
		temp |= (1 << 10);
	}

	// configure spi serial clock speed(baud rate)
	temp |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR ;  // spi control registers - registers in bit position

	// configure DFF
	temp |= pSPIHandle->SPIConfig.SPI_DFF << 11;
	// configure CPOL
	temp |= pSPIHandle->SPIConfig.SPI_CPOL << 1;

	// configure CPHA
	temp |= pSPIHandle->SPIConfig.SPI_CPHA << 0;

	temp |= pSPIHandle->SPIConfig.SPI_SSM << 9;

	pSPIHandle->pSPIx->CR1 = temp;
}

/********************************************************************
 * @fn                  - 	SPI_DeInit
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{

}




uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx,uint32_t FlagName)
{
	if(pSPIx->SR && FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}


/*
 * Data send and receive
 */
/********************************************************************
 * @fn                  - SPI_SendData
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - This is a blocking call  (2 while loop)
 *
 *
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer,uint32_t len)
{
	while(len > 0)    // is used to block
	{
		// wait until TXE is set
	    while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);

	    // check the DFF bit in CR1
	    if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
	    {
	    	// 16 bit DFF
	    	// load the data to DR
	    	pSPIx->DR = *((uint16_t*)pTxBuffer);
	    	len--;
	    	len--;
	    	(uint16_t*)(pTxBuffer)++;
	    }
	    else
	    {
	    	// 8 bit DFF
	    	pSPIx->DR = *(pTxBuffer);
	    	len--;
	    	pTxBuffer++;
	    }
	}
}


/********************************************************************
 * @fn                  - SPI_Receve Data
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t len)
{
	while(len > 0)
	{
		// wait until RXNE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET);

		// check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16 bit DFF
			// load the data from DR to Rxbuffer
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			len--;
			len--;
			(uint16_t*)(pRxBuffer)++;
		}
		else
		{
			// 8 bit DFF
			 *(pRxBuffer) = pSPIx->DR;
			len--;
			pRxBuffer++;
		}
	}
}


/********************************************************************
 * @fn                  - SPI_Transmitt_Receve Data
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */


void SPI_TransmitReceive(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint8_t *pRxBuffer, uint32_t len)
{
	while(len > 0)
	{
	    // wait TX empty
	    while(!(SPI2->SR & (1 << SPI_SR_TXE)));

	    // send data
	    SPI2->DR = *pTxBuffer;

	    // wait RX not empty
	    while(!(SPI2->SR & (1 << SPI_SR_RXNE)));

	    // read data
	    *pRxBuffer = SPI2->DR;

	    pTxBuffer++;
	    pRxBuffer++;
	    len--;
	}

    // Wait until SPI not busy
    while(pSPIx->SR & (1 << SPI_SR_BSY));

    // Clear OVR
    uint8_t temp;
    temp = pSPIx->DR;
    temp = pSPIx->SR;
}

/********************************************************************
 * @fn                  - SPI_PeripheralControl
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE); //Enable the SPI Enable
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); //	Disable the SPI
}
}


/********************************************************************
 * @fn                  -  SPI_SSOIConfig
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}


/********************************************************************
 * @fn                  - SPI_SSOEConfig
 *
 * @brief               - This function enables or disables peripheral clock for given GPIO port
 *
 * @param[in]           - base address of the GPIO peripheral
 * @param[in]           - enable or disbale macros
 * @param[in]           -
 *
 * @return              - none
 *
 * @Note                - none
 *
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}

}

