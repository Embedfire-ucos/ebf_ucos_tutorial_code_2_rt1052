/*
  ******************************************************************************
  * @file    bsp.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   �弶����
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
*                                             �������ļ�
*********************************************************************************************************
*/

//�弶����
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./led/bsp_led.h" 
#include "bsp.h" 

/*
*********************************************************************************************************
* ������ : BSP_Init
* ����   : ���е�Ӳ���豸��Ӧ�÷������������߳�ʼ��
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/
void  BSP_Init (void)
{
  /* ��ʼ���ڴ汣����Ԫ */
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  //BOARD_BootClockRUN();
  /* ��ʼ�����Դ��� */
  BOARD_InitDebugConsole();
 
  /* ��ӡϵͳʱ�� */
  PRINTF("\r\n");
  PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk)); 
  PRINTF("����һ��[Ұ��]-ȫϵ�п�����-uCOS-III  ������  ʵ�飡\n\n");	
	/* Ӳ��BSP��ʼ��ͳͳ�����������LED�����ڣ�LCD�� */
  LED_GPIO_Config(); 		//��ʼ��LED

}
