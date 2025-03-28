/**
 *********************************************************************
 * @file    MPU9250
 * @author
 * @version V1.0
 * @date    2025-03
 * @brief   MPU9250相关
 *********************************************************************
 */
#include "mpu9250.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "bsp_led.h"


/**
 * 函    数：MPU9250等待事件
 * 参    数：同I2C_CheckEvent
 * 返 回 值：无
 */
void MPU9250_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
  uint32_t Timeout;
  Timeout = 10000;                                   // 给定超时计数时间
  while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) // 循环等待指定事件
  {
    Timeout--;        // 等待时，计数值自减
    if (Timeout == 0) // 自减到0后，等待超时
    {
      printf("time out!\n");
      break; // 跳出等待，不等了
    }
  }
}

/**
 * 函    数：MPU9250写寄存器
 * 参    数：RegAddress 寄存器地址，范围：参考MPU9250手册的寄存器描述
 * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
 * 返 回 值：无
 */
void MPU9250_WriteReg(uint8_t RegAddress, uint8_t Data)
{
  I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成起始条件
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

  I2C_Send7bitAddress(I2C2, MPU9250_ADDRESS, I2C_Direction_Transmitter); // 硬件I2C发送从机地址，方向为发送
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   // 等待EV6

  I2C_SendData(I2C2, RegAddress);                              // 硬件I2C发送寄存器地址
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 等待EV8

  I2C_SendData(I2C2, Data);                                   // 硬件I2C发送数据
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

  I2C_GenerateSTOP(I2C2, ENABLE); // 硬件I2C生成终止条件
}

/**
 * 函    数：MPU9250读寄存器
 * 参    数：RegAddress 寄存器地址，范围：参考MPU9250手册的寄存器描述
 * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
 */
uint8_t MPU9250_ReadReg(uint8_t RegAddress)
{
  uint8_t Data;

  I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成起始条件
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

  I2C_Send7bitAddress(I2C2, MPU9250_ADDRESS, I2C_Direction_Transmitter); // 硬件I2C发送从机地址，方向为发送
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   // 等待EV6

  I2C_SendData(I2C2, RegAddress);                             // 硬件I2C发送寄存器地址
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

  I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成重复起始条件
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

  I2C_Send7bitAddress(I2C2, MPU9250_ADDRESS, I2C_Direction_Receiver); // 硬件I2C发送从机地址，方向为接收
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);   // 等待EV6

  I2C_AcknowledgeConfig(I2C2, DISABLE); // 在接收最后一个字节之前提前将应答失能
  I2C_GenerateSTOP(I2C2, ENABLE);       // 在接收最后一个字节之前提前申请停止条件

  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // 等待EV7
  Data = I2C_ReceiveData(I2C2);                            // 接收数据寄存器

  I2C_AcknowledgeConfig(I2C2, ENABLE); // 将应答恢复为使能，为了不影响后续可能产生的读取多字节操作

  return Data;
}

/**
 * 函    数：MPU9250初始化
 * 参    数：无
 * 返 回 值：无
 */
void MPU9250_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef I2C_InitStructure; // 定义结构体变量
  /*开启时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  // 开启I2C2的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟

  /*GPIO初始化*/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); // 将PB10和PB11引脚初始化为复用开漏输出

  /*I2C初始化*/

  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                // 模式，选择为I2C模式
  I2C_InitStructure.I2C_ClockSpeed = 50000;                                 // 时钟速度，选择为50KHz
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // 时钟占空比，选择Tlow/Thigh = 2
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               // 应答，选择使能
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 应答地址，选择7位，从机模式下才有效
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;                                 // 自身地址，从机模式下才有效
  I2C_Init(I2C2, &I2C_InitStructure);                                       // 将结构体变量交给I2C_Init，配置I2C2

  /*I2C使能*/
  I2C_Cmd(I2C2, ENABLE); // 使能I2C2，开始运行

  /*MPU9250寄存器初始化，需要对照MPU9250手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
  MPU9250_WriteReg(MPU_PWR_MGMT1_REG, 0x01);   // 电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
  MPU9250_WriteReg(MPU_PWR_MGMT2_REG, 0x00);   // 电源管理寄存器2，保持默认值0，所有轴均不待机
  MPU9250_WriteReg(MPU_SAMPLE_RATE_REG, 0x09); // 采样率分频寄存器，配置采样率
  MPU9250_WriteReg(MPU_CFG_REG, 0x06);         // 配置寄存器，配置DLPF
  MPU9250_WriteReg(MPU_GYRO_CFG_REG, 0x18);    // 陀螺仪配置寄存器，选择满量程为±2000°/s
  MPU9250_WriteReg(MPU_ACCEL_CFG_REG, 0x18);   // 加速度计配置寄存器，选择满量程为±16g
}

/**
 * 函    数：MPU9250获取ID号
 * 参    数：无
 * 返 回 值：MPU9250的ID号
 */
