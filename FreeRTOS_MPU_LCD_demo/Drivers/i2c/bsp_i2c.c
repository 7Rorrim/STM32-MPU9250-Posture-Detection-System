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

/// @brief I2C��ʼ��
/// @param
void I2cMaster_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    /* ʹ��I2Cʱ�� */
    RCC_APB1PeriphClockCmd(SENSORS_I2C_RCC_CLK, ENABLE);

    /* ʹ��I2C GPIO ʱ�� */
    RCC_APB2PeriphClockCmd(SENSORS_I2C_GPIO_RCC, ENABLE);

    /* ����I2C����: SCL */
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* ����I2C����: SDA */
    GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SDA_GPIO_PIN;
    GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    /* ����I2C */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = SNEOSRS_I2C_OWN_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    //I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_AcknowledgedAddress = 0x00;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Init(SENSORS_I2CX, &I2C_InitStructure);

    /* ʹ��I2C */
    I2C_Cmd(SENSORS_I2CX, ENABLE);
}

// /// @brief I2C���� ���³�ʼ��I2C
// /// @param Addr I2C�ĵ�ַ
// static void I2Cx_Error(uint8_t Addr)
// {
//     /* ��� I2C �Ĵ��� */
//     I2C_InitTypeDef I2C_InitStructure;
//     I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//     I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//     I2C_InitStructure.I2C_OwnAddress1 = 0x00;
//     I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//     I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//     I2C_InitStructure.I2C_ClockSpeed = 400000;
//     I2C_Init(SENSORS_I2CX, &I2C_InitStructure);

//     /* ���³�ʼ�� I2C */
//     I2cMaster_Init();
// }

/// @brief I2C �ȴ��¼�
/// @param I2Cx I2C1��I2C2
/// @param I2C_EVENT �ȴ����¼�
void I2C_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    uint32_t Timeout;
    Timeout = 10000;                                   // ������ʱ����ʱ��
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) // ѭ���ȴ�ָ���¼�
    {
        Timeout--;        // �ȴ�ʱ������ֵ�Լ�
        if (Timeout == 0) // �Լ���0�󣬵ȴ���ʱ
        {
            printf("time out! file:%s line %d\n", __FILE__, __LINE__); // ����������Ϣ
            break;                                                    // �����ȴ���������
        }
    }
}

/// @brief д�Ĵ����������ṩ���ϲ�Ľӿ�
/// @param slave_addr �ӻ���ַ
/// @param reg_addr �Ĵ�����ַ
/// @param len д��ĳ���
/// @param data_ptr ָ�� ָ��Ҫд������� ÿ�����ݷ�Χ��0x00~0xFF Ҳ����uint8_t
void Sensors_I2C_WriteReg(unsigned char slave_addr,
                          unsigned char reg_addr,
                          unsigned short len,
                          unsigned char *data_ptr)
{
    I2C_GenerateSTART(SENSORS_I2CX, ENABLE);                       // Ӳ��I2C������ʼ����
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_MODE_SELECT); // �ȴ�EV5

    I2C_Send7bitAddress(SENSORS_I2CX, slave_addr, I2C_Direction_Transmitter);    // Ӳ��I2C���ʹӻ���ַ������Ϊ����
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // �ȴ�EV6

    I2C_SendData(SENSORS_I2CX, reg_addr);                                // Ӳ��I2C���ͼĴ�����ַ
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // �ȴ�EV8

    while (len)
    {
        I2C_SendData(SENSORS_I2CX, *data_ptr); // Ӳ��I2C��������
        if (len == 1)
        {
            I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // �ȴ�EV8_2
        }
        else
        {
            I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // �ȴ�EV8
            data_ptr++;
        }
        len--;
    }
    I2C_GenerateSTOP(SENSORS_I2CX, ENABLE); // Ӳ��I2C������ֹ����
}

/// @brief ���Ĵ����������ṩ���ϲ�Ľӿ�
/// @param slave_addr �ӻ���ַ
/// @param reg_addr �Ĵ�����ַ
/// @param len Ҫ��ȡ�ĳ���
/// @param data_ptr ָ��Ҫ�洢���ݵ�ָ��
void Sensors_I2C_ReadReg(unsigned char slave_addr,
                         unsigned char reg_addr,
                         unsigned short len,
                         unsigned char *data_ptr)
{

    I2C_GenerateSTART(SENSORS_I2CX, ENABLE);                       // Ӳ��I2C������ʼ����
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_MODE_SELECT); // �ȴ�EV5

    I2C_Send7bitAddress(SENSORS_I2CX, slave_addr, I2C_Direction_Transmitter);    // Ӳ��I2C���ʹӻ���ַ������Ϊ����
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // �ȴ�EV6

    I2C_SendData(SENSORS_I2CX, reg_addr);                               // Ӳ��I2C���ͼĴ�����ַ
    I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // �ȴ�EV8_2

    while (len)
    {

        I2C_GenerateSTART(SENSORS_I2CX, ENABLE);                       // Ӳ��I2C�����ظ���ʼ����
        I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_MODE_SELECT); // �ȴ�EV5

        I2C_Send7bitAddress(SENSORS_I2CX, slave_addr, I2C_Direction_Receiver);    // Ӳ��I2C���ʹӻ���ַ������Ϊ����
        I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // �ȴ�EV6
        if (len == 1)
        {
            I2C_AcknowledgeConfig(SENSORS_I2CX, DISABLE); // �ڽ������һ���ֽ�֮ǰ��ǰ��Ӧ��ʧ��
            I2C_GenerateSTOP(SENSORS_I2CX, ENABLE);       // �ڽ������һ���ֽ�֮ǰ��ǰ����ֹͣ����
        }
        I2C_WaitEvent(SENSORS_I2CX, I2C_EVENT_MASTER_BYTE_RECEIVED); // �ȴ�EV7
        *data_ptr = I2C_ReceiveData(SENSORS_I2CX);                       // �������ݼĴ���
        data_ptr++;
        len--;
    }

    I2C_AcknowledgeConfig(SENSORS_I2CX, ENABLE); // ��Ӧ��ָ�Ϊʹ�ܣ�Ϊ�˲�Ӱ��������ܲ����Ķ�ȡ���ֽڲ���
}

/*********************************************END OF FILE**********************/
