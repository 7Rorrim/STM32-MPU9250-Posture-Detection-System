#ifndef __MPU9250_H
#define __MPU9250_H
#include "stm32f10x.h"

#define ALL_MPU_DEFINE 0
#if ALL_MPU_DEFINE
// ģ���A0���Ž�GND��IIC��7λ��ַΪ0x68�����ӵ�VCC����Ҫ��Ϊ0x69
#define MPU9250_ADDRESS (0x68 << 1) // MPU9250��������ַ

// void MPU9250_task(void *pvParameters);

#define MPU9250_ADDR 0X68 // MPU9250 device IIC address //MPU9250������IIC��ַ
#define MPU6500_ID 0X71   // MPU6500 device ID //MPU6500������ID

// The MPU9250 contains an AK8963 magnetometer with the following address and ID:
// MPU9250�ڲ���װ��һ��AK8963������,��ַ��ID����:
#define AK8963_ADDR 0X0C // I2C address of AK8963 //AK8963��I2C��ַ
#define AK8963_ID 0X48   // The device ID of AK8963 //AK8963������ID

// The internal register of AK8963
// AK8963���ڲ��Ĵ���
#define MAG_WIA 0x00 // The device ID register address of AK8963 //AK8963������ID�Ĵ�����ַ
#define MAG_CNTL1 0X0A
#define MAG_CNTL2 0X0B

#define MAG_XOUT_L 0X03
#define MAG_XOUT_H 0X04
#define MAG_YOUT_L 0X05
#define MAG_YOUT_H 0X06
#define MAG_ZOUT_L 0X07
#define MAG_ZOUT_H 0X08

// The internal register of MPU6500
// MPU6500���ڲ��Ĵ���
#define MPU_SELF_TESTX_REG 0X0D   // Check register X //�Լ�Ĵ���X
#define MPU_SELF_TESTY_REG 0X0E   // Check register Y //�Լ�Ĵ���Y
#define MPU_SELF_TESTZ_REG 0X0F   // Check register Z //�Լ�Ĵ���Z
#define MPU_SELF_TESTA_REG 0X10   // Check register A //�Լ�Ĵ���A
#define MPU_SAMPLE_RATE_REG 0X19  // Sampling frequency divider //����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG 0X1A          // Configure the register //���üĴ���
#define MPU_GYRO_CFG_REG 0X1B     // The gyroscope configuration register //���������üĴ���
#define MPU_ACCEL_CFG_REG 0X1C    // Accelerometer configuration register //���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG 0X1F   // Motion detection threshold setting register //�˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG 0X23      // FIFO Enable register //FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG 0X24  // IIC host control register //IIC�������ƼĴ���
#define MPU_I2CSLV0_ADDR_REG 0X25 // IIC slave 0 device address register //IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG 0X26      // IIC slave 0 data address register //IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG 0X27 // IIC slave 0 control register //IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG 0X28 // IIC slave 1 device address register //IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG 0X29      // IIC slave 1 data address register //IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG 0X2A // IIC slave 1 control register //IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG 0X2B // IIC slave 2 device address register //IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG 0X2C      // IIC slave 2 data address register //IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG 0X2D // IIC slave 2 control register //IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG 0X2E // IIC slave 3 device address register //IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG 0X2F      // IIC slave 3 data address register //IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG 0X30 // IIC slave 3 control register //IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG 0X31 // IIC slave 4 device address register //IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG 0X32      // IIC slave 4 data address register//IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG 0X33   // IIC slave 4 writes data register //IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG 0X34 // IIC slave 4 control register //IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG 0X35   // IIC slave 4 reads the data register //IIC�ӻ�4�����ݼĴ���

