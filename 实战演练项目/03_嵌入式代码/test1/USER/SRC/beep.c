/***************************************************************
  *	Name		:	BEEP.C
  *	Data		:	2019.7.8
  *	Author	:	NIEAOWEI
  *	Note		:	��Դ������������������ʱ���������IO�ڲ���һ��Ƶ��
							�ķ������ﵽ����������Ĥ�𶯷�����Ч����
****************************************************************/
#include "beep.h"
#include "delay.h"
//���������IO������
#define BEEP_PORT 	GPIOA
#define BEEP_PIN 		GPIO_Pin_8
#define BEEP_CLK 		RCC_APB2Periph_GPIOA
//������Ƶ������
#define BEEP_INTERVAL 200 //IO toggle interval time.
#define BEEP_DRIVER 	2*BEEP_INTERVAL //IO toggle cycle.
/***************************************************************
  *	@brief	��������ʱ���ŵ�ƽ��ת����
  *	@param	utime ��ת���ʱ�� ��λ����
  *	@retval	void
****************************************************************/
void BEEP_Pin_State_Toggler(uint32_t utime){
	delay_us(utime);
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);
	delay_us(utime);
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}
/***************************************************************
  *	@brief	���������IO�ڵĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void BEEP_Init(void){
	GPIO_InitTypeDef beep;
	
	RCC_APB2PeriphClockCmd(BEEP_CLK,ENABLE);
	
	beep.GPIO_Mode=GPIO_Mode_Out_PP;
	beep.GPIO_Pin=BEEP_PIN;
	beep.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(BEEP_PORT,&beep);
	
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);
}
/***************************************************************
  *	@brief	���������Ų���
  *	@param	mtime ����������ʱ�� ��λ����
  *	@retval	void
****************************************************************/
void BEEP_Play(uint32_t mtime){
	uint32_t i=0;
	while(i<(mtime*1000)){
		BEEP_Pin_State_Toggler(BEEP_INTERVAL);
		i+=BEEP_DRIVER;
	}
}

