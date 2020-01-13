#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H

/* ------------------ �������� ------------------- */
#define  OS_CFG_ISR_STK_SIZE                         256u       /*ISR��ջ�Ĵ�С*/

#define  OS_CFG_MSG_POOL_SIZE                         32u       /* ֧�ֵ������Ϣ����*/

#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY              10u      	/*����ջ��ʣ���С���ٷְ���ʽ�˴���10%*/

/* -------------------- �������� --------------------- */
#define  OS_CFG_IDLE_TASK_STK_SIZE                    64u       /* ���������ջ��С*/


/* ------------------ ͳ������ ------------------ */
#define  OS_CFG_STAT_TASK_PRIO  ((OS_PRIO)(OS_CFG_PRIO_MAX-2u)) /* ���ȼ�*/
#define  OS_CFG_STAT_TASK_RATE_HZ                     10u       /* ִ���ʣ�1��10 Hz��*/
#define  OS_CFG_STAT_TASK_STK_SIZE                   128u       /* ��ջ��С��CPU_STKԪ�ص�������*/


/* ---------------------- ʱ�ӽ������� ----------------------- */
#define  OS_CFG_TICK_RATE_HZ                        1000u       /*ϵͳ��ʱ�ӽ���(һ��Ϊ10 �� 1000 Hz)*/
#define  OS_CFG_TICK_TASK_PRIO                         1u       /*ʱ�ӽ�����������ȼ�    */
#define  OS_CFG_TICK_TASK_STK_SIZE                   128u       /*ʱ�ӽ�������Ķ�ջ��С */


/* --------------------- ��ʱ������ ----------------------- */
#define  OS_CFG_TMR_TASK_PRIO   ((OS_PRIO)(OS_CFG_PRIO_MAX-3u)) /*����ʱ�����񡱵����ȼ�*/
#define  OS_CFG_TMR_TASK_RATE_HZ                       1u       /*��ʱ����Ƶ��*/
#define  OS_CFG_TMR_TASK_STK_SIZE                    128u       /*��ʱ������Ķ�ջ��С*/

#endif
