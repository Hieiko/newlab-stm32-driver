/***************************************************************
  *	Name		:	UART4.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	����3����ز���������������
****************************************************************/
#include "uart4.h"
#include "sys.h"

//����3�����ţ�ʱ������
#define UART4_RX_PORT	GPIOC
#define UART4_RX_PIN		GPIO_Pin_11
#define UART4_RX_CLK		RCC_APB2Periph_GPIOC

#define UART4_TX_PORT	GPIOC
#define UART4_TX_PIN		GPIO_Pin_10
#define UART4_TX_CLK		RCC_APB2Periph_GPIOC

#define UART4_CLK			RCC_APB1Periph_UART4

//���ջ�������Լ����ձ�־
FIFO_Type uart4_rx_fifo;
uint8_t 	uart4_receiving;
/***************************************************************
  *	@brief	�����������io������ĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void uart4_init(){
	GPIO_InitTypeDef uart4;
	
	USART_InitTypeDef uart4_;
	
	NVIC_InitTypeDef uart4_it;
	//ʱ�ӳ�ʼ��
	RCC_APB1PeriphClockCmd(UART4_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(UART4_RX_CLK|UART4_TX_CLK,ENABLE);
	//io�ڲ������ü���ʼ��
	uart4.GPIO_Mode=GPIO_Mode_AF_PP;
	uart4.GPIO_Speed=GPIO_Speed_50MHz;
	uart4.GPIO_Pin=UART4_TX_PIN;
	GPIO_Init(UART4_TX_PORT,&uart4);
	
	uart4.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	uart4.GPIO_Pin=UART4_RX_PIN;
	GPIO_Init(UART4_RX_PORT,&uart4);
	//���ڲ�������
	uart4_.USART_BaudRate=115200;
	uart4_.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	uart4_.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	uart4_.USART_Parity=USART_Parity_No;
	uart4_.USART_StopBits=USART_StopBits_1;
	uart4_.USART_WordLength=USART_WordLength_8b;
	
	//�жϲ�������
	uart4_it.NVIC_IRQChannel=UART4_IRQn;
	uart4_it.NVIC_IRQChannelSubPriority=2;
	uart4_it.NVIC_IRQChannelPreemptionPriority=1;
	uart4_it.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&uart4_it);
	
	USART_Init(UART4,&uart4_);
	//���ý����ж�
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	//ʹ�ܴ���3
	USART_Cmd(UART4,ENABLE);
	//��ʼ���������
	FIFO_Init(&uart4_rx_fifo);
	
}
/***************************************************************
  *	@brief	����3���жϺ��������ڽ������ݣ��洢����Ӧ�������
  *	@param	void
  *	@retval	void
****************************************************************/
void UART4_IRQHandler(void){
	static uint8_t former=0;
	static uint8_t current=0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)){
		former=current;
		current=USART_ReceiveData(UART4);
		FIFO_In_Byte(&uart4_rx_fifo,current);
		//printf("%c",current);
		if(former==0x0d && current==0x0a){
			uart4_receiving=0;
			
		}else{
			uart4_receiving=1;
		}
	}
}
/***************************************************************
	*	@brief	����3����lenth���ֽڵ�����
	*	@param	*data ���͵�����ָ��
	* @param	lenth	���͵����ݳ���
  *	@retval	void
****************************************************************/
void UART4_SendStrlen(uint8_t *data,uint16_t lenth){
	
	while(lenth--){//��������ݣ�����ѭ����û�������ѭ��
		while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)!=SET){};//�жϷ��ͼĴ����գ�����գ��ͽ���ѭ�������ռ���ѭ��
			USART_SendData(UART4,*data);//����һ���ֽڵ����ݳ�ȥ
			data++;
	}
}

