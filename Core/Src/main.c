/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "encoder.h"
#include "hall.h"
#include "motor.h"
#include "hcsr04.h"
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_COMMAND_MAX_LEN 64

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint8_t uart_rx_byte;
static char uart_rx_line[UART_COMMAND_MAX_LEN];
static char uart_cmd_line[UART_COMMAND_MAX_LEN];
static volatile uint8_t uart_rx_index = 0;
static volatile uint8_t uart_cmd_ready = 0;

uint32_t hall_tick = 0;
uint32_t encoder_tick = 0;
uint32_t oled_tick = 0;
uint32_t ultrasonic_tick = 0;
char oled_buf[36] = ""; // oled显示

float left_rps = 0.0f;
float right_rps = 0.0f;
float distance_cm = 0.0f;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void UART_StartReceiveIT(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 串口重定向
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

// 启动串口1的中断接收
static void UART_StartReceiveIT(void)
{
  if (HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1) != HAL_OK)
  {
    Error_Handler();
  }
}

// exti0 中断回调
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  Hall_EXTI_Callback(GPIO_Pin);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // 编码器初始化
  Encoder_Init();
  // 超声波测距初始化
  HCSR04_Init();
  // 左右减速电机&驱动模块初始化
  Motor_Init();
  // 配置 USART2 接收中断
  UART_StartReceiveIT();
  printf("UART motor control ready\r\n");
  printf("Commands: STOP | BRAKE | F 60 | B 60 | L 60 | R 60 | RL 60 | RR 60 | M -30 50\r\n");

  if (OLED_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  OLED_Clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint32_t now = HAL_GetTick();

    /*串口命令*/
    if (uart_cmd_ready != 0U)
    {
      char command[UART_COMMAND_MAX_LEN];

      __disable_irq();
      strncpy(command, uart_cmd_line, sizeof(command));
      uart_cmd_ready = 0U;
      __enable_irq();

      printf("recv:%s\r\n", command);

      Motor_ParseCommand(command);
    }

    /*编码器*/
    if ((now - encoder_tick) >= 100)
    {
      encoder_tick = now;

      left_rps = Encoder_GetRPS(ENCODER_LEFT);
      right_rps = Encoder_GetRPS(ENCODER_RIGHT);

      printf("encoder:left_rps=%f,right_rps=%f\r\n", left_rps, right_rps);
    }

    /*霍尔*/
    if ((now - hall_tick) >= 100)
    {
      hall_tick = now;

      printf("hall:PulseCnt=%d,left_rpm=%d,freq=%f\r\n", Hall_GetPulse(), Hall_GetRPM(), Hall_GetRPS());
    }

    /*超声波*/
    if ((now - ultrasonic_tick) >= 50)
    {
      ultrasonic_tick = now;

      HCSR04_Trigger();
      distance_cm = HCSR04_GetDistanceCm();

      printf("dist=%.1fcm\r\n", distance_cm);
    }

    /*OLED显示*/
    if ((now - oled_tick) >= 200)
    {
      oled_tick = now;
      // 左电机编码器(tim2 encoder)
      sprintf(oled_buf, "ML:%5.2f rps", left_rps);
      OLED_ShowString(0, 0, oled_buf, 16, 0);
      // 右电机编码器(tim3 encoder)
      sprintf(oled_buf, "MR:%5.2f rps", right_rps);
      OLED_ShowString(0, 2, oled_buf, 16, 0);
      // 左电机霍尔转速（exti0)
      sprintf(oled_buf, "LH:%5.2f rps", Hall_GetRPS());
      OLED_ShowString(0, 4, oled_buf, 16, 0);
      // 超声波测距(tim3 ic)
      sprintf(oled_buf, "D:%5.1fCM", distance_cm);
      OLED_ShowString(0, 6, oled_buf, 16, 0);
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    if ((uart_rx_byte == '\r') || (uart_rx_byte == '\n'))
    {
      if (uart_rx_index > 0U)
      {
        uart_rx_line[uart_rx_index] = '\0';
        if (uart_cmd_ready == 0U)
        {
          strncpy(uart_cmd_line, uart_rx_line, UART_COMMAND_MAX_LEN);
          uart_cmd_ready = 1U;
        }
        uart_rx_index = 0U;
      }
    }
    else if (uart_rx_index < (UART_COMMAND_MAX_LEN - 1U))
    {
      uart_rx_line[uart_rx_index++] = (char)uart_rx_byte;
    }
    else
    {
      uart_rx_index = 0U;
    }

    // 继续接收
    HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    uart_rx_index = 0U;
    UART_StartReceiveIT();
  }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
