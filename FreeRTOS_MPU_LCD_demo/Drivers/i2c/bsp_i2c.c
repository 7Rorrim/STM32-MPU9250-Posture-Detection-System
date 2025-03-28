/**
 ******************************************************************************
 * @file    bsp_i2c.c
 * @author  STMicroelectronics
 * @version V1.0
 * @date    2025-03
 * @brief
 ******************************************************************************
 **/

#include "bsp_i2c.h"
#include "bsp_usart.h"

// static void I2Cx_Error(uint8_t Addr);
static void I2C_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT);

/// @brief I2C初始化
/// @param
void I2cMaster_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    /* 使能I2C时钟 */
    RCC_APB1PeriphClockCmd(SENSORS_I2C_RCC_CLK, ENABLE);

    /* 使能I2C GPIO 时钟 */
    RCC_APB2PeriphClockCmd(SENSORS_I2C_GPIO_RCC, ENABLE);

    /* 配置I2C引脚: SCL */
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* 配置I2C引脚: SDA */
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SDA_GPIO_PIN;
    GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    /* 配置I2C */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = SNEOSRS_I2C_OWN_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    //I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_AcknowledgedAddress = 0x00;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Init(SENSORS_I2CX, &I2C_InitStructure);

    /* 使能I2C */
    I2C_Cmd(SENSORS_I2CX, ENABLE);
}

// /// @brief I2C错误 重新初始化I2C
// /// @param Addr I2C的地址
// static void I2Cx_Error(uint8_t Addr)
// {
//     /* 清除 I2C 寄存器 */
//     I2C_InitTypeDef I2C_InitStructure;
//     I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//     I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//     I2C_InitStructure.I2C_OwnAddress1 = 0x00;
//     I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//     I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//     I2C_InitStructure.I2C_ClockSpeed = 400000;
//     I2C_Init(SENSORS_I2CX, &I2C_InitStructure);

//     /* 重新初始化 I2C */
//     I2cMaster_Init();
// }

/// @brief I2C 等待事件
/// @param I2Cx I2C1或I2C2
/// @param I2C_EVENT 等待的事件
void I2C_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    uint32_t Timeout;
    Timeout = 10000;                                   // 给定超时计数时间
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) // 循环等待指定事件
    {
        Timeout--;        // 等待时，计数值自减
        if (Timeout == 0) // 自减到0后，等待超时
        {
            printf("time out! file:%s line %d\n", __FILE__, __LINE__); // 发生错误信息
            break;                                                    // 跳出等待，不等了
        }
    }
}

/// @brief 写寄存器，这是提供给上层的接口
/// @param slave_addr 从机地址
/// @param reg_addr 寄存器地址
/// @param len 写入的长度
/// @param data_ptr 指针 指向要写入的数据 每个数据范围：0x00~0xFF 也就是uint8_t
void Sensors_I2C_WriteReg(unsigned char slave_addr,
                          unsigned char reg_addr,
                          unsigned short len,
                          unsigned char *data_ptr)
{
    I2C_GenerateSTART(SENSORS_I2CX, ENABLE);                       // 硬件I2C生成起始条件
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(SENSORS_I2CX, slave_addr, I2C_Direction_Transmitter);    // 硬件I2C发送从机地址，方向为发送
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待EV6

    I2C_SendData(SENSORS_I2CX, reg_addr);                                // 硬件I2C发送寄存器地址
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 等待EV8

    while (len)
    {
        I2C_SendData(SENSORS_I2CX, *data_ptr); // 硬件I2C发送数据
        if (len == 1)
        {
            I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2
        }
        else
        {
            I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 等待EV8
            data_ptr++;
        }
        len--;
    }
    I2C_GenerateSTOP(SENSORS_I2CX, ENABLE); // 硬件I2C生成终止条件
}

/// @brief 读寄存器，这是提供给上层的接口
/// @param slave_addr 从机地址
/// @param reg_addr 寄存器地址
/// @param len 要读取的长度
/// @param data_ptr 指向要存储数据的指针
void Sensors_I2C_ReadReg(unsigned char slave_addr,
                         unsigned char reg_addr,
                         unsigned short len,
                         unsigned char *data_ptr)
{

    I2C_GenerateSTART(SENSORS_I2CX, ENABLE);                       // 硬件I2C生成起始条件
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(SENSORS_I2CX, slave_addr, I2C_Direction_Transmitter);    // 硬件I2C发送从机地址，方向为发送
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待EV6

    I2C_SendData(SENSORS_I2CX, reg_addr);                               // 硬件I2C发送寄存器地址
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

    while (len)
    {

        I2C_GenerateSTART(SENSORS_I2CX, ENABLE);                       // 硬件I2C生成重复起始条件
        I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

        I2C_Send7bitAddress(SENSORS_I2CX, slave_addr, I2C_Direction_Receiver);    // 硬件I2C发送从机地址，方向为接收
        I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // 等待EV6
        if (len == 1)
        {
            I2C_AcknowledgeConfig(SENSORS_I2CX, DISABLE); // 在接收最后一个字节之前提前将应答失能
            I2C_GenerateSTOP(SENSORS_I2CX, ENABLE);       // 在接收最后一个字节之前提前申请停止条件
        }
        I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_RECEIVED); // 等待EV7
        *data_ptr = I2C_ReceiveData(SENSORS_I2CX);                       // 接收数据寄存器
        data_ptr++;
        len--;
    }

    I2C_AcknowledgeConfig(SENSORS_I2CX, ENABLE); // 将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
}

/*********************************************END OF FILE**********************/
