#include "./bsp/dma_uart/bsp_dma_uart.h"
#include "fsl_debug_console.h"  
#include "fsl_lpuart_edma.h"
#include  "os.h"

uint8_t ucTemp[UCTEMP_LEN];

/*�������*/
lpuart_edma_handle_t g_lpuartEdmaHandle;
edma_handle_t g_lpuartTxEdmaHandle;
edma_handle_t g_lpuartRxEdmaHandle;
lpuart_transfer_t receiveXfer;
/*ȫ�ֱ���*/
int Get_Uart_Data_flag=0;
uint32_t data;

uint8_t g_rxBuffer[RX_BUFFER_LEN] = {0};
/*
*�������ܣ���ʼ����������
ע��:��ʵ��ʹ�õ���UART1,UART1��ϵͳ���ڣ���ϵͳ�������Ѿ���������ŵĳ�ʼ��
�������������������UART1���ų�ʼ�������ʹ��������������������ų�ʼ�������Ǳ�����ӵġ�
*/
void UART_GPIO_Init(void)
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t adc_config;
  /*ADC��GPIO����*/   
  adc_config.direction = kGPIO_DigitalInput; //����ģʽ
//adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ��������ģʽ�����ø�ѡ����Ч
  adc_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  
  IOMUXC_SetPinMux(UART_RX_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(UART_TX_IOMUXC, 0U);
  
  IOMUXC_SetPinConfig(UART_RX_IOMUXC, UART_RX_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(UART_TX_IOMUXC, UART_TX_PAD_CONFIG_DATA);
  
  GPIO_PinInit(UART_RX_GPIO, UART_RX_GPIO_PIN, &adc_config);
  
  adc_config.direction = kGPIO_DigitalOutput; //����ģʽ
  adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  GPIO_PinInit(UART_TX_GPIO, UART_TX_GPIO_PIN, &adc_config);
  
}


/*
*�������ܣ���ʼ������
*/
void UART_Init(void)
{
  lpuart_config_t lpuartConfig;//����LUART��ʼ���ṹ��
  
  /*��ʼ��UART����*/
  UART_GPIO_Init();
  /* LPUART.Ĭ������ */
  /*
  * lpuartConfig.baudRate_Bps = 115200U;
  * lpuartConfig.parityMode = kLPUART_ParityDisabled;
  * lpuartConfig.stopBitCount = kLPUART_OneStopBit;
  * lpuartConfig.txFifoWatermark = 0;
  * lpuartConfig.rxFifoWatermark = 0;
  * lpuartConfig.enableTx = false;
  * lpuartConfig.enableRx = false;
  */
  LPUART_GetDefaultConfig(&lpuartConfig);
  lpuartConfig.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
  lpuartConfig.enableTx = true;
  lpuartConfig.enableRx = true;
  
  LPUART_Init(DEMO_LPUART, &lpuartConfig, DEMO_LPUART_CLK_FREQ);
}

/*
	���ڿ����ж�
*/
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{

}
extern uint8_t ucTemp[];
extern OS_TCB	 AppTaskUsartTCB;
/*
*�������ܣ���ʼ��������ʹ�õ�DMA
*/
void UART_DMA_Init(void)
{
  edma_config_t config;//����eDMA��ʼ���ṹ��
  
  /*��ʼ��DMAMUX */
  DMAMUX_Init(EXAMPLE_LPUART_DMAMUX_BASEADDR);
  /* ΪLPUART����DMA����ͨ��*/
  DMAMUX_SetSource(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL, LPUART_TX_DMA_REQUEST);
  DMAMUX_SetSource(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL, LPUART_RX_DMA_REQUEST);
  DMAMUX_EnableChannel(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL);
  DMAMUX_EnableChannel(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL);
  
  /* ��ʼ��DMA */
  EDMA_GetDefaultConfig(&config);
  EDMA_Init(EXAMPLE_LPUART_DMA_BASEADDR, &config);
  /*����eDMA�����*/
  EDMA_CreateHandle(&g_lpuartTxEdmaHandle, EXAMPLE_LPUART_DMA_BASEADDR, LPUART_TX_DMA_CHANNEL);
  EDMA_CreateHandle(&g_lpuartRxEdmaHandle, EXAMPLE_LPUART_DMA_BASEADDR, LPUART_RX_DMA_CHANNEL);
  
  /* ���� LPUART DMA ��� */
  LPUART_TransferCreateHandleEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxEdmaHandle,
                                  &g_lpuartRxEdmaHandle);
	/*ʹ�ܿ����ж�*/
	LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_IdleLineInterruptEnable);
	/*ʹ�ܴ����ж�**/
	EnableIRQ(DEMO_LPUART_IRqn);
	
	receiveXfer.data = ucTemp; //��ַ����
	receiveXfer.dataSize = UCTEMP_LEN;    //���յ��������
	LPUART_ReceiveEDMA(LPUART1, &g_lpuartEdmaHandle, &receiveXfer);  //ʹ��eDMA����
}

void DEBUG_UART_IRQHandler(void)
{
		OS_ERR   err;
	OSIntEnter(); 	                             //�����ж�

		/*	���յ��������˴����ж�	*/
    if ((kLPUART_IdleLineFlag) & LPUART_GetStatusFlags(DEMO_LPUART))
    {
			/*��������ж�*/
			DEMO_LPUART->STAT |= LPUART_STAT_IDLE_MASK; 
			/*����eDMA��������*/
			LPUART_TransferGetReceiveCountEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &data); 
			Get_Uart_Data_flag=1;
			LPUART_TransferAbortReceiveEDMA(DEMO_LPUART, &g_lpuartEdmaHandle);   //eDMA��ֹ��������
			LPUART_ReceiveEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &receiveXfer);  //ʹ��eDMA����
			
						/* ���������ź��������� AppTaskKey */
			OSTaskSemPost((OS_TCB  *)&AppTaskUsartTCB,   //Ŀ������
										(OS_OPT   )OS_OPT_POST_NONE, //ûѡ��Ҫ��
									(OS_ERR  *)&err);            //���ش�������	
    }
		
	 OSIntExit();	                               //�˳��ж�
}
