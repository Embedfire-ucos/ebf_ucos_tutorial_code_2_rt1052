#ifndef OS_CFG_H
#define OS_CFG_H

/* -------------- �������� --------------- */                                               
#define OS_CFG_APP_HOOKS_EN             DEF_ENABLED    /* �Ƿ�ʹ���Ӻ���*/
#define OS_CFG_ARG_CHK_EN               DEF_ENABLED    /* �Ƿ�ʹ�ò������*/
#define OS_CFG_CALLED_FROM_ISR_CHK_EN   DEF_ENABLED    /* �Ƿ�ʹ���жϵ��ü��*/
#define OS_CFG_DBG_EN                   DEF_ENABLED    /* �Ƿ�ʹ��debug*/
#define OS_CFG_DYN_TICK_EN              DEF_DISABLED   /* �Ƿ�ʹ�ö�̬��ѡ*/
#define OS_CFG_INVALID_OS_CALLS_CHK_EN  DEF_ENABLED    /* �Ƿ�ʹ�ü����Ч���ں˵��� */
#define OS_CFG_OBJ_TYPE_CHK_EN          DEF_ENABLED    /* �Ƿ�ʹ�ö������ͼ��*/
#define OS_CFG_TS_EN                    DEF_ENABLED    /* �Ƿ�ʹ��ʱ��� */

#define OS_CFG_PRIO_MAX                 64u            /* �����������ȼ�����������������OS_PRIO��������*/

#define OS_CFG_SCHED_LOCK_TIME_MEAS_EN  DEF_DISABLED   /* �Ƿ������DEF_ENABLED�������Բ������ȳ�������ʱ��*/
#define OS_CFG_SCHED_ROUND_ROBIN_EN     DEF_ENABLED    /* �Ƿ������DEF_ENABLED��ѭ�����ȵĴ���*/

#define OS_CFG_STK_SIZE_MIN             64u            /*��С���������ջ��С*/


/* -------------- �¼���־λ------------- */                                                  
#define OS_CFG_FLAG_EN                  DEF_ENABLED    /*�Ƿ�ʹ���¼���־λ*/
#define OS_CFG_FLAG_DEL_EN              DEF_ENABLED    /*�Ƿ����OSFlagDel()�Ĵ��� */
#define OS_CFG_FLAG_MODE_CLR_EN         DEF_ENABLED    /*�Ƿ��������¼���־λ�Ĵ��� */
#define OS_CFG_FLAG_PEND_ABORT_EN       DEF_ENABLED    /*�Ƿ����OSFlagPendAbort()�Ĵ���*/


 /* --------------- �ڴ���� ----------- */                                                      
#define OS_CFG_MEM_EN                   DEF_ENABLED     /* �Ƿ�ʹ���ڴ���� */


 /* --------------- ������ ------------- */                                                   
#define OS_CFG_MUTEX_EN                 DEF_ENABLED    /*�Ƿ�ʹ�û����� */
#define OS_CFG_MUTEX_DEL_EN             DEF_ENABLED    /*�Ƿ����OSMutexDel()�Ĵ���*/
#define OS_CFG_MUTEX_PEND_ABORT_EN      DEF_ENABLED    /*�Ƿ����OSMutexPendAbort()�Ĵ��� */


 /* ---------- ��Ϣ����---------------- */                                                      
#define OS_CFG_Q_EN                     DEF_ENABLED    /*�Ƿ�ʹ����Ϣ����       */                  
#define OS_CFG_Q_DEL_EN                 DEF_ENABLED    /*�Ƿ����OSQDel()�Ĵ��� */                  
#define OS_CFG_Q_FLUSH_EN               DEF_ENABLED    /*�Ƿ����OSQFlush()�Ĵ��� */                
#define OS_CFG_Q_PEND_ABORT_EN          DEF_ENABLED    /*�Ƿ����OSQPendAbort()�Ĵ��� */            


 /* --------------- �ź��� ------------ */                                                     
