/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "task_manager.h"

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick中断服务函数
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




/// @brief KEY1_IRQHandler 中断服务函数
/// @param  
void KEY1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  //确保是否产生了EXTI Line中断
  uint32_t ulReturn;
  /* 进入临界段，临界段可以嵌套 */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
    
    ulReturn = xEventGroupSetBitsFromISR(Event_Handle,
                                            KEY1_EVENT,
                                            &pxHigherPriorityTaskWoken);
		
	
		//清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);   
    //如果需要的话进行一次任务切换
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);  
	}  
  
  /* 退出临界段 */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}


/// @brief KEY2_IRQHandler 中断服务函数
/// @param  
void KEY2_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  uint32_t ulReturn;
  /* 进入临界段，临界段可以嵌套 */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
    ulReturn = xEventGroupSetBitsFromISR(Event_Handle,
                                            KEY2_EVENT,
                                            &pxHigherPriorityTaskWoken);
		
		//如果需要的话进行一次任务切换
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
		//清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
  
  /* 退出临界段 */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

