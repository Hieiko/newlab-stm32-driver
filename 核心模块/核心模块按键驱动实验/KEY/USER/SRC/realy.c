/***************************************************************
  *	Name		:	RELAY.H
  *	Data		:	2019.7.8
  *	Author	:	NIEAOWEI
  *	Note		:	�̵�����ز�����ʵ��
****************************************************************/
#include "relay.h"
#include "sys.h"
/*
			�̵���IO�ڵ�����
*/
#define RELAY_CLK			RCC_APB2Periph_GPIOB
#define RELAY_PORT 		GPIOB
#define RELAY_PIN			GPIO_Pin_5
/*
	�Ĵ����������߿⺯������������ʵ�鲻���ṩ���ַ���
*/
#ifdef _sys_h_

#define RELAY					PBout(5)
#define RELAY_ON			RELAY=1
#define RELAY_OFF			RELAY=0
#define RELAY_TOGGLE	RELAY=!RELAY

#else

#define RELAY_ON			GPIO_SetBits(RELAY_PORT,RELAY_PIN)
#define RELAY_OFF			GPIO_ResetBits(RELAY_PORT,RELAY_PIN)

#define RELAY_TOGGLE	GPIO_WriteBit(RELAY_PORT,RELAY_PIN,\
											GPIO_ReadOutputDataBit(RELAY_PORT,RELAY_PIN))

#endif
/***************************************************************
  *	@brief	�̵������IO�ڵĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void Relay_Init(void){
	
	GPIO_InitTypeDef relay;
	
	RCC_APB2PeriphClockCmd(RELAY_CLK,ENABLE);
	
	relay.GPIO_Mode=GPIO_Mode_Out_PP;
	relay.GPIO_Pin=RELAY_PIN;
	relay.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(RELAY_PORT,&relay);
	
	GPIO_ResetBits(RELAY_PORT,RELAY_PIN);
	
}
/***************************************************************
  *	@brief	�̵���IO��״̬��ת������˵���ط�ת
  *	@param	void
  *	@retval	void
****************************************************************/
void Relay_Toggle(void){
	RELAY_TOGGLE;
}
/***************************************************************
  *	@brief	�򿪼̵�������
  *	@param	void
  *	@retval	void
****************************************************************/
void Relay_ON(void){
	RELAY_ON;
}
/***************************************************************
  *	@brief	�رռ̵�������
  *	@param	void
  *	@retval	void
****************************************************************/
void Relay_OFF(void){
	RELAY_OFF;
}
