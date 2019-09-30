/***************************************************************
  *	Name		:	TIMER.C
  *	Data		:	2019.7.25
  *	Author	:	NIEAOWEI
  *	Note		:	��ʱ���ĺ���ʵ��
****************************************************************/
#include "timer.h"
#include "sys.h"

/***************************************************************
  *	@brief	��ʱ��6�ĳ�ʼ����72Ϊ1΢��
  *	@param	void
  *	@retval	void
****************************************************************/
void TIM6_Init(void){
	TIM_TimeBaseInitTypeDef tim6;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	tim6.TIM_ClockDivision=TIM_CKD_DIV1;
	tim6.TIM_CounterMode=TIM_CounterMode_Up;
	tim6.TIM_Prescaler=1-1;
	tim6.TIM_Period=65535-1;
	TIM_TimeBaseInit(TIM6,&tim6);
	
	TIM_Cmd(TIM6,ENABLE);
	
}


/***************************************************************
  *	@brief	���ö�ʱ��6������ʱ�ĺ���
  *	@param	utime ��ʱ��΢���� ��Χ�� 0~910΢��֮�䣬��Ϊ��Ҫ
									�ṩ��ȷ����ʱ
  *	@retval	void
****************************************************************/
void TIM6_Delay_us(int utime){
	
	TIM_SetCounter(TIM6,0);
	while(TIM_GetCounter(TIM6)<(utime*72));
	//printf(" time : %d \r\n",TIM_GetCounter(TIM6));
	
}
