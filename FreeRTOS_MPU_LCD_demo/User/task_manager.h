#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

/* FreeRTOS 相关头文件*/
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

/** 开发板硬件bsp头文件 
 * 以下设备的初始化函数会被main调用
*/
#include "bsp_key.h"
#include "bsp_i2c.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_ili9341_lcd.h"

/* 外接设备头文件 */
#include "mpu9250.h"


/******************************* 宏定义 ************************************/
#define KEY1_EVENT (0x01 << 0) // 设置事件掩码的位0
#define KEY2_EVENT (0x01 << 1) // 设置事件掩码的位1

extern TaskHandle_t AppTaskCreate_Handle;
extern EventGroupHandle_t Event_Handle;
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

void AppTaskCreate(void);//任务创建
void vApplicationIdleHook(void);//空闲钩子函数


#endif /* __TASK_MANAGER_H */
