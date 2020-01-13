
#ifndef  CPU_CFG_MODULE_PRESENT
#define  CPU_CFG_MODULE_PRESENT
/*   �Ƿ�ʹ��CPU���֣�DEF_ENABLED����DEF_DISABLED*/
#define  CPU_CFG_NAME_EN                        DEF_DISABLED
/* CPU���ִ�С��ASCII�ַ�����ʽ��*/
#define  CPU_CFG_NAME_SIZE                                16  
/* CPUʱ����������� ��ֻ��ѡ������һ����*/
/*  �Ƿ�ʹ��32λ��ʱ���������DEF_ENABLED����DEF_DISABLED*/
#define  CPU_CFG_TS_32_EN                       DEF_ENABLED
/*  �Ƿ�ʹ��64λ��ʱ���������DEF_ENABLED����DEF_DISABLED*/
#define  CPU_CFG_TS_64_EN                       DEF_DISABLED
/* *����CPUʱ�����ʱ���ִ�С */
#define  CPU_CFG_TS_TMR_SIZE                    CPU_WORD_SIZE_32

/* �Ƿ�ʹ�ò���CPU�����жϵ�ʱ��  */
#if 1                                                          
#define  CPU_CFG_INT_DIS_MEAS_EN                               
#endif
/* ���ò����Ĵ���*/                                            
#define  CPU_CFG_INT_DIS_MEAS_OVRHD_NBR                    1u  
/* �Ƿ�ʹ��CPUǰ����ָ���ҪӲ��֧�֣�  */
#if 1                                                      
#define  CPU_CFG_LEAD_ZEROS_ASM_PRESENT                    
#endif
/* �Ƿ�����CPU����β����λ */
#if 0                                                      
#define  CPU_CFG_TRAIL_ZEROS_ASM_PRESENT                   
#endif
/*�Ƿ���CPU�����ִ洢˳��	�μ�ע�ͣ�2*/
#if 0
#define  CPU_CFG_ENDIAN_TYPE            CPU_ENDIAN_TYPE_BIG  
#endif
/*�Ƿ��嶨��CPU�����ִ洢˳�򣨲μ�ע1��*/
#define  CPU_CFG_CACHE_MGMT_EN            DEF_DISABLED       

#endif                        




