/***************************************************************
  *	Name		:	RELAY.C
  *	Data		:	2019.7.8
  *	Author	:	NIEAOWEI
  *	Note		:	�̵�����غ�����ʵ�֣��򵥵�˵����IO�ڸߵ͵�ƽ�����
							�̵���ԭ�����С�������ƴ������صıպϡ�ʵ���Ͼ���
							�򵥵�IO�����ʵ�顣
****************************************************************/
#include "relay.h"
#include "sys.h"

//Define the parameters of the relay IO port.
#define RELAY_CLK 			RCC_APB2Periph_GPIOB
#define RELAY_PORT 			GPIOB
#define RELAY_PIN 			GPIO_Pin_5
#define RELAY_ON				GPIO_SetBits(RELAY_PORT,RELAY_PIN)
#define RELAY_OFF				GPIO_ResetBits(RELAY_PORT,RELAY_PIN)
/***************************************************************
  *	@brief	�̵������IO�ĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void Relay_Init(){
	GPIO_InitTypeDef relay;//Declare the IO initialization structure
	
	RCC_APB2PeriphClockCmd(RELAY_CLK,ENABLE);//Enable the relevant clock.
	
	//Set the parameters of the IO port
	relay.GPIO_Pin=RELAY_PIN;
	relay.GPIO_Mode=GPIO_Mode_Out_PP;//Ste push-pull-output mode.
	relay.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(RELAY_PORT,&relay);
	
	GPIO_ResetBits(RELAY_PORT,RELAY_PIN);//Initialization IO status.
	
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