#define OS_CFG_SEM_EN                   DEF_ENABLED    /*�Ƿ�ʹ���ź���  */        					
#define OS_CFG_SEM_DEL_EN               DEF_ENABLED    /*�Ƿ����OSSemDel()�Ĵ��� */       
#define OS_CFG_SEM_PEND_ABORT_EN        DEF_ENABLED    /*�Ƿ����OSSemPendAbort()�Ĵ���*/  
#define OS_CFG_SEM_SET_EN               DEF_ENABLED    /*�Ƿ����OSSemSet()�Ĵ���  */      


 /* ----------- ������� -------------- */                                                     
#define OS_CFG_STAT_TASK_EN             DEF_ENABLED    /*�Ƿ�ʹ������ͳ�ƹ��� */
#define OS_CFG_STAT_TASK_STK_CHK_EN     DEF_ENABLED    /*��ͳ�������м�������ջ */

#define OS_CFG_TASK_CHANGE_PRIO_EN      DEF_ENABLED    /*�Ƿ����OSTaskChangePrio()����*/
#define OS_CFG_TASK_DEL_EN              DEF_ENABLED    /*�Ƿ����OSTaskDel()����*/
#define OS_CFG_TASK_IDLE_EN             DEF_ENABLED    /*�Ƿ�ʹ�ÿ�������*/
#define OS_CFG_TASK_PROFILE_EN          DEF_ENABLED    /*�Ƿ�ʹ��OS_TCB�еı�����������*/
#define OS_CFG_TASK_Q_EN                DEF_ENABLED    /*�Ƿ����OSTaskQXXXX()����*/
#define OS_CFG_TASK_Q_PEND_ABORT_EN     DEF_ENABLED    /*�Ƿ����OSTaskQPendAbort()����*/
#define OS_CFG_TASK_REG_TBL_SIZE        1u             /*�����ض��Ĵ���������*/
#define OS_CFG_TASK_STK_REDZONE_EN      DEF_DISABLED   /*�Ƿ�ʹ�ö�ջ redzone */
#define OS_CFG_TASK_STK_REDZONE_DEPTH   8u             /*��ջredzone�����*/
#define OS_CFG_TASK_SEM_PEND_ABORT_EN   DEF_ENABLED    /*�Ƿ����OSTaskSemPendAbort()����*/
#define OS_CFG_TASK_SUSPEND_EN          DEF_ENABLED    /*�Ƿ����OSTaskSuspend()��OSTaskResume()���� */
#define OS_CFG_TASK_TICK_EN             DEF_ENABLED    /*�Ƿ������DEF_ENABLED���ں�tick����*/


/* ------- ���񱾵ش洢���� ------- */                                                   
#define OS_CFG_TLS_TBL_SIZE             0u             /* �Ƿ�������񱾵ش洢��TLS���Ĵ����Ĵ��루DEF_ENABLED��*/


 /* -------- ʱ����� ------------- */                                                     
#define OS_CFG_TIME_DLY_HMSM_EN         DEF_ENABLED    /*�Ƿ����OSTimeDlyHMSM()�Ĵ���*/
#define OS_CFG_TIME_DLY_RESUME_EN       DEF_ENABLED    /*�Ƿ����OSTimeDlyResume()�Ĵ���*/


 /* ---------- ��ʱ������ -------- */                                                     
#define OS_CFG_TMR_EN                   DEF_ENABLED    /*�Ƿ�ʹ�ö�ʱ�� */
#define OS_CFG_TMR_DEL_EN               DEF_ENABLED    /*�Ƿ�֧��OSTmrDel()*/


/*-----------����¼����-----------*/                       
#define OS_CFG_TRACE_EN                 DEF_DISABLED   /*�Ƿ�ʹ�� uC/OS-III ��������*/
#define OS_CFG_TRACE_API_ENTER_EN       DEF_DISABLED   /*�Ƿ�ʹ�� uC/OS-III Trace API������*/
#define OS_CFG_TRACE_API_EXIT_EN        DEF_DISABLED   /*�Ƿ�ʹ�� uC/OS-III Trace API�˳����*/


#endif
