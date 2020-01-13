#ifndef  BSP_H_
#define  BSP_H_


/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef __cplusplus
extern  "C" {                                  /* See Note #1.                                         */
#endif


/*
*********************************************************************************************************
*                                              包含的文件
*********************************************************************************************************
*/
#include "./led/bsp_led.h" 
/*
*********************************************************************************************************
*                                             函数原型
*********************************************************************************************************
*/

void  BSP_Init(void);

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                      
