# 电机测试指南

## 如何判断左右电机是否颠倒

### 测试步骤：

1. **单独测试左电机**
   ```c
   Motor_TestLeftMotor(50, 2000);  // 左电机以50%速度运行2秒
   ```
   - 观察：应该是**左侧**电机转动
   - 如果是右侧电机转动，说明定义颠倒了

2. **单独测试右电机**
   ```c
   Motor_TestRightMotor(50, 2000);  // 右电机以50%速度运行2秒
   ```
   - 观察：应该是**右侧**电机转动
   - 如果是左侧电机转动，说明定义颠倒了

3. **测试前进**
   ```c
   Motor_Forward(50);  // 前进
   HAL_Delay(2000);
   Motor_StopAll();
   ```
   - 观察：两个电机应该**同向转动**，小车**前进**
   - 如果小车后退，说明两个电机方向都反了
   - 如果小车原地转，说明一个电机方向反了

4. **测试转向**
   ```c
   Motor_TurnLeft(50);   // 左转
   HAL_Delay(2000);
   Motor_StopAll();
   ```
   - 观察：小车应该**向左转**
   - 如果向右转，说明左右电机定义颠倒了

## 如果发现问题，修复方法：

### 方法1：交换硬件定义（推荐）

在 `motor.h` 中交换 A 和 B 的引脚定义：

```c
// 原来的定义
#define MOTOR_AIN1_PIN GPIO_PIN_5    // 左电机
#define MOTOR_AIN1_PORT GPIOB
#define MOTOR_AIN2_PIN GPIO_PIN_4
#define MOTOR_AIN2_PORT GPIOB

#define MOTOR_BIN1_PIN GPIO_PIN_3    // 右电机
#define MOTOR_BIN1_PORT GPIOB
#define MOTOR_BIN2_PIN GPIO_PIN_15
#define MOTOR_BIN2_PORT GPIOA

// 如果颠倒了，改成：
#define MOTOR_AIN1_PIN GPIO_PIN_3    // 交换
#define MOTOR_AIN1_PORT GPIOB
#define MOTOR_AIN2_PIN GPIO_PIN_15
#define MOTOR_AIN2_PORT GPIOA

#define MOTOR_BIN1_PIN GPIO_PIN_5    // 交换
#define MOTOR_BIN1_PORT GPIOB
#define MOTOR_BIN2_PIN GPIO_PIN_4
#define MOTOR_BIN2_PORT GPIOB
```

### 方法2：交换 PWM 通道

在 `motor.c` 的 `Motor_SetPWM()` 函数中交换通道：

```c
switch (motor) {
case MOTOR_LEFT:
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm);  // 改成 CHANNEL_4
    break;

case MOTOR_RIGHT:
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm);  // 改成 CHANNEL_1
    break;
}
```

### 方法3：如果只是方向反了

如果左右没颠倒，但前进变后退，在 `Motor_SetDirection()` 中交换 SET 和 RESET：

```c
case MOTOR_DIR_FORWARD:
    // 把 GPIO_PIN_SET 和 GPIO_PIN_RESET 互换
    HAL_GPIO_WritePin(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN, GPIO_PIN_SET);
    break;
```

## 编码器方向测试

测试编码器是否与电机方向匹配：

```c
Encoder_ResetAll();
Motor_Forward(50);
HAL_Delay(1000);
Motor_StopAll();

int16_t left_count = Encoder_GetDeltaCount(ENCODER_LEFT);
int16_t right_count = Encoder_GetDeltaCount(ENCODER_RIGHT);

printf("Left: %d, Right: %d\n", left_count, right_count);
```

- 前进时，两个编码器计数应该都是**正数**
- 如果某个是负数，说明该编码器接线反了，或者在 CubeMX 中配置反了

## 串口命令测试

通过串口发送以下命令测试：

- `F 50` - 前进，速度50%
- `B 50` - 后退，速度50%
- `L 50` - 左转
- `R 50` - 右转
- `RL 50` - 原地左旋
- `RR 50` - 原地右旋
- `M 30 50` - 左轮30%，右轮50%
- `STOP` - 停止
