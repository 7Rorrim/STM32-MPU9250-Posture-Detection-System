/**
 ******************************************************************************
 * @file    task_manager.c
 * @version V1.0
 * @date    2025-03
 * @brief   任务管理
 ******************************************************************************
 */

#include "task_manager.h"

/********************************** 内核对象句柄 *********************************/
// 事件句柄会被it调用
static SemaphoreHandle_t Mutex_Handle = NULL; /* 计数统计量句柄 */

/**************************** 任务句柄 ********************************/

static TaskHandle_t MPU_Task_Handle = NULL;  // MPU_Task任务句柄
static TaskHandle_t LCD_Task_Handle = NULL;  // LCD_Task任务句柄
static TaskHandle_t SEND_Task_Handle = NULL; // SNED_Task任务句柄

static TaskHandle_t Housekeeping_Task_Handle = NULL;  // Housekeeping_Task任务句柄
static TaskHandle_t Event_Handler_Task_Hanlde = NULL; // Event_Handler_Task任务句柄

TaskHandle_t AppTaskCreate_Handle = NULL; // 任务创建句柄
EventGroupHandle_t Event_Handle = NULL;   // 事件句柄

/******************************* 全局变量声明 ************************************/

portSHORT Acel[3] = {0, 0, 0};
portSHORT Gyro[3] = {0, 0, 0};
portFLOAT Temp[1] = {0};
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

/* 任务 */
static void MPU_Task(void *parameter);  // MPU_Task 任务实现
static void LCD_Task(void *parameter);  // LCD_Task 任务实现
static void SEND_Task(void *parameter); // SEND_Task 任务实现
static void Housekeeping_Task(void);    // Housekeeping_Task任务实现
// void AppTaskCreate(void);//任务创建

/* 空闲相关 */
// void vApplicationIdleHook(void);//空闲钩子函数
static void LCD_idle_dispA(void); // LCD空闲显示A
static void LCD_idle_dispB(void); // LCD空闲显示B
static void LCD_idle_dispC(void); // LCD空闲显示C

/* 中断相关 */
static void Event_Handler_Task(void *parameter); // 事件处理函数

/// @brief MPU_Task 任务
/// @param parameter
static void MPU_Task(void *parameter)
{
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */
    while (1)
    {
        xReturn = xSemaphoreTake(Mutex_Handle,   /* 互斥信号量句柄 */
                                 portMAX_DELAY); /* 等待时间 */
        if (xReturn == pdTRUE)
        {
            // printf("---MPU工作中---\n");

            MPU9250_GetData(&Acel[0], &Acel[1], &Acel[2],
                            &Gyro[0], &Gyro[1], &Gyro[2], &Temp[0]);

            // MPU9250ReadAcc(Acel);
            // MPU9250ReadGyro(Gyro);
            // MPU9250_ReturnTemp(&Temp);
            xReturn = xSemaphoreGive(Mutex_Handle); // 给出互斥信号量
        }

        vTaskDelay(500);
    }
}

/// @brief LCD_Task任务
///
/// 等待互斥量Mutex_Handle后 显示一定内容
/// @param parameter
static void LCD_Task(void *parameter)
{
    /*演示显示变量*/
    static uint8_t sum_disptime = 1;
    char dispBuff[100];
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

    while (1) /*任务部分*/
    {

        xReturn = xSemaphoreTake(Mutex_Handle,   /* 互斥信号量句柄 */
                                 portMAX_DELAY); /* 等待时间 */
        if (xReturn == pdTRUE)
        {
            ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // 清屏
            LCD_SetFont(&Font8x16);
            LCD_SetTextColor(GREEN);
            /********显示变量, 0~3行已被占用了*******/
            /*使用c标准库把变量转化成字符串 然后显示该字符串即可*/

            sprintf(dispBuff, "MPU is collecting : %4d times", sum_disptime++);
            ILI9341_DispStringLine_EN(LINE(4), dispBuff);

            sprintf(dispBuff, "Acel: %05d %05d %05d", Acel[0], Acel[1], Acel[2]);
            ILI9341_DispStringLine_EN(LINE(5), dispBuff);
            sprintf(dispBuff, "Gyro: %05d %05d %05d", Gyro[0], Gyro[1], Gyro[2]);
            ILI9341_DispStringLine_EN(LINE(7), dispBuff);
            sprintf(dispBuff, "Temp: %4.2f", Temp[0]);
            ILI9341_DispStringLine_EN(LINE(9), dispBuff);

            vTaskDelay(500); // 显示一段时间

            LCD_ClearLine(LINE(4)); // 清除单行文字
            LCD_ClearLine(LINE(5));
            LCD_ClearLine(LINE(7));
            LCD_ClearLine(LINE(9));

            xReturn = xSemaphoreGive(Mutex_Handle); // 给出互斥信号量
        }

        vTaskDelay(500);
    }
}

