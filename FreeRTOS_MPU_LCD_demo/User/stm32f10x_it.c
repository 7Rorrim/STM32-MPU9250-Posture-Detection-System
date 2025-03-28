/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "task_manager.h"

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick�жϷ�����
void SysTick_Handler(void)
{	
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */  
    
    xPortSysTickHandler();
    
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
}




/// @brief KEY1_IRQHandler �жϷ�����
/// @param  
void KEY1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  //ȷ���Ƿ������EXTI Line�ж�
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
    
    ulReturn = xEventGroupSetBitsFromISR(Event_Handle,
                                            KEY1_EVENT,
                                            &pxHigherPriorityTaskWoken);
		
	
		//����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);   
    //�����Ҫ�Ļ�����һ�������л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);  
	}  
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}


/// @brief KEY2_IRQHandler �жϷ�����
/// @param  
void KEY2_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
    ulReturn = xEventGroupSetBitsFromISR(Event_Handle,
                                            KEY2_EVENT,
                                            &pxHigherPriorityTaskWoken);
		
		//�����Ҫ�Ļ�����һ�������л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
		//����жϱ�־λ
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

