#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"


/**************************I2C�������壬I2C1��I2C2********************************/
#define SENSORS_I2CX                      I2C1


/* I2Cʱ�Ӻ�I2C GPIO RCCʱ�ӣ���ʵ��I2C1��I2C2����APB1 �� APB2 */
#define SENSORS_I2C_RCC_CLK               RCC_APB1Periph_I2C1
#define SENSORS_I2C_GPIO_RCC              RCC_APB2Periph_GPIOB
//�����ַ ����stm32 I2C��ַ��һ��
#define SNEOSRS_I2C_OWN_ADDRESS7    0x0A

/* ���Ŷ��� */
#define SENSORS_I2C_SCL_GPIO_PORT         GPIOB
#define SENSORS_I2C_SCL_GPIO_PIN          GPIO_Pin_6

#define SENSORS_I2C_SDA_GPIO_PORT         GPIOB
#define SENSORS_I2C_SDA_GPIO_PIN          GPIO_Pin_7


/* �ȴ���ʱʱ�� */
#define I2Cx_FLAG_TIMEOUT             ((uint32_t) 1000)

/**************************��������********************************/
void I2cMaster_Init(void);
void Sensors_I2C_WriteReg(unsigned char slave_addr,
                              unsigned char reg_addr,
                              unsigned short len, 
                              unsigned char *data_ptr);
void Sensors_I2C_ReadReg(unsigned char slave_addr,
                             unsigned char reg_addr,
                             unsigned short len, 
                             unsigned char *data_ptr);

#endif /* __I2C_H */
