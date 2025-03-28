/**
 ******************************************************************************
 * @file    task_manager.c
 * @version V1.0
 * @date    2025-03
 * @brief   �������
 ******************************************************************************
 */

#include "task_manager.h"

/********************************** �ں˶����� *********************************/
// �¼�����ᱻit����
static SemaphoreHandle_t Mutex_Handle = NULL; /* ����ͳ������� */

/**************************** ������ ********************************/

static TaskHandle_t MPU_Task_Handle = NULL;  // MPU_Task������
static TaskHandle_t LCD_Task_Handle = NULL;  // LCD_Task������
static TaskHandle_t SEND_Task_Handle = NULL; // SNED_Task������

static TaskHandle_t Housekeeping_Task_Handle = NULL;  // Housekeeping_Task������
static TaskHandle_t Event_Handler_Task_Hanlde = NULL; // Event_Handler_Task������

TaskHandle_t AppTaskCreate_Handle = NULL; // ���񴴽����
EventGroupHandle_t Event_Handle = NULL;   // �¼����

/******************************* ȫ�ֱ������� ************************************/

portSHORT Acel[3] = {0, 0, 0};
portSHORT Gyro[3] = {0, 0, 0};
portFLOAT Temp[1] = {0};
/*
*************************************************************************
*                             ��������
*************************************************************************
*/

/* ���� */
static void MPU_Task(void *parameter);  // MPU_Task ����ʵ��
static void LCD_Task(void *parameter);  // LCD_Task ����ʵ��
static void SEND_Task(void *parameter); // SEND_Task ����ʵ��
static void Housekeeping_Task(void);    // Housekeeping_Task����ʵ��
// void AppTaskCreate(void);//���񴴽�

/* ������� */
// void vApplicationIdleHook(void);//���й��Ӻ���
static void LCD_idle_dispA(void); // LCD������ʾA
static void LCD_idle_dispB(void); // LCD������ʾB
static void LCD_idle_dispC(void); // LCD������ʾC

/* �ж���� */
static void Event_Handler_Task(void *parameter); // �¼�������

/// @brief MPU_Task ����
/// @param parameter
static void MPU_Task(void *parameter)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    while (1)
    {
        xReturn = xSemaphoreTake(Mutex_Handle,   /* �����ź������ */
                                 portMAX_DELAY); /* �ȴ�ʱ�� */
        if (xReturn == pdTRUE)
        {
            // printf("---MPU������---\n");

            MPU9250_GetData(&Acel[0], &Acel[1], &Acel[2],
                            &Gyro[0], &Gyro[1], &Gyro[2], &Temp[0]);

            // MPU9250ReadAcc(Acel);
            // MPU9250ReadGyro(Gyro);
            // MPU9250_ReturnTemp(&Temp);
            xReturn = xSemaphoreGive(Mutex_Handle); // ���������ź���
        }

        vTaskDelay(500);
    }
}

/// @brief LCD_Task����
///
/// �ȴ�������Mutex_Handle�� ��ʾһ������
/// @param parameter
static void LCD_Task(void *parameter)
{
    /*��ʾ��ʾ����*/
    static uint8_t sum_disptime = 1;
    char dispBuff[100];
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    while (1) /*���񲿷�*/
    {

        xReturn = xSemaphoreTake(Mutex_Handle,   /* �����ź������ */
                                 portMAX_DELAY); /* �ȴ�ʱ�� */
        if (xReturn == pdTRUE)
        {
            ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // ����
            LCD_SetFont(&Font8x16);
            LCD_SetTextColor(GREEN);
            /********��ʾ����, 0~3���ѱ�ռ����*******/
            /*ʹ��c��׼��ѱ���ת�����ַ��� Ȼ����ʾ���ַ�������*/

            sprintf(dispBuff, "MPU is collecting : %4d times", sum_disptime++);
            ILI9341_DispStringLine_EN(LINE(4), dispBuff);

            sprintf(dispBuff, "Acel: %05d %05d %05d", Acel[0], Acel[1], Acel[2]);
            ILI9341_DispStringLine_EN(LINE(5), dispBuff);
            sprintf(dispBuff, "Gyro: %05d %05d %05d", Gyro[0], Gyro[1], Gyro[2]);
            ILI9341_DispStringLine_EN(LINE(7), dispBuff);
            sprintf(dispBuff, "Temp: %4.2f", Temp[0]);
            ILI9341_DispStringLine_EN(LINE(9), dispBuff);

            vTaskDelay(500); // ��ʾһ��ʱ��

            LCD_ClearLine(LINE(4)); // �����������
            LCD_ClearLine(LINE(5));
            LCD_ClearLine(LINE(7));
            LCD_ClearLine(LINE(9));

            xReturn = xSemaphoreGive(Mutex_Handle); // ���������ź���
        }

        vTaskDelay(500);
    }
}

