/**
  *********************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2025-03
  * @brief   FreeRTOS V9.0.0 + STM32 �¼�
  *********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
#include "stm32f10x.h"
#include "task_manager.h"

/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void Device_Init(void);/* ���ڳ�ʼ�����ؼ����������Դ */


/// @brief main
/// 
/// ��һ����������Ӳ����ʼ��
/// 
/// �ڶ���������APPӦ������
///
/// ������������FreeRTOS����ʼ���������
/// @return ������Ӧ���з���ֵ
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  /* ������Ӳ����ʼ�� */
  Device_Init();
	printf("FreeRTOSϵͳ��������!\n");
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
  {
    printf("���񴴽��ɹ�!\n");
    vTaskStartScheduler();   /* �������񣬿������� */
  }
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/// @brief Device_Init ��ʼ�����غ�����豸
///
/// NVIC LED USART I2C KEY LCD MPU9250
/// @param  
static void Device_Init(void)
{
	//�ж����ȼ�����
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    //����EXTI
	EXTI_Key_Config();
  
	// LED ��ʼ�� 
	LED_GPIO_Config();

	// ���ڳ�ʼ��	
	USART_Config();

  /* LCD��ʼ��	*/
  LCD_Init();

  //MPU9250��ʼ��
	MPU9250_Init();

}

/********************************END OF FILE****************************/
