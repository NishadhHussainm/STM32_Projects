#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/* W25Q Flash Commands */
#define W25Q_WRITE_ENABLE      0x06
#define W25Q_READ_STATUS_REG1  0x05
#define W25Q_READ_DATA         0x03
#define W25Q_PAGE_PROGRAM      0x02
#define W25Q_SECTOR_ERASE      0x20
#define W25Q_CHIP_ID           0x90

#define BASE_ADDRESS           0x000000

/* Ultrasonic */
#define TRIG_PIN               GPIO_PIN_10
#define ECHO_PIN               GPIO_PIN_9
#define ECHO_PORT              GPIOB
#define TRIG_PORT              GPIOB

/* Push Button */
#define B1_Pin                 GPIO_PIN_0
#define B1_GPIO_Port           GPIOA

/* Flash CS */
#define W25_CS_SELECT()        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define W25_CS_DESELECT()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

/* Function Prototypes */
void UART_Log(char* msg);
void W25_Wait_Busy(void);
void W25_Write_Enable(void);
void W25_Write_Page(uint32_t address, uint8_t *pData, uint16_t size);
void W25_Read_Data(uint32_t address, uint8_t *pBuffer, uint16_t size);
void W25_Erase_Sector(uint32_t address);

void delay_us(uint16_t us);
int get_distance(void);

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
