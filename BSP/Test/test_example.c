/**
  ******************************************************************************
  * @file    test_example.c
  * @brief   测试系统使用示例
  * @note    将这些代码添加到你的 main.c 中
  ******************************************************************************
  */

/*
 * 1. 在 main.c 顶部添加头文件
 */
#include "test.h"

/*
 * 2. 在 main() 函数中初始化测试系统
 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART1_UART_Init();

    // 初始化各模块
    Motor_Init();
    Encoder_Init();
    HCSR04_Init();

    // 初始化测试系统
    Test_Init();
    Test_PrintMenu();

    while (1)
    {
        // 方法1: 直接调用测试函数
        // Test_Motor_Left();
        // HAL_Delay(3000);

        // 方法2: 通过枚举调用
        // Test_RunModule(TEST_MOTOR_LEFT);
        // HAL_Delay(3000);

        // 方法3: 串口命令控制（见下方）
    }
}

/*
 * 3. 串口命令控制示例
 *
 * 在串口接收中断或轮询中添加：
 */
void UART_CommandHandler(char *cmd)
{
    if (cmd == NULL || cmd[0] == '\0') {
        return;
    }

    switch (cmd[0]) {
        case '0':
            Test_RunModule(TEST_MOTOR_LEFT);
            break;
        case '1':
            Test_RunModule(TEST_MOTOR_RIGHT);
            break;
        case '2':
            Test_RunModule(TEST_MOTOR_FORWARD);
            break;
        case '3':
            Test_RunModule(TEST_MOTOR_BACKWARD);
            break;
        case '4':
            Test_RunModule(TEST_MOTOR_TURN_LEFT);
            break;
        case '5':
            Test_RunModule(TEST_MOTOR_TURN_RIGHT);
            break;
        case '6':
            Test_RunModule(TEST_MOTOR_ROTATE_LEFT);
            break;
        case '7':
            Test_RunModule(TEST_MOTOR_ROTATE_RIGHT);
            break;
        case '8':
            Test_RunModule(TEST_ENCODER_LEFT);
            break;
        case '9':
            Test_RunModule(TEST_ENCODER_RIGHT);
            break;
        case 'a':
        case 'A':
            Test_RunModule(TEST_ENCODER_BOTH);
            break;
        case 'b':
        case 'B':
            Test_RunModule(TEST_HCSR04);
            break;
        case 'c':
        case 'C':
            Test_RunModule(TEST_MOTOR_ENCODER_SYNC);
            break;
        case 'd':
        case 'D':
            Test_RunModule(TEST_ALL);
            break;
        case 'm':
        case 'M':
            Test_PrintMenu();
            break;
        default:
            printf("未知命令: %c\r\n", cmd[0]);
            printf("输入 'm' 显示菜单\r\n");
            break;
    }
}

/*
 * 4. 快速测试示例（在 main 函数的 while(1) 之前）
 */
void QuickTest_Example(void)
{
    // 快速测试：判断左右电机是否颠倒
    printf("\r\n=== 快速测试开始 ===\r\n");

    // 测试1: 左电机
    printf("\r\n[1/4] 测试左电机（观察是否是左侧转动）\r\n");
    Test_Motor_Left();
    HAL_Delay(2000);

    // 测试2: 右电机
    printf("\r\n[2/4] 测试右电机（观察是否是右侧转动）\r\n");
    Test_Motor_Right();
    HAL_Delay(2000);

    // 测试3: 前进
    printf("\r\n[3/4] 测试前进（观察小车是否前进）\r\n");
    Test_Motor_Forward();
    HAL_Delay(2000);

    // 测试4: 编码器同步
    printf("\r\n[4/4] 测试编码器同步\r\n");
    Test_Motor_Encoder_Sync();

    printf("\r\n=== 快速测试完成 ===\r\n");
    printf("如果发现问题，请参考 motor_test.md 文档\r\n");
}

/*
 * 5. 定时测试示例（使用定时器）
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t test_step = 0;

    if (htim->Instance == TIM6) {  // 假设使用 TIM6，每秒触发一次
        switch (test_step) {
            case 0:
                Test_Motor_Left();
                break;
            case 1:
                Test_Motor_Right();
                break;
            case 2:
                Test_Motor_Forward();
                break;
            case 3:
                Test_Encoder_Both();
                break;
            case 4:
                Test_HCSR04();
                test_step = 0;  // 循环
                return;
        }
        test_step++;
    }
}

/*
 * 6. 按键触发测试示例
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint8_t test_index = 0;

    if (GPIO_Pin == GPIO_PIN_0) {  // 假设使用 PA0 作为测试按键
        Test_RunModule((Test_Module_TypeDef)test_index);
        test_index++;
        if (test_index >= TEST_ALL) {
            test_index = 0;
        }
    }
}

/*
 * 7. 完整的 main.c 示例
 */
/*
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor.h"
#include "encoder.h"
#include "hcsr04.h"
#include "test.h"
#include <stdio.h>

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART1_UART_Init();

    Motor_Init();
    Encoder_Init();
    HCSR04_Init();

    Test_Init();
    Test_PrintMenu();

    // 运行快速测试（可选）
    // QuickTest_Example();

    while (1)
    {
        // 这里可以添加串口接收处理
        // 或者直接调用测试函数
    }
}
*/
