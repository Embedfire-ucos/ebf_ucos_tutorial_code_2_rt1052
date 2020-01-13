/*
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   二值信号量-标志事件
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/
/*
*********************************************************************************************************
*                                            包含的文件
*********************************************************************************************************
*/
//ucosiii系统相关
#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>
#include  <bsp_int.h>
#include  "os_app_hooks.h"
#include  "app_cfg.h"
//板级驱动
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

OS_FLAG_GRP flag_grp;                   //声明事件标志组

#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1

/*
*********************************************************************************************************
*                                         任务控制块TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;      //任务控制块

static  OS_TCB   AppTaskPostTCB;
static  OS_TCB   AppTaskPendTCB;
/*
*********************************************************************************************************
*                                            任务堆栈
*********************************************************************************************************
*/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //任务堆栈

static  CPU_STK  AppTaskPostStk [ APP_TASK_POST_STK_SIZE ];
static  CPU_STK  AppTaskPendStk [ APP_TASK_PEND_STK_SIZE ];
/*
*********************************************************************************************************
*                                            函数原型
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);               //任务函数声明

static  void  AppTaskPost   ( void * p_arg );
static  void  AppTaskPend   ( void * p_arg );

/*
*********************************************************************************************************
* 函数名 : main
* 描述   : 标准的C函数入口
* 形参   : 无
* 返回值 : 无
*********************************************************************************************************
*/
int  main (void)
{
    OS_ERR  err;


    BSP_ClkInit();                       /* 初始化系统时钟       */
    BSP_IntInit();                       /* 初始化RAM中断向量表. */
    BSP_OS_TickInit();                   /* 初始化内核计时器     */
															
    Mem_Init();                          /* 初始化内存管理模块   */
    CPU_IntDis();                        /* 禁用所有中断         */
    CPU_Init();                          /* 初始化uC/CPU相关     */

    OSInit(&err);                        /*初始化uC / OS-III     */
    if (err != OS_ERR_NONE) {
        while (1);
    }

    App_OS_SetAllHooks();                                       						//设置所有应用程序钩子函数

	/* 创建起始任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Start",                            //任务名称
                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);     
    if (err != OS_ERR_NONE) {
        while (1);
    }

    OSStart(&err);                                          								//启动多任务管理（交由uC/OS-III控制）

    while (DEF_ON) {                                           
        ;
    }
}


/*
*********************************************************************************************************
* 函数名 ：AppTaskStart
* 描述   : 这是一个启动任务，在多任务系统启动后，必须初始化滴答计数器(在 BSP_Init 中实现)。
* 形参   : p_arg   是OSTaskCreate()在创建该任务时传递过来的形参。
* 返回值 : 无
* 注意   : 1) 第一行代码 (void)p_arg; 是为了防止编译器报错，因为形参p_arg并没有用到
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;
	
   (void)p_arg;

    OS_TRACE_INIT();                               //初始化uC / OS-III跟踪记录器

    BSP_OS_TickEnable();                           //启用滴答计时器和中断
    BSP_Init();                                    //板级初始化

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                  //无需任务运行即可计算CPU容量
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

  /* 创建事件标志组 flag_grp */
  OSFlagCreate ((OS_FLAG_GRP  *)&flag_grp,        //指向事件标志组的指针
                (CPU_CHAR     *)"FLAG For Test",  //事件标志组的名字
                (OS_FLAGS      )0,                //事件标志组的初始值
                (OS_ERR       *)&err);					  //返回错误类型
							  

		/* 创建 AppTaskPost 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskPostTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Post",                             //任务名称
                 (OS_TASK_PTR ) AppTaskPost,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_POST_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskPostStk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_POST_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_POST_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

		/* 创建 AppTaskPend 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskPendTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Pend",                             //任务名称
                 (OS_TASK_PTR ) AppTaskPend,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_PEND_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskPendStk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_PEND_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_PEND_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
    
		OSTaskDel ( & AppTaskStartTCB, & err );                     //删除起始任务本身，该任务不再运行
		
							
}

/*
*********************************************************************************************************
*                                          POST TASK
*********************************************************************************************************
*/
static  void  AppTaskPost ( void * p_arg )
{
	OS_ERR      err;

	
	(void)p_arg;

					 
	while (DEF_TRUE) {                                                     //任务体
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON ) //如果KEY1被按下
		{		                                                    //点亮LED1
			PRINTF("KEY1被按下\n");
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp,                             //将标志组的BIT0置1
                  (OS_FLAGS      )KEY1_EVENT,
                  (OS_OPT        )OS_OPT_POST_FLAG_SET,
                  (OS_ERR       *)&err);

		}

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON ) //如果KEY2被按下
		{		                                                    //点亮LED2
			PRINTF("KEY2被按下\n");
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp,                             //将标志组的BIT1置1
                  (OS_FLAGS      )KEY2_EVENT,
                  (OS_OPT        )OS_OPT_POST_FLAG_SET,
                  (OS_ERR       *)&err);

		}

		OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );  //每20ms扫描一次
		
	}

}


/*
*********************************************************************************************************
*                                          PEND TASK
*********************************************************************************************************
*/
static  void  AppTaskPend ( void * p_arg )
{
	OS_ERR      err;
  OS_FLAGS      flags_rdy;
	
	(void)p_arg;

					 
	while (DEF_TRUE) {                                       //任务体
  //等待标志组的的BIT0和BIT1均被置1 
  flags_rdy =   OSFlagPend ((OS_FLAG_GRP *)&flag_grp,                 
                (OS_FLAGS     )( KEY1_EVENT | KEY2_EVENT ),
                (OS_TICK      )0,
                (OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
		                           OS_OPT_PEND_BLOCKING |
                                OS_OPT_PEND_FLAG_CONSUME,
                (CPU_TS      *)0,
                (OS_ERR      *)&err);
    if((flags_rdy & (KEY1_EVENT|KEY2_EVENT)) == (KEY1_EVENT|KEY2_EVENT)) 
    {
      /* 如果接收完成并且正确 */
      PRINTF ( "KEY1与KEY2都按下\n");		
      LED1_TOGGLE;       //LED1	反转
    }
                                  

	}
	
}

