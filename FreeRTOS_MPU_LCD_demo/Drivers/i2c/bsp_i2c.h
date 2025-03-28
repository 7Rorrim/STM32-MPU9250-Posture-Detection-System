#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"


/**************************I2C参数定义，I2C1或I2C2********************************/
#define SENSORS_I2CX                      I2C1


/* I2C时钟和I2C GPIO RCC时钟，事实上I2C1和I2C2都是APB1 和 APB2 */
#define SENSORS_I2C_RCC_CLK               RCC_APB1Periph_I2C1
#define SENSORS_I2C_GPIO_RCC              RCC_APB2Periph_GPIOB
//任意地址 不与stm32 I2C地址不一样
#define SNEOSRS_I2C_OWN_ADDRESS7    0x0A

/* 引脚定义 */
#define SENSORS_I2C_SCL_GPIO_PORT         GPIOB
#define SENSORS_I2C_SCL_GPIO_PIN          GPIO_Pin_6

#define SENSORS_I2C_SDA_GPIO_PORT         GPIOB
#define SENSORS_I2C_SDA_GPIO_PIN          GPIO_Pin_7


/* 等待超时时间 */
#define I2Cx_FLAG_TIMEOUT             ((uint32_t) 1000)

/**************************函数声明********************************/
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