/// @brief SEND_Task����
///
/// �ȴ���������Mutex_Handle֮�� ͨ���ض������������
/// @param parameter
static void SEND_Task(void *parameter)
{

    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
        xReturn = xSemaphoreTake(Mutex_Handle,   /* �����ź������ */
                                 portMAX_DELAY); /* �ȴ�ʱ�� */
        if (xReturn == pdTRUE)
        {
            printf("Acel: %05d %05d %05d", Acel[0], Acel[1], Acel[2]);

            printf("   Gyro: %05d %05d %05d", Gyro[0], Gyro[1], Gyro[2]);

            printf("   Temp: %4.2f\n", Temp[0]);

            xReturn = xSemaphoreGive(Mutex_Handle); // ���������ź���
        }

        vTaskDelay(500);
    }
}

/// @brief ������ȼ��Ŀ�����
///
/// ������Ϊ�˷�ֹ������������ʱ�� ��������������
/// @param
static void Housekeeping_Task(void)
{

    while (1)
    {
        taskYIELD();
    }
}

/// @brief ���񴴽�
///
/// ���е����񴴽����������������������
/// @param
void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    taskENTER_CRITICAL(); // �����ٽ���

    /* ���� Event_Handle */
    Event_Handle = xEventGroupCreate();
    if (NULL != Event_Handle)
        printf("Event_Handle �¼������ɹ�!\r\n");

    /* ����Mutex_Handle */
    Mutex_Handle = xSemaphoreCreateMutex(); /* ��������, ��ʼ��*/
    if (NULL != Mutex_Handle)
        printf("Mutex_Handle�����ź��������ɹ�!\r\n");

    xReturn = xTaskCreate((TaskFunction_t)Housekeeping_Task,          // ������ں���
                          (const char *)"Housekeeping_Task",          // ��������
                          (uint16_t)512,                              // ����ջ��С
                          (void *)NULL,                               // ������ں�������
                          (UBaseType_t)1,                             // ��������ȼ�
                          (TaskHandle_t *)&Housekeeping_Task_Handle); // ������ƿ�ָ��
    if (pdPASS == xReturn)
        printf("����Housekeeping_Task����ɹ�!\n");

    xReturn = xTaskCreate((TaskFunction_t)MPU_Task,          // ������ں���
                          (const char *)"MPU_Task",          // ��������
                          (uint16_t)512,                     // ����ջ��С
                          (void *)NULL,                      // ������ں�������
                          (UBaseType_t)3,                    // ��������ȼ�
                          (TaskHandle_t *)&MPU_Task_Handle); // ������ƿ�ָ��
    if (pdPASS == xReturn)
        printf("����MPU_Task����ɹ�!\n");

    xReturn = xTaskCreate((TaskFunction_t)LCD_Task,          // ������ں���
                          (const char *)"LCD_Task",          // ��������
                          (uint16_t)512,                     // ����ջ��С
                          (void *)NULL,                      // ������ں�������
                          (UBaseType_t)4,                    // ��������ȼ�
                          (TaskHandle_t *)&LCD_Task_Handle); // ������ƿ�ָ��
    if (pdPASS == xReturn)
        printf("����LCD_Task����ɹ�!\n");

    xReturn = xTaskCreate((TaskFunction_t)SEND_Task,          // ������ں���
                          (const char *)"SEND_Task",          // ��������
                          (uint16_t)512,                      // ����ջ��С
                          (void *)NULL,                       // ������ں�������
                          (UBaseType_t)5,                     // ��������ȼ�
                          (TaskHandle_t *)&SEND_Task_Handle); // ������ƿ�ָ��
    if (pdPASS == xReturn)
        printf("����SEND_Task����ɹ�!\n");

    xReturn = xTaskCreate((TaskFunction_t)Event_Handler_Task,          // ������ں���
                          (const char *)"Event_Handler_Task",          // ��������
                          (uint16_t)512,                               // ����ջ��С
                          (void *)NULL,                                // ������ں�������
                          (UBaseType_t)6,                              // ��������ȼ�
                          (TaskHandle_t *)&Event_Handler_Task_Hanlde); // ������ƿ�ָ��
    if (pdPASS == xReturn)
        printf("����Event_Handler_Task����ɹ�!\n");

    vTaskDelete(AppTaskCreate_Handle); // ɾ��AppTaskCreate����

    taskEXIT_CRITICAL(); // �˳��ٽ���
}

/******************************* ������ز��� ************************************/