/// @brief SEND_Task任务
///
/// 等待到互斥量Mutex_Handle之后 通过重定向输出到串口
/// @param parameter
static void SEND_Task(void *parameter)
{

    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
        xReturn = xSemaphoreTake(Mutex_Handle,   /* 互斥信号量句柄 */
                                 portMAX_DELAY); /* 等待时间 */
        if (xReturn == pdTRUE)
        {
            printf("Acel: %05d %05d %05d", Acel[0], Acel[1], Acel[2]);

            printf("   Gyro: %05d %05d %05d", Gyro[0], Gyro[1], Gyro[2]);

            printf("   Temp: %4.2f\n", Temp[0]);

            xReturn = xSemaphoreGive(Mutex_Handle); // 给出互斥信号量
        }

        vTaskDelay(500);
    }
}

/// @brief 最低优先级的空任务
///
/// 作用是为了防止三个任务都在延时中 意外进入空闲任务
/// @param
static void Housekeeping_Task(void)
{

    while (1)
    {
        taskYIELD();
    }
}

/// @brief 任务创建
///
/// 所有的任务创建函数都放在这个函数里面
/// @param
void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL(); // 进入临界区

    /* 创建 Event_Handle */
    Event_Handle = xEventGroupCreate();
    if (NULL != Event_Handle)
        printf("Event_Handle 事件创建成功!\r\n");

    /* 创建Mutex_Handle */
    Mutex_Handle = xSemaphoreCreateMutex(); /* 最大计数量, 初始量*/
    if (NULL != Mutex_Handle)
        printf("Mutex_Handle互斥信号量创建成功!\r\n");

    xReturn = xTaskCreate((TaskFunction_t)Housekeeping_Task,          // 任务入口函数
                          (const char *)"Housekeeping_Task",          // 任务名字
                          (uint16_t)512,                              // 任务栈大小
                          (void *)NULL,                               // 任务入口函数参数
                          (UBaseType_t)1,                             // 任务的优先级
                          (TaskHandle_t *)&Housekeeping_Task_Handle); // 任务控制块指针
    if (pdPASS == xReturn)
        printf("创建Housekeeping_Task任务成功!\n");

    xReturn = xTaskCreate((TaskFunction_t)MPU_Task,          // 任务入口函数
                          (const char *)"MPU_Task",          // 任务名字
                          (uint16_t)512,                     // 任务栈大小
                          (void *)NULL,                      // 任务入口函数参数
                          (UBaseType_t)3,                    // 任务的优先级
                          (TaskHandle_t *)&MPU_Task_Handle); // 任务控制块指针
    if (pdPASS == xReturn)
        printf("创建MPU_Task任务成功!\n");

    xReturn = xTaskCreate((TaskFunction_t)LCD_Task,          // 任务入口函数
                          (const char *)"LCD_Task",          // 任务名字
                          (uint16_t)512,                     // 任务栈大小
                          (void *)NULL,                      // 任务入口函数参数
                          (UBaseType_t)4,                    // 任务的优先级
                          (TaskHandle_t *)&LCD_Task_Handle); // 任务控制块指针
    if (pdPASS == xReturn)
        printf("创建LCD_Task任务成功!\n");

    xReturn = xTaskCreate((TaskFunction_t)SEND_Task,          // 任务入口函数
                          (const char *)"SEND_Task",          // 任务名字
                          (uint16_t)512,                      // 任务栈大小
                          (void *)NULL,                       // 任务入口函数参数
                          (UBaseType_t)5,                     // 任务的优先级
                          (TaskHandle_t *)&SEND_Task_Handle); // 任务控制块指针
    if (pdPASS == xReturn)
        printf("创建SEND_Task任务成功!\n");

    xReturn = xTaskCreate((TaskFunction_t)Event_Handler_Task,          // 任务入口函数
                          (const char *)"Event_Handler_Task",          // 任务名字
                          (uint16_t)512,                               // 任务栈大小
                          (void *)NULL,                                // 任务入口函数参数
                          (UBaseType_t)6,                              // 任务的优先级
                          (TaskHandle_t *)&Event_Handler_Task_Hanlde); // 任务控制块指针
    if (pdPASS == xReturn)
        printf("创建Event_Handler_Task任务成功!\n");

    vTaskDelete(AppTaskCreate_Handle); // 删除AppTaskCreate任务

    taskEXIT_CRITICAL(); // 退出临界区
}

/******************************* 空闲相关部分 ************************************/

/// @brief 空闲钩子函数
/// 根据系统时间 依次显示LCD
/// @param
void vApplicationIdleHook(void)
{

    static TickType_t xLastCheck = 0;                   // 上次检查的时间
    static uint8_t xFuncIndex = 0;                      // 当前应该调用的函数索引,用于实现多个任务依次执行
    const TickType_t xDelayTicks = pdMS_TO_TICKS(2000); // 延时 2000ms

    /* 任务都是一个无限循环，不能返回 */

    TickType_t xCurrentTime = xTaskGetTickCount(); // 获取当前的系统 Tick 计数

    // 判断是否达到延时条件
    if ((xCurrentTime - xLastCheck) >= xDelayTicks)
    {
        xLastCheck = xCurrentTime; // 更新时间戳为当前时间
        LCD_SetFont(&Font8x16);    // 字体一直都只用8*16 其实没什么用
        // 根据索引调用对应的函数
        switch (xFuncIndex)
        {
        case 0:
            LCD_idle_dispA();
            break;
        case 1:
            LCD_idle_dispB();

            break;
        case 2:
            LCD_idle_dispC();

            break;
        default:
            break;
        }
        xFuncIndex++; // 更新函数索引
        if (xFuncIndex >= 3)
        {
            xFuncIndex = 0; // 如果索引超出范围，重置为 0
        }
    }
}
#define IDLE_FUNC 1
#if IDLE_FUNC

