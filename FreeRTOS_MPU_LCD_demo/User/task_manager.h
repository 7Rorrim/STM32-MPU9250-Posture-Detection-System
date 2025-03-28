#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

/* FreeRTOS ���ͷ�ļ�*/
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

/** ������Ӳ��bspͷ�ļ� 
 * �����豸�ĳ�ʼ�������ᱻmain����
*/
#include "bsp_key.h"
#include "bsp_i2c.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_ili9341_lcd.h"

/* ����豸ͷ�ļ� */
#include "mpu9250.h"


/******************************* �궨�� ************************************/
#define KEY1_EVENT (0x01 << 0) // �����¼������λ0
#define KEY2_EVENT (0x01 << 1) // �����¼������λ1

extern TaskHandle_t AppTaskCreate_Handle;
extern EventGroupHandle_t Event_Handle;
/*
*************************************************************************
*                             ��������
*************************************************************************
*/

void AppTaskCreate(void);//���񴴽�
void vApplicationIdleHook(void);//���й��Ӻ���


#endif /* __TASK_MANAGER_H */
