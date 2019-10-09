/***************************************************************
  *	Name		:	RELAY.H
  *	Data		:	2019.7.8
  *	Author	:	NIEAOWEI
  *	Note		:	�̵�����ز�����ʵ��
****************************************************************/
#include "relay.h"
#include "sys.h"
#include "stdio.h"
/*
			�̵���IO�ڵ�����
*/
#define DOOR_CLK			RCC_APB2Periph_GPIOB
#define DOOR_PORT 		GPIOB
#define DOOR_PIN			GPIO_Pin_5

#define LATERN_CLK			RCC_APB2Periph_GPIOD
#define LATERN_PORT 		GPIOD
#define LATERN_PIN			GPIO_Pin_3

/*
	�Ĵ����������߿⺯������������ʵ�鲻���ṩ���ַ���
*/
#ifdef _SYS_H_

#define RELAY					PBout(5)
#define RELAY_ON			RELAY=1
#define RELAY_OFF			RELAY=0
#define RELAY_TOGGLE	RELAY=!RELAY

#else

#define DOOR_ON				GPIO_SetBits(DOOR_PORT,DOOR_PIN)
#define DOOR_OFF			GPIO_ResetBits(DOOR_PORT,DOOR_PIN)

#define DOOR_STATUS		GPIO_ReadOutputDataBit(DOOR_PORT,DOOR_PIN)

#define DOOR_TOGGLE	GPIO_WriteBit(DOOR_PORT,DOOR_PIN,\
											!GPIO_ReadOutputDataBit(DOOR_PORT,DOOR_PIN))



#endif

uint8_t door_status=0;
uint8_t latern_status=0;
/***************************************************************
  *	@brief	�̵������IO�ڵĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void Relay_Init(void){
	
	GPIO_InitTypeDef relay;
	
	RCC_APB2PeriphClockCmd(DOOR_CLK|LATERN_CLK,ENABLE);
	
	relay.GPIO_Mode=GPIO_Mode_Out_PP;
	relay.GPIO_Pin=DOOR_PIN;
	relay.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(DOOR_PORT,&relay);
	
	relay.GPIO_Pin=LATERN_PIN;
	
	GPIO_Init(LATERN_PORT,&relay);
	
	GPIO_ResetBits(DOOR_PORT,DOOR_PIN);
	GPIO_ResetBits(LATERN_PORT,LATERN_PIN);
	
}
/***************************************************************
  *	@brief	�̵���IO��״̬��ת������˵���ط�ת
  *	@param	void
  *	@retval	void
****************************************************************/
void door_Toggle(void){
	DOOR_TOGGLE;
}

void latern_Toogle(void){

}
/***************************************************************
  *	@brief	�򿪼̵�������
  *	@param	void
  *	@retval	void
****************************************************************/
void door_ON(void){
	DOOR_ON;
	door_status=1;
	printf("door open %d \r\n",door_status);
}

void latern_ON(void){
	GPIO_SetBits(LATERN_PORT,LATERN_PIN);
	latern_status=1;
	printf("latern open %d \r\n",latern_status);
}
/***************************************************************
  *	@brief	�رռ̵�������
  *	@param	void
  *	@retval	void
****************************************************************/
void door_OFF(void){
	DOOR_OFF;
	door_status=0;
	printf("door close %d \r\n",door_status);
}

void latern_OFF(void){
	GPIO_ResetBits(LATERN_PORT,LATERN_PIN);
	latern_status=0;
	printf("latern close %d \r\n",latern_status);
}