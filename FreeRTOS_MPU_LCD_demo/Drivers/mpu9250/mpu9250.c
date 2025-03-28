/**
 *********************************************************************
 * @file    MPU9250
 * @author
 * @version V1.0
 * @date    2025-03
 * @brief   MPU9250���
 *********************************************************************
 */
#include "mpu9250.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "bsp_led.h"


/**
 * ��    ����MPU9250�ȴ��¼�
 * ��    ����ͬI2C_CheckEvent
 * �� �� ֵ����
 */
void MPU9250_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
  uint32_t Timeout;
  Timeout = 10000;                                   // ������ʱ����ʱ��
  while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) // ѭ���ȴ�ָ���¼�
  {
    Timeout--;        // �ȴ�ʱ������ֵ�Լ�
    if (Timeout == 0) // �Լ���0�󣬵ȴ���ʱ
    {
      printf("time out!\n");
      break; // �����ȴ���������
    }
  }
}

/**
 * ��    ����MPU9250д�Ĵ���
 * ��    ����RegAddress �Ĵ�����ַ����Χ���ο�MPU9250�ֲ�ļĴ�������
 * ��    ����Data Ҫд��Ĵ��������ݣ���Χ��0x00~0xFF
 * �� �� ֵ����
 */
void MPU9250_WriteReg(uint8_t RegAddress, uint8_t Data)
{
  I2C_GenerateSTART(I2C2, ENABLE);                       // Ӳ��I2C������ʼ����
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // �ȴ�EV5

  I2C_Send7bitAddress(I2C2, MPU9250_ADDRESS, I2C_Direction_Transmitter); // Ӳ��I2C���ʹӻ���ַ������Ϊ����
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   // �ȴ�EV6

  I2C_SendData(I2C2, RegAddress);                              // Ӳ��I2C���ͼĴ�����ַ
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // �ȴ�EV8

  I2C_SendData(I2C2, Data);                                   // Ӳ��I2C��������
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // �ȴ�EV8_2

  I2C_GenerateSTOP(I2C2, ENABLE); // Ӳ��I2C������ֹ����
}

/**
 * ��    ����MPU9250���Ĵ���
 * ��    ����RegAddress �Ĵ�����ַ����Χ���ο�MPU9250�ֲ�ļĴ�������
 * �� �� ֵ����ȡ�Ĵ��������ݣ���Χ��0x00~0xFF
 */
uint8_t MPU9250_ReadReg(uint8_t RegAddress)
{
  uint8_t Data;

  I2C_GenerateSTART(I2C2, ENABLE);                       // Ӳ��I2C������ʼ����
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // �ȴ�EV5

  I2C_Send7bitAddress(I2C2, MPU9250_ADDRESS, I2C_Direction_Transmitter); // Ӳ��I2C���ʹӻ���ַ������Ϊ����
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   // �ȴ�EV6

  I2C_SendData(I2C2, RegAddress);                             // Ӳ��I2C���ͼĴ�����ַ
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // �ȴ�EV8_2

  I2C_GenerateSTART(I2C2, ENABLE);                       // Ӳ��I2C�����ظ���ʼ����
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // �ȴ�EV5

  I2C_Send7bitAddress(I2C2, MPU9250_ADDRESS, I2C_Direction_Receiver); // Ӳ��I2C���ʹӻ���ַ������Ϊ����
  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);   // �ȴ�EV6

  I2C_AcknowledgeConfig(I2C2, DISABLE); // �ڽ������һ���ֽ�֮ǰ��ǰ��Ӧ��ʧ��
  I2C_GenerateSTOP(I2C2, ENABLE);       // �ڽ������һ���ֽ�֮ǰ��ǰ����ֹͣ����

  MPU9250_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // �ȴ�EV7
  Data = I2C_ReceiveData(I2C2);                            // �������ݼĴ���

  I2C_AcknowledgeConfig(I2C2, ENABLE); // ��Ӧ��ָ�Ϊʹ�ܣ�Ϊ�˲�Ӱ��������ܲ����Ķ�ȡ���ֽڲ���

  return Data;
}

/**
 * ��    ����MPU9250��ʼ��
 * ��    ������
 * �� �� ֵ����
 */
void MPU9250_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef I2C_InitStructure; // ����ṹ�����
  /*����ʱ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  // ����I2C2��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ����GPIOB��ʱ��

  /*GPIO��ʼ��*/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); // ��PB10��PB11���ų�ʼ��Ϊ���ÿ�©���

  /*I2C��ʼ��*/

  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                // ģʽ��ѡ��ΪI2Cģʽ
  I2C_InitStructure.I2C_ClockSpeed = 50000;                                 // ʱ���ٶȣ�ѡ��Ϊ50KHz
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // ʱ��ռ�ձȣ�ѡ��Tlow/Thigh = 2
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               // Ӧ��ѡ��ʹ��
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // Ӧ���ַ��ѡ��7λ���ӻ�ģʽ�²���Ч
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;                                 // �����ַ���ӻ�ģʽ�²���Ч
  I2C_Init(I2C2, &I2C_InitStructure);                                       // ���ṹ���������I2C_Init������I2C2

  /*I2Cʹ��*/
  I2C_Cmd(I2C2, ENABLE); // ʹ��I2C2����ʼ����

  /*MPU9250�Ĵ�����ʼ������Ҫ����MPU9250�ֲ�ļĴ����������ã��˴��������˲�����Ҫ�ļĴ���*/
  MPU9250_WriteReg(MPU_PWR_MGMT1_REG, 0x01);   // ��Դ����Ĵ���1��ȡ��˯��ģʽ��ѡ��ʱ��ԴΪX��������
  MPU9250_WriteReg(MPU_PWR_MGMT2_REG, 0x00);   // ��Դ����Ĵ���2������Ĭ��ֵ0���������������
  MPU9250_WriteReg(MPU_SAMPLE_RATE_REG, 0x09); // �����ʷ�Ƶ�Ĵ��������ò�����
  MPU9250_WriteReg(MPU_CFG_REG, 0x06);         // ���üĴ���������DLPF
  MPU9250_WriteReg(MPU_GYRO_CFG_REG, 0x18);    // ���������üĴ�����ѡ��������Ϊ��2000��/s
  MPU9250_WriteReg(MPU_ACCEL_CFG_REG, 0x18);   // ���ٶȼ����üĴ�����ѡ��������Ϊ��16g
}

/**
 * ��    ����MPU9250��ȡID��
 * ��    ������
 * �� �� ֵ��MPU9250��ID��
 */
uint8_t MPU9250_GetID(void)
{
  return MPU9250_ReadReg(MPU_DEVICE_ID); // ����WHO_AM_I�Ĵ�����ֵ
}


/// @brief MPU9250��ȡ����
/// @param AccN ָ���ٶȼ�X��Y��Z������� ��Χ��(int16_t)-32768~32767
/// @param GyroN  ָ��������X��Y��Z�������
/// @param Tempp �¶ȼ�
void MPU9250_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ,float *Temp)
{
  uint8_t DataH, DataL; // �������ݸ�8λ�͵�8λ�ı���

  DataH = MPU9250_ReadReg(MPU_ACCEL_XOUTH_REG); // ��ȡ���ٶȼ�X��ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_ACCEL_XOUTL_REG); // ��ȡ���ٶȼ�X��ĵ�8λ����
  *AccX = (DataH << 8) | DataL;                 // ����ƴ�ӣ�ͨ�������������

  DataH = MPU9250_ReadReg(MPU_ACCEL_YOUTH_REG); // ��ȡ���ٶȼ�Y��ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_ACCEL_YOUTL_REG); // ��ȡ���ٶȼ�Y��ĵ�8λ����
  *AccY = (DataH << 8) | DataL;                 // ����ƴ�ӣ�ͨ�������������

  DataH = MPU9250_ReadReg(MPU_ACCEL_ZOUTH_REG); // ��ȡ���ٶȼ�Z��ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_ACCEL_ZOUTL_REG); // ��ȡ���ٶȼ�Z��ĵ�8λ����
  *AccZ = (DataH << 8) | DataL;                 // ����ƴ�ӣ�ͨ�������������

  DataH = MPU9250_ReadReg(MPU_GYRO_XOUTH_REG); // ��ȡ������X��ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_GYRO_XOUTL_REG); // ��ȡ������X��ĵ�8λ����
  *GyroX = (DataH << 8) | DataL;               // ����ƴ�ӣ�ͨ�������������

  DataH = MPU9250_ReadReg(MPU_GYRO_YOUTH_REG); // ��ȡ������Y��ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_GYRO_YOUTL_REG); // ��ȡ������Y��ĵ�8λ����
  *GyroY = (DataH << 8) | DataL;               // ����ƴ�ӣ�ͨ�������������

  DataH = MPU9250_ReadReg(MPU_GYRO_ZOUTH_REG); // ��ȡ������Z��ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_GYRO_ZOUTL_REG); // ��ȡ������Z��ĵ�8λ����
  *GyroZ = (DataH << 8) | DataL;               
  
  DataH = MPU9250_ReadReg(MPU_TEMP_OUTH_REG); // ��ȡ�¶�ֵ�ĸ�8λ����
  DataL = MPU9250_ReadReg(MPU_TEMP_OUTL_REG); // ��ȡ�¶�ֵ�ĵ�8λ����
  *Temp = ((double)((DataH << 8) | DataL)/340+36.53);// ���չ�ʽ����ת�� �����������
  // printf("%d %d %d , %d %d %d\n",*AccX,*AccY,*AccZ,*GyroX,*GyroY,*GyroZ);
}
/* end of line*/