uint8_t MPU9250_GetID(void)
{
  return MPU9250_ReadReg(MPU_DEVICE_ID); // 返回WHO_AM_I寄存器的值
}


/// @brief MPU9250获取数据
/// @param AccN 指向速度计X、Y、Z轴的数据 范围：(int16_t)-32768~32767
/// @param GyroN  指向陀螺仪X、Y、Z轴的数据
/// @param Tempp 温度计
void MPU9250_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ,float *Temp)
{
  uint8_t DataH, DataL; // 定义数据高8位和低8位的变量

  DataH = MPU9250_ReadReg(MPU_ACCEL_XOUTH_REG); // 读取加速度计X轴的高8位数据
  DataL = MPU9250_ReadReg(MPU_ACCEL_XOUTL_REG); // 读取加速度计X轴的低8位数据
  *AccX = (DataH << 8) | DataL;                 // 数据拼接，通过输出参数返回

  DataH = MPU9250_ReadReg(MPU_ACCEL_YOUTH_REG); // 读取加速度计Y轴的高8位数据
  DataL = MPU9250_ReadReg(MPU_ACCEL_YOUTL_REG); // 读取加速度计Y轴的低8位数据
  *AccY = (DataH << 8) | DataL;                 // 数据拼接，通过输出参数返回

  DataH = MPU9250_ReadReg(MPU_ACCEL_ZOUTH_REG); // 读取加速度计Z轴的高8位数据
  DataL = MPU9250_ReadReg(MPU_ACCEL_ZOUTL_REG); // 读取加速度计Z轴的低8位数据
  *AccZ = (DataH << 8) | DataL;                 // 数据拼接，通过输出参数返回

  DataH = MPU9250_ReadReg(MPU_GYRO_XOUTH_REG); // 读取陀螺仪X轴的高8位数据
  DataL = MPU9250_ReadReg(MPU_GYRO_XOUTL_REG); // 读取陀螺仪X轴的低8位数据
  *GyroX = (DataH << 8) | DataL;               // 数据拼接，通过输出参数返回

  DataH = MPU9250_ReadReg(MPU_GYRO_YOUTH_REG); // 读取陀螺仪Y轴的高8位数据
  DataL = MPU9250_ReadReg(MPU_GYRO_YOUTL_REG); // 读取陀螺仪Y轴的低8位数据
  *GyroY = (DataH << 8) | DataL;               // 数据拼接，通过输出参数返回

  DataH = MPU9250_ReadReg(MPU_GYRO_ZOUTH_REG); // 读取陀螺仪Z轴的高8位数据
  DataL = MPU9250_ReadReg(MPU_GYRO_ZOUTL_REG); // 读取陀螺仪Z轴的低8位数据
  *GyroZ = (DataH << 8) | DataL;               
  
  DataH = MPU9250_ReadReg(MPU_TEMP_OUTH_REG); // 读取温度值的高8位数据
  DataL = MPU9250_ReadReg(MPU_TEMP_OUTL_REG); // 读取温度值的低8位数据
  *Temp = ((double)((DataH << 8) | DataL)/340+36.53);// 按照公式进行转换 但还是有误差
  // printf("%d %d %d , %d %d %d\n",*AccX,*AccY,*AccZ,*GyroX,*GyroY,*GyroZ);
}
/* end of line*/
