/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <pwm.h>
#include <stdio.h>
#include <usart.h>
#include <i2c.h>
#include "MPU6050.h"
#include "Uart.h"
#include "UartProtocol.h"
#include "TelemetryStream.h"
#include "FSM.h"
#include "ControlLoop.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t uartTaskHandle;
const osThreadAttr_t uartTask_attributes = {
  .name = "uartTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


uint8_t uart2_rx_byte;

osThreadId_t UartStreamTaskHandle;
const osThreadAttr_t UartStreamTask_attributes = {
  .name = "UartStreamTask",
  .stack_size = 384 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

osThreadId_t TelemetryStreamTaskHandle;
const osThreadAttr_t TelemetryStreamTask_attributes = {
  .name = "TelemetryStreamTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t StateMachineTaskHandle;
const osThreadAttr_t StateMachineTask_attributes = {
  .name = "StateMachineTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh2,
};

osThreadId_t SystemHealthMonitorTaskHandle;
const osThreadAttr_t SystemHealthMonitorTask_attributes = {
  .name = "SystemHealthMonitorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
osThreadId_t PropulsionTaskHandle;
const osThreadAttr_t PropulsionTask_attributes = {
  .name = "PropulsionTask",
  .stack_size = 384 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartUartTask(void *argument);
void StartWiredUARTStream(void *argument);
void StartStateMachineTask(void *argument);
void StartTelemetryStreamTask(void *argument);
void StartSystemHealthMonitorTask(void *argument);
void Propulsion_Task(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  //uartTaskHandle = osThreadNew(StartUartTask, NULL, &uartTask_attributes);
  UartStreamTaskHandle = osThreadNew(StartWiredUARTStream, NULL, &UartStreamTask_attributes);
  StateMachineTaskHandle = osThreadNew(StartStateMachineTask, NULL, &StateMachineTask_attributes);
  TelemetryStreamTaskHandle = osThreadNew(StartTelemetryStreamTask, NULL, &TelemetryStreamTask_attributes);
  PropulsionTaskHandle = osThreadNew(Propulsion_Task, NULL, &PropulsionTask_attributes);
 // SystemHealthMonitorTaskHandle = osThreadNew(StartSystemHealthMonitorTask, NULL, &SystemHealthMonitorTask_attributes);
  if (UartStreamTaskHandle == NULL) {
      printf("Failed to create UART task\r\n");
  }
  if (StateMachineTaskHandle == NULL) {
      printf("Failed to create state machine task\r\n");
  }
  if (TelemetryStreamTaskHandle == NULL) {
      printf("Failed to create telemetry task\r\n");
  }

  if (PropulsionTaskHandle == NULL) {
      printf("Failed to create propulsion task\r\n");
  }
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  PWM_Init();


  printf("Started default task...\n");

  uint8_t test = MPU6050_Init(&hi2c1);

    osDelay(100);

  MPU6050_t data;


  while(1){

    MPU6050_Read_All(&data);
    printf("%f, %f, %f\n", data.Ax, data.Ay, data.Az);

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
// 	HAL_UART_Receive_IT(&huart1, buffer, 1);
// 	HAL_UART_Transmit(&huart1, buffer, 1, 0xFFFF);
// }

void StartUartTask(void *argument){
    uint8_t buffer[1];
    HAL_UART_Receive_IT(&huart1, buffer, 1);

    while(1){

    }
}

void StartWiredUARTStream(void *argument)
{
    UART3_INIT_RxDMA();
    printf("\r\n");
    printf("|Uart Task has been initialized...|\r\n");
    printf("\r\n");
    Packet_Sent_t rxPacket;

    for (;;)
    {
        UART3_Traverse_RxDMA();
        if (BuildRxPacket(&rxPacket, 0))
        {
  
            Protocol_ParsePacket(&rxPacket);
        }
        Protocol_UpdateThroughput(); //count the throughput 

        osDelay(1);
    }
  /* USER CODE END 5 */
}

void StartStateMachineTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  printf("\r\n");
  printf("|State Machine Task has been initialized...|\r\n");
  printf("\r\n");
  /* Infinite loop */
  for(;;)
  {
    SeaSTAR_FSM();
    osDelay(80);
  }
  /* USER CODE END 5 */
}

void StartSystemHealthMonitorTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  printf("\r\n");
  printf("|Initializing system health monitor task...|\r\n");
  printf("\r\n");
  /* Infinite loop */
  for(;;)
  {

    osDelay(200);
  }
  /* USER CODE END 5 */
}
void Propulsion_Task(void *argument)
{
    // static uint32_t min_age = 100000;
    // static uint32_t max_age = 0;
    // static uint32_t last_report = 0;
    PWM_Init();
    printf("\r\n");
    printf("|Thruster Stream Task has been initialized...|\r\n");
    printf("\r\n");
    while (1)
    {
       
 

        osDelay(1); // small consistent loop rate
    }
}
void StartTelemetryStreamTask(void *argument)
{
    printf("\r\n");
    printf("|Telemetry Stream Task has been initialized...|\r\n");
    printf("\r\n");
    TickType_t last_wake = xTaskGetTickCount();

    const TickType_t period = pdMS_TO_TICKS(50);   // quick fake data for the land test 

    TelemetryStream_Init();

    for (;;)
    {
        TelemetryStream_SendEnvironmental();
        TelemetryStream_SendOrientation();

        vTaskDelayUntil(&last_wake, period);
        osDelay(50);

    }
}




/* USER CODE END Application */