/// @brief ���й��Ӻ���
/// ����ϵͳʱ�� ������ʾLCD
/// @param
void vApplicationIdleHook(void)
{

    static TickType_t xLastCheck = 0;                   // �ϴμ���ʱ��
    static uint8_t xFuncIndex = 0;                      // ��ǰӦ�õ��õĺ�������,����ʵ�ֶ����������ִ��
    const TickType_t xDelayTicks = pdMS_TO_TICKS(2000); // ��ʱ 2000ms

    /* ������һ������ѭ�������ܷ��� */

    TickType_t xCurrentTime = xTaskGetTickCount(); // ��ȡ��ǰ��ϵͳ Tick ����

    // �ж��Ƿ�ﵽ��ʱ����
    if ((xCurrentTime - xLastCheck) >= xDelayTicks)
    {
        xLastCheck = xCurrentTime; // ����ʱ���Ϊ��ǰʱ��
        LCD_SetFont(&Font8x16);    // ����һֱ��ֻ��8*16 ��ʵûʲô��
        // �����������ö�Ӧ�ĺ���
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
        xFuncIndex++; // ���º�������
        if (xFuncIndex >= 3)
        {
            xFuncIndex = 0; // �������������Χ������Ϊ 0
        }
    }
}
#define IDLE_FUNC 1
#if IDLE_FUNC

/// @brief LCD������ʾA
/// @param
static void LCD_idle_dispA(void)
{
    ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // ���� Ҳ�����ú�ɫ
    LED_RGBOFF;
    LED1_ON;
    /* ��ֱ�� */
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

    // LCD_ClearLine(LINE(4));// �����������
}
/// @brief LCD������ʾB
/// @param
static void LCD_idle_dispB(void)
{
    ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // ���� Ҳ�����ú�ɫ
    LED_RGBOFF;
    LED2_ON;
    /*������*/
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN(LINE(4), "Draw Rect:");

    LCD_SetTextColor(RED);
    ILI9341_DrawRectangle(50, 200, 100, 30, 1);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawRectangle(160, 200, 20, 40, 0);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawRectangle(170, 200, 50, 20, 1);
}

/// @brief LCD������ʾC
/// @param
static void LCD_idle_dispC(void)
{
    ILI9341_Clear(0, 16 * 4, LCD_X_LENGTH, LCD_Y_LENGTH - 16 * 4); // ���� Ҳ�����ú�ɫ
    LED_RGBOFF;
    LED3_ON;
    /* ��Բ */
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
/******************************* �ж���ز��� ************************************/

/// @brief Event_Handler_Task ����
///
/// �ȴ����¼������ڴ��������
///
/// �ȴ���EVENTʱ������Ϣ, ���������:
///
/// 1)ֻ��KEY1_EVENT ����������������, ������� �ȴ���һ��KEY1_EVENT����
///
/// 2)ֻ��KEY2_EVENT ������Ϣ����ǿ����ʱһ��ʱ��
///
/// 3)KEY1 ��KEY2 EVENT���� ����2)�Ҳ����1),������ɺ�ص�1
/// @param parameter
static void Event_Handler_Task(void *parameter)
{
    EventBits_t r_event;
    static uint8_t xEventIndex = 0; // ���ڼ�¼KEY1����״̬

    while (1)
    {
        r_event = xEventGroupWaitBits(Event_Handle,
                                      KEY1_EVENT | KEY2_EVENT, /* �����̸߳���Ȥ���¼� */
                                      pdFALSE,                 /* �˳�ʱ������¼�λ */
                                      pdFALSE,                 /* ���õȴ������¼� */
                                      portMAX_DELAY);          /* ָ����ʱʱ��,һֱ�� */

        /* KEY2_EVENT���� ������KEY1_EVENT��س���������ʱ���� */
        if ((r_event & KEY2_EVENT) != 0)
        {
            // ���2��ֻ��KEY2_EVENT
            vTaskSuspendAll();

            printf("KEY2 EVENT interrupt\n");

            // vTaskDelay(pdMS_TO_TICKS(5000)); // ��ʱһ��ʱ��
            uint32_t num;
            for (num = 0; num < 100000; num++)
                ;

            printf("interrupt end\n");                      // ���� ������ʾ
            xEventGroupClearBits(Event_Handle, KEY2_EVENT); // ����¼�
            xTaskResumeAll();                               // �ָ�������
        }
        /* KEY1_EVENT��� */
        if ((r_event & KEY1_EVENT) != 0)
        {
            if (xEventIndex == 0) // ˵���ǽ���״̬
            {
                vTaskSuspend(Housekeeping_Task_Handle);
                vTaskSuspend(MPU_Task_Handle);
                vTaskSuspend(LCD_Task_Handle);
                vTaskSuspend(SEND_Task_Handle);
                
                // ������������ ִ�п�������
                printf("device idle , waiting Key1 \n");
                xEventGroupClearBits(Event_Handle, KEY1_EVENT); // ����¼�
                xEventIndex++;
            }
            else // ˵���ǵڶ��ν���
            {
                vTaskResume(Housekeeping_Task_Handle);
                vTaskResume(MPU_Task_Handle);
                vTaskResume(LCD_Task_Handle);
                vTaskResume(SEND_Task_Handle);
                
                printf("device resume\n");
                xEventGroupClearBits(Event_Handle, KEY1_EVENT); // ����¼�
                xEventIndex = 0;
            }
        }
        portYIELD();
    }
}
/*********************************************END OF FILE**********************/
