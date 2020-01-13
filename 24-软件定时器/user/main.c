/*
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   �����ʱ��
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

CPU_TS             ts_start;       //ʱ�������
CPU_TS             ts_end; 


/*
*********************************************************************************************************
*                                         ������ƿ�TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;                                //������ƿ�

static  OS_TCB   AppTaskTmrTCB;


/*
*********************************************************************************************************
*                                            �����ջ
*********************************************************************************************************
*/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //�����ջ

static  CPU_STK  AppTaskTmrStk [ APP_TASK_TMR_STK_SIZE ];
/*
*********************************************************************************************************
*                                            ����ԭ��
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);                       //����������

static  void  AppTaskTmr  ( void * p_arg );

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
		/* ���� AppTaskTmr ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskTmrTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Tmr",                             //��������
                 (OS_TASK_PTR ) AppTaskTmr,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_TMR_PRIO,                         //��������ȼ�
                 (CPU_STK    *)&AppTaskTmrStk[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
								 
		OSTaskDel ( & AppTaskStartTCB, & err );                     //ɾ����ʼ������������������
		
								
}

/*
*********************************************************************************************************
*                                          TMR TASK
*********************************************************************************************************
*/
void TmrCallback (OS_TMR *p_tmr, void *p_arg) //�����ʱ��MyTmr�Ļص�����
{
	CPU_INT32U       cpu_clk_freq;	

  PRINTF ( "%s", ( char * ) p_arg );
	
	cpu_clk_freq = BSP_ClkFreqGet(kCLOCK_SysPllClk);                   //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���

	LED1_TOGGLE; 
	
  ts_end = OS_TS_GET() - ts_start;     //��ȡ��ʱ���ʱ�������CPUʱ�ӽ��м�����һ������ֵ��
	                                     //�������㶨ʱʱ�䡣

	PRINTF ( "\r\n��ʱ1s��ͨ��ʱ�����ö�ʱ %07d us���� %04d ms��\r\n", 
						ts_end / ( cpu_clk_freq / 1000000 ),     //����ʱʱ������� us 
						ts_end / ( cpu_clk_freq / 1000 ) );      //����ʱʱ������� ms 

	ts_start = OS_TS_GET();                            //��ȡ��ʱǰʱ���
	
}


static  void  AppTaskTmr ( void * p_arg )
{
	OS_ERR      err;
	OS_TMR      my_tmr;   //���������ʱ������

	
	(void)p_arg;


  /* ���������ʱ�� */
  OSTmrCreate ((OS_TMR              *)&my_tmr,             //�����ʱ������
               (CPU_CHAR            *)"MySoftTimer",       //���������ʱ��
               (OS_TICK              )1,                  //��ʱ����ʼֵ����1Hzʱ�����㣬��Ϊ1s
               (OS_TICK              )1,                  //��ʱ����������ֵ����1Hzʱ�����㣬��Ϊ1s
               (OS_OPT               )OS_OPT_TMR_PERIODIC, //�����Զ�ʱ
               (OS_TMR_CALLBACK_PTR  )TmrCallback,         //�ص�����
               (void                *)"Timer Over!",       //����ʵ�θ��ص�����
               (OS_ERR              *)err);                //���ش�������
							 
	/* ���������ʱ�� */						 
  OSTmrStart ((OS_TMR   *)&my_tmr, //�����ʱ������
              (OS_ERR   *)err);    //���ش�������
					 
	ts_start = OS_TS_GET();                       //��ȡ��ʱǰʱ���
							 
	while (DEF_TRUE) {                            //�����壬ͨ��д��һ����ѭ��

		OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //��������������

	}
	
}