#define MPU_I2CMST_STA_REG 0X36 // IIC host status register //IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG 0X37  // Interrupt/bypass set register //�ж�/��·���üĴ���
#define MPU_INT_EN_REG 0X38     // Interrupt enable register//�ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG 0X3A    // Interrupt status register//�ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG 0X3B // Acceleration value, X-axis high 8-bit register //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG 0X3C // Acceleration value, X-axis low 8-bit register //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG 0X3D // Acceleration value,Y axis high 8 bit registe //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG 0X3E // Acceleration value,Y axis low 8-bit register //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG 0X3F // Acceleration value, Z-axis high 8-bit register //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG 0X40 // Acceleration value, Z-axis low 8-bit register //���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG 0X41 // High temperature 8-bit register //�¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG 0X42 // Low temperature 8 bit register //�¶�ֵ��8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG 0X43 // Gyroscope value, X-axis high 8-bit register//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG 0X44 // Gyroscope value, X-axis low 8-bit register //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG 0X45 // Gyroscope value, Y axis high 8 bit register//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG 0X46 // Gyroscope value, Y axis low 8-bit register /������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG 0X47 // Gyroscope value, Z-axis high 8-bit register//������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG 0X48 // Gyroscope value, Z-axis low 8-bit register//������ֵ,Z���8λ�Ĵ���

#define MPU_I2CSLV0_DO_REG 0X63 // IIC slave 0 data register //IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG 0X64 // IIC slave 1 data register //IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG 0X65 // IIC slave 2 data register //IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG 0X66 // IIC slave 3 data register //IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG 0X67 // IIC host delay management register //IIC������ʱ�����Ĵ���
#define MPU_SIGPATH_RST_REG 0X68  // Signal channel reset register//�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG 0X69 // Motion detection control register //�˶������ƼĴ���
#define MPU_USER_CTRL_REG 0X6A    // User control register//�û����ƼĴ���
#define MPU_PWR_MGMT1_REG 0X6B    // Power Management Register 1 //��Դ�����Ĵ���1
#define MPU_PWR_MGMT2_REG 0X6C    // Power Management Register 2 //��Դ�����Ĵ���2
#define MPU_FIFO_CNTH_REG 0X72    // The FIFO count register is eight bits high //FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG 0X73    // The FIFO count register is eight bits lower //FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG 0X74      // The FIFO read/write register //FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG 0X75    // Device ID register //����ID�Ĵ���

#define MPU9250_TASK_PRIO 3
#define MPU9250_STK_SIZE 256
#endif

/**************************MPU�õ��ĺ�********************************/
#define USER_DEFINED 1
#if USER_DEFINED

/* ��ʼ��Ҫ�õ��ĺ� */
#define MPU_ADDRESS (0x68 << 1)  // MPU9250��������ַ 8λ
#define MPU_PWR_MGMT1_REG 0X6B   // Power Management Register 1 //��Դ�����Ĵ���1
#define MPU_PWR_MGMT2_REG 0X6C   // Power Management Register 2 //��Դ�����Ĵ���2
#define MPU_SAMPLE_RATE_REG 0X19 // Sampling frequency divider //����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG 0X1A         // Configure the register //���üĴ���
#define MPU_GYRO_CFG_REG 0X1B    // The gyroscope configuration register //���������üĴ���
#define MPU_ACCEL_CFG_REG 0X1C   // Accelerometer configuration register //���ٶȼ����üĴ���
#define MPU_DEVICE_ID 0X75       // ����ID�Ĵ���

/* ��ȡҪ�õ��ĺ� */
#define MPU_GYRO_XOUTH_REG 0X43 //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG 0X44 //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG 0X45 //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG 0X46 //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG 0X47 //������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG 0X48 //������ֵ,Z���8λ�Ĵ���

#define MPU_ACCEL_XOUTH_REG 0X3B //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG 0X3C //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG 0X3D //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG 0X3E //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG 0X3F //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG 0X40 //���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG 0X41 //�¶�ֵ��8λ�Ĵ���
#define MPU_TEMP_OUTL_REG 0X42 //�¶�ֵ��8λ�Ĵ���

#define MPU9250_ADDRESS		0xD0		//MPU9250��I2C�ӻ���ַ

#endif


void MPU9250_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT);
void MPU9250_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU9250_ReadReg(uint8_t RegAddress);
void MPU9250_Init(void);
uint8_t MPU9250_GetID(void);
void MPU9250_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ,float *Temp);

#endif /*__MPU9250 */