/// @brief LCD空闲显示A
/// @param
static void LCD_idle_dispA(void)
{
    ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // 清屏 也就是置黑色
    LED_RGBOFF;
    LED1_ON;
    /* 画直线 */
    LCD_SetTextColor(BLUE);
    ILI9341_DispStringLine_EN(LINE(4), "Draw line:");

    LCD_SetTextColor(RED);
    ILI9341_DrawLine(50, 170, 210, 230);
    ILI9341_DrawLine(50, 200, 210, 240);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawLine(100, 170, 200, 230);
    ILI9341_DrawLine(200, 200, 220, 240);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawLine(110, 170, 110, 230);
    ILI9341_DrawLine(130, 200, 220, 240);

    // LCD_ClearLine(LINE(4));// 清除单行文字
}
/// @brief LCD空闲显示B
/// @param
static void LCD_idle_dispB(void)
{
    ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // 清屏 也就是置黑色
    LED_RGBOFF;
    LED2_ON;
    /*画矩形*/
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN(LINE(4), "Draw Rect:");

    LCD_SetTextColor(RED);
    ILI9341_DrawRectangle(50, 200, 100, 30, 1);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawRectangle(160, 200, 20, 40, 0);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawRectangle(170, 200, 50, 20, 1);
}

/// @brief LCD空闲显示C
/// @param
static void LCD_idle_dispC(void)
{
    ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // 清屏 也就是置黑色
    LED_RGBOFF;
    LED3_ON;
    /* 画圆 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN(LINE(4), "Draw Cir:");

    LCD_SetTextColor(RED);
    ILI9341_DrawCircle(100, 200, 20, 0);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawCircle(100, 200, 10, 1);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawCircle(140, 200, 20, 0);
}
#endif
/******************************* 中断相关部分 ************************************/

/// @brief Event_Handler_Task 主体
///
/// 等待到事件后用于处理的任务
///
/// 等待到EVENT时发送信息, 有三种情况:
///
/// 1)只有KEY1_EVENT 挂起所有其他任务, 进入空闲 等待下一次KEY1_EVENT发生
///
/// 2)只有KEY2_EVENT 发送信息并且强制延时一段时间
///
/// 3)KEY1 和KEY2 EVENT都有 运行2)且不打断1),运行完成后回到1
/// @param parameter
static void Event_Handler_Task(void *parameter)
{
    EventBits_t r_event;
    static uint8_t xEventIndex = 0; // 用于记录KEY1触发状态

    while (1)
    {
        r_event = xEventGroupWaitBits(Event_Handle,
                                      KEY1_EVENT | KEY2_EVENT, /* 接收线程感兴趣的事件 */
                                      pdFALSE,                 /* 退出时不清除事件位 */
                                      pdFALSE,                 /* 不用等待所有事件 */
                                      portMAX_DELAY);          /* 指定超时时间,一直等 */

        /* KEY2_EVENT优先 可以在KEY1_EVENT相关程序已运行时运行 */
        if ((r_event & KEY2_EVENT) != 0)
        {
            // 情况2：只有KEY2_EVENT
            vTaskSuspendAll();

            printf("KEY2 EVENT interrupt\n");

            // vTaskDelay(pdMS_TO_TICKS(5000)); // 延时一段时间
            uint32_t num;
            for (num = 0; num < 100000; num++)
                ;

            printf("interrupt end\n");                      // 结束 发送提示
            xEventGroupClearBits(Event_Handle, KEY2_EVENT); // 清除事件
            xTaskResumeAll();                               // 恢复调度器
        }
        /* KEY1_EVENT相关 */
        if ((r_event & KEY1_EVENT) != 0)
        {
            if (xEventIndex == 0) // 说明是进入状态
            {
                vTaskSuspend(Housekeeping_Task_Handle);
                vTaskSuspend(MPU_Task_Handle);
                vTaskSuspend(LCD_Task_Handle);
                vTaskSuspend(SEND_Task_Handle);
                
                // 挂起所有任务 执行空闲任务
                printf("device idle , waiting Key1 \n");
                xEventGroupClearBits(Event_Handle, KEY1_EVENT); // 清除事件
                xEventIndex++;
            }
            else // 说明是第二次进入
            {
                vTaskResume(Housekeeping_Task_Handle);
                vTaskResume(MPU_Task_Handle);
                vTaskResume(LCD_Task_Handle);
                vTaskResume(SEND_Task_Handle);
                
                printf("device resume\n");
                xEventGroupClearBits(Event_Handle, KEY1_EVENT); // 清除事件
                xEventIndex = 0;
            }
        }
        portYIELD();
    }
}
/*********************************************END OF FILE**********************/
