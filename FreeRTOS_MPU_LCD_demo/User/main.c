/**
  *********************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2025-03
  * @brief   FreeRTOS V9.0.0 + STM32 事件
  *********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
#include "stm32f10x.h"
#include "task_manager.h"

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

static void Device_Init(void);/* 用于初始化板载及外设相关资源 */


/// @brief main
/// 
/// 第一步：开发板硬件初始化
/// 
/// 第二步：创建APP应用任务
///
/// 第三步：启动FreeRTOS，开始多任务调度
/// @return 正常不应该有返回值
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  /* 开发板硬件初始化 */
  Device_Init();
	printf("FreeRTOS系统正在启动!\n");
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
  {
    printf("任务创建成功!\n");
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  }
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}


/// @brief Device_Init 初始化板载和外接设备
///
/// NVIC LED USART I2C KEY LCD MPU9250
/// @param  
static void Device_Init(void)
{
	//中断优先级分组
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    //配置EXTI
	EXTI_Key_Config();
  
	// LED 初始化 
	LED_GPIO_Config();

	// 串口初始化	
	USART_Config();

  /* LCD初始化	*/
  LCD_Init();

  //MPU9250初始化
	MPU9250_Init();

}

/********************************END OF FILE****************************/
