#include <stdio.h>
#include <string.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

UART_HandleTypeDef huart2;

char user_msg[100];

/* Private function prototypes */
void SystemClock_Config(void);

static void prvSetupHardware(void);
static void prvSetupUart(void);

void vTask1(void *params);
void vTask2(void *params);

/* Mutex Handle */
SemaphoreHandle_t xMutex;

/****************************************************
 * UART Print Function
 ****************************************************/
void printmsg(char *msg)
{
    for(uint32_t i = 0 ; i < strlen(msg) ; i++)
    {
        HAL_UART_Transmit(&huart2,
                         (uint8_t*)&msg[i],
                         1,
                         100);
    }
}

/****************************************************
 * MAIN
 ****************************************************/
int main(void)
{
    HAL_Init();

    SystemClock_Config();

    prvSetupHardware();

    sprintf(user_msg,
            "Demo of Mutex FreeRTOS\r\n");
    printmsg(user_msg);

    /* Create Mutex */
    xMutex = xSemaphoreCreateMutex();

    if(xMutex != NULL)
    {
        xTaskCreate(vTask1,
                    "Task1",
                    500,
                    NULL,
                    1,
                    NULL);

        xTaskCreate(vTask2,
                    "Task2",
                    500,
                    NULL,
                    1,
                    NULL);

        vTaskStartScheduler();
    }
    else
    {
        sprintf(user_msg,
                "Mutex Creation Failed\r\n");

        printmsg(user_msg);
    }

    while(1);
}

/****************************************************
 * TASK1
 ****************************************************/
void vTask1(void *params)
{
    const char *pcTaskName =
            "Task 1 is running\r\n";

    while(1)
    {
        xSemaphoreTake(
                xMutex,
                portMAX_DELAY);

        sprintf(user_msg,
                "%s",
                pcTaskName);

        printmsg(user_msg);

        xSemaphoreGive(
                xMutex);

        vTaskDelay(
                pdMS_TO_TICKS(500));
    }
}

/****************************************************
 * TASK2
 ****************************************************/
void vTask2(void *params)
{
    const char *pcTaskName =
            "Task 2 is running\r\n";

    while(1)
    {
        xSemaphoreTake(
                xMutex,
                portMAX_DELAY);

        sprintf(user_msg,
                "%s",
                pcTaskName);

        printmsg(user_msg);

        xSemaphoreGive(
                xMutex);

        vTaskDelay(
                pdMS_TO_TICKS(500));
    }
}

/****************************************************
 * UART INIT
 ****************************************************/
static void prvSetupUart(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart2);
}

/****************************************************
 * HARDWARE INIT
 ****************************************************/
static void prvSetupHardware(void)
{
    prvSetupUart();
}

/****************************************************
 * CLOCK CONFIG
 ****************************************************/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(
            PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType =
            RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
            RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
            RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
            RCC_PLL_ON;

    RCC_OscInitStruct.PLL.PLLSource =
            RCC_PLLSOURCE_HSI;

    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 50;
    RCC_OscInitStruct.PLL.PLLP =
            RCC_PLLP_DIV4;

    RCC_OscInitStruct.PLL.PLLQ = 7;

    if(HAL_RCC_OscConfig(
            &RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_HCLK |
            RCC_CLOCKTYPE_SYSCLK |
            RCC_CLOCKTYPE_PCLK1 |
            RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
            RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider =
            RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
            RCC_HCLK_DIV4;

    RCC_ClkInitStruct.APB2CLKDivider =
            RCC_HCLK_DIV2;

    if(HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/****************************************************
 * ERROR HANDLER
 ****************************************************/
void Error_Handler(void)
{
    __disable_irq();

    while(1)
    {

    }
}
