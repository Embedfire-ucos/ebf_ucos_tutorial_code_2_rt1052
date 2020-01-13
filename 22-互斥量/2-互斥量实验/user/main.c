/*
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ��ֵ�ź���-��־�¼�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/
/*
*********************************************************************************************************
*                                            �������ļ�
*********************************************************************************************************
*/
//ucosiiiϵͳ���
#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>
#include  <bsp_int.h>
#include  "os_app_hooks.h"
#include  "app_cfg.h"
//�弶����
#include "bsp.h" 
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

OS_MUTEX TestMutex;     

/*
*********************************************************************************************************
*                                         ������ƿ�TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   AppTaskLed1TCB;
static  OS_TCB   AppTaskLed2TCB;
static  OS_TCB   AppTaskLed3TCB;
/*
*********************************************************************************************************
*                                            �����ջ
*********************************************************************************************************
*/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTaskLed2Stk [ APP_TASK_LED2_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];
/*
*********************************************************************************************************
*                                            ����ԭ��
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskLed1  ( void * p_arg );
static  void  AppTaskLed2  ( void * p_arg );
static  void  AppTaskLed3  ( void * p_arg );

/*
*********************************************************************************************************
* ������ : main
* ����   : ��׼��C�������
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/
int  main (void)
{
    OS_ERR  err;


    BSP_ClkInit();                       /* ��ʼ��ϵͳʱ��       */
    BSP_IntInit();                       /* ��ʼ��RAM�ж�������. */
    BSP_OS_TickInit();                   /* ��ʼ���ں˼�ʱ��     */
															
    Mem_Init();                          /* ��ʼ���ڴ����ģ��   */
    CPU_IntDis();                        /* ���������ж�         */
    CPU_Init();                          /* ��ʼ��uC/CPU���     */

    OSInit(&err);                        /*��ʼ��uC / OS-III     */
    if (err != OS_ERR_NONE) {
        while (1);
    }

    App_OS_SetAllHooks();                                       						//��������Ӧ�ó����Ӻ���

	/* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) AppTaskStart,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);     
    if (err != OS_ERR_NONE) {
        while (1);
    }

    OSStart(&err);                                          								//�����������������uC/OS-III���ƣ�

    while (DEF_ON) {                                           
        ;
    }
}


/*
*********************************************************************************************************
* ������ ��AppTaskStart
* ����   : ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(�� BSP_Init ��ʵ��)��
* �β�   : p_arg   ��OSTaskCreate()�ڴ���������ʱ���ݹ������βΡ�
* ����ֵ : ��
* ע��   : 1) ��һ�д��� (void)p_arg; ��Ϊ�˷�ֹ������������Ϊ�β�p_arg��û���õ�
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;
	
   (void)p_arg;

    OS_TRACE_INIT();                               //��ʼ��uC / OS-III���ټ�¼��

    BSP_OS_TickEnable();                           //���õδ��ʱ�����ж�
    BSP_Init();                                    //�弶��ʼ��

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                  //�����������м��ɼ���CPU����
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

		/* ���������ź��� mutex */
    OSMutexCreate ((OS_MUTEX  *)&TestMutex,           //ָ���ź���������ָ��
                   (CPU_CHAR  *)"Mutex For Test", //�ź���������
                   (OS_ERR    *)&err);            //��������

    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task Led1",
                 (OS_TASK_PTR ) AppTaskLed1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,
                 (CPU_STK    *)&AppTaskLed1Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                /* Create the Led2 task                                */
                 (CPU_CHAR   *)"App Task Led2",
                 (OS_TASK_PTR ) AppTaskLed2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED2_PRIO,
                 (CPU_STK    *)&AppTaskLed2Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Led3",
                 (OS_TASK_PTR ) AppTaskLed3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED3_PRIO,
                 (CPU_STK    *)&AppTaskLed3Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
		
		
		OSTaskDel ( & AppTaskStartTCB, & err );
		
					
}

/*
*********************************************************************************************************
*                                          LED1 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR      err;
    static uint32_t i;
  
   (void)p_arg;
  

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			
      PRINTF("AppTaskLed1 ��ȡ������\n");
      //��ȡ ������ ,û��ȡ����һֱ�ȴ�
		OSMutexPend ((OS_MUTEX  *)&TestMutex,                  //���뻥���ź��� mutex
								 (OS_TICK    )0,                       //�����޵ȴ�
								 (OS_OPT     )OS_OPT_PEND_BLOCKING,    //����������뵽�ź����Ͷ�������
								 (CPU_TS    *)0,                       //������ʱ���
								 (OS_ERR    *)&err);                   //���ش�������		
		
      
      for(i=0;i<1500000;i++)      //ģ������ȼ�����ռ�û�����
      {
//        ;
        OSSched();//�����������
      }

      PRINTF("AppTaskLed1 �ͷŻ�����\n");
		OSMutexPost ((OS_MUTEX  *)&TestMutex,                  //�ͷŻ����ź��� mutex
								 (OS_OPT     )OS_OPT_POST_NONE,        //�����������
								 (OS_ERR    *)&err);                   //���ش�������		
		
      
      
      LED1_TOGGLE;
			OSTimeDlyHMSM (0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed2 ( void * p_arg )
{
    OS_ERR      err;

  
   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			PRINTF("AppTaskLed2 Running\n");
      

			OSTimeDlyHMSM (0,0,0,200,OS_OPT_TIME_PERIODIC,&err);
    }
		
		
}


/*
*********************************************************************************************************
*                                          LED3 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed3 ( void * p_arg )
{
    OS_ERR      err;

   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			
      PRINTF("AppTaskLed3 ��ȡ������\n");	
      //��ȡ ������ ,û��ȡ����һֱ�ȴ�
		OSMutexPend ((OS_MUTEX  *)&TestMutex,                  //���뻥���ź��� mutex
								 (OS_TICK    )0,                       //�����޵ȴ�
								 (OS_OPT     )OS_OPT_PEND_BLOCKING,    //����������뵽�ź����Ͷ�������
								 (CPU_TS    *)0,                       //������ʱ���
								 (OS_ERR    *)&err);                   //���ش�������		
		
      LED2_TOGGLE;
      
      PRINTF("AppTaskLed3 �ͷŻ�����\n");
      OSMutexPost ((OS_MUTEX  *)&TestMutex,                  //�ͷŻ����ź��� mutex
								  (OS_OPT     )OS_OPT_POST_NONE,        //�����������
								  (OS_ERR    *)&err);                   //���ش�������		
		
      
			OSTimeDlyHMSM (0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
      
    }
		
		
}



