/***************************************************************
  *	Name		:	UART5.C
  *	Data		:	2019.8.2
  *	Author	:	NIEAOWEI
  *	Note		:	�ô�������ָ��ģ��Ŀ���
****************************************************************/
#include "uart5.h"
#include "sys.h"

//����5���IO����
#define UART5_RX_PORT	GPIOD
#define UART5_RX_PIN 	GPIO_Pin_2
#define UART5_RX_CLK	RCC_APB2Periph_GPIOD

#define UART5_TX_PORT	GPIOC
#define UART5_TX_PIN	GPIO_Pin_12
#define UART5_TX_CLK	RCC_APB2Periph_GPIOC

#define UART5_ALL_CLK	UART5_RX_CLK|UART5_TX_CLK

FIFO_Type uart5_rx_fifo;//����5�Ľ��ջ�������

/***************************************************************
  *	@brief	����5���IO�ڡ��жϡ�����ĳ�ʼ��
  *	@param	baudrate ������
  *	@retval	void
****************************************************************/
void UART5_Init(uint32_t baudrate){
	GPIO_InitTypeDef uart5_io;
	USART_InitTypeDef uart5;
	NVIC_InitTypeDef uartit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(UART5_ALL_CLK,ENABLE);
	//io��ʼ��
	uart5_io.GPIO_Mode=GPIO_Mode_AF_PP;
	uart5_io.GPIO_Pin=UART5_TX_PIN;
	uart5_io.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(UART5_TX_PORT,&uart5_io);
	
	uart5_io.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	uart5_io.GPIO_Pin=UART5_RX_PIN;
	GPIO_Init(UART5_RX_PORT,&uart5_io);
	//���ڳ�ʼ��
	uart5.USART_BaudRate=baudrate;
	uart5.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	uart5.USART_Parity=USART_Parity_No;
	uart5.USART_StopBits=USART_StopBits_1;
	uart5.USART_WordLength=USART_WordLength_8b;
	uart5.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//�жϳ�ʼ��
	uartit.NVIC_IRQChannel=UART5_IRQn;
	uartit.NVIC_IRQChannelCmd=ENABLE;
	uartit.NVIC_IRQChannelPreemptionPriority=3;
	uartit.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&uartit);
	//���ý����ж�
	USART_Init(UART5,&uart5);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	FIFO_Init(&uart5_rx_fifo);
	USART_Cmd(UART5,ENABLE);
}
/***************************************************************
  *	@brief	����5���жϺ���
  *	@param	void
  *	@retval	void
****************************************************************/
void UART5_IRQHandler(void){
	uint8_t current;
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==1){
		current=USART_ReceiveData(UART5);
		FIFO_In_Byte(&uart5_rx_fifo,current);
	}
}
/***************************************************************
	*	@brief	����lenth���ȵ�����
	*	@param	*data ��Ҫ���͵����ݵ�ַ
	* @param	lenth ��Ҫ�������ݵĳ���
  *	@retval	uint8_t
										0:�������
****************************************************************/
uint8_t UART5_SendBytes(uint8_t *data,uint8_t lenth){
	uint8_t i=0;
	while(lenth--){
		while(USART_GetFlagStatus(UART5,USART_FLAG_TXE)!=1){};
		USART_SendData(UART5,data[i++]);
	}
	return 0;
}


