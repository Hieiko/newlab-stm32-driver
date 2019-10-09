/***************************************************************
  *	Name		:	USART3.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	����3����ز���������������
****************************************************************/
#include "usart3.h"
#include "sys.h"

//����3�����ţ�ʱ������
#define USART3_RX_PORT	GPIOB
#define USART3_RX_PIN		GPIO_Pin_11
#define USART3_RX_CLK		RCC_APB2Periph_GPIOB

#define USART3_TX_PORT	GPIOB
#define USART3_TX_PIN		GPIO_Pin_10
#define USART3_TX_CLK		RCC_APB2Periph_GPIOB

#define USART3_CLK			RCC_APB1Periph_USART3

//���ջ�������Լ����ձ�־
FIFO_Type usart3_rx_fifo;
uint8_t 	usart3_receiving;
/***************************************************************
  *	@brief	�����������io������ĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void usart3_init(){
	GPIO_InitTypeDef usart3;
	
	USART_InitTypeDef usart3_;
	
	NVIC_InitTypeDef usart3_it;
	//ʱ�ӳ�ʼ��
	RCC_APB1PeriphClockCmd(USART3_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(USART3_RX_CLK|USART3_TX_CLK,ENABLE);
	//io�ڲ������ü���ʼ��
	usart3.GPIO_Mode=GPIO_Mode_AF_PP;
	usart3.GPIO_Speed=GPIO_Speed_50MHz;
	usart3.GPIO_Pin=USART3_TX_PIN;
	GPIO_Init(USART3_TX_PORT,&usart3);
	
	usart3.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	usart3.GPIO_Pin=USART3_RX_PIN;
	GPIO_Init(USART3_RX_PORT,&usart3);
	//���ڲ�������
	usart3_.USART_BaudRate=115200;
	usart3_.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	usart3_.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart3_.USART_Parity=USART_Parity_No;
	usart3_.USART_StopBits=USART_StopBits_1;
	usart3_.USART_WordLength=USART_WordLength_8b;
	
	//�жϲ�������
	usart3_it.NVIC_IRQChannel=USART3_IRQn;
	usart3_it.NVIC_IRQChannelSubPriority=3;
	usart3_it.NVIC_IRQChannelPreemptionPriority=3;
	usart3_it.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&usart3_it);
	
	USART_Init(USART3,&usart3_);
	//���ý����ж�
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	//ʹ�ܴ���3
	USART_Cmd(USART3,ENABLE);
	//��ʼ���������
	FIFO_Init(&usart3_rx_fifo);
	
}
/***************************************************************
  *	@brief	����3���жϺ��������ڽ������ݣ��洢����Ӧ�������
  *	@param	void
  *	@retval	void
****************************************************************/
void USART3_IRQHandler(void){
	static uint8_t current;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET){//����ǽ�������ж�
		current=USART_ReceiveData(USART3);//��ȡ���ܵ������ݴ�ȡcurrent
		FIFO_In_Byte(&usart3_rx_fifo,current);//�ŵ������������
	}
}
/***************************************************************
	*	@brief	����3����lenth���ֽڵ�����
	*	@param	*data ���͵�����ָ��
	* @param	lenth	���͵����ݳ���
  *	@retval	void
****************************************************************/
void usart3_send_str(uint8_t *data,uint16_t lenth){
	
	while(lenth--){//��������ݣ�����ѭ����û�������ѭ��
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!=SET){};//�жϷ��ͼĴ����գ�����գ��ͽ���ѭ�������ռ���ѭ��
			USART_SendData(USART3,*data);//����һ���ֽڵ����ݳ�ȥ
			data++;
	}
}

