# STM32 MPU9250 Posture Detection System
 MPU9250, LCD and FreeRTOS projects using STM32F103.
#简述
 一个STM32F103项目 在FreeRTOS中使用MPU9250和LCD
 
# 实现功能
1. 整个项目在STM32F103的FreeRTOS系统下运行
2. I2C通讯从MPU9250传感器中读取相关数据
3. USART重定向, 进行USB转串口通讯
4. FSMC模拟8080通讯, 在LCD中显示数据
5. 通过互斥量来协调三个功能(任务)
6. EXTI用按键触发中断, 设置事件, 实现暂停功能
7. 通过钩子函数实现空闲功能, 显示LCD

# 主要设备
- 野火STM32F103开发板;
- 3.2寸LCD;
- MPU9250;
