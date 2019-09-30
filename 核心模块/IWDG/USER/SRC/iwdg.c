/***************************************************************
  *	Name		:	IWDG.C
  *	Data		:	2019.7.9
  *	Author	:	NIEAOWEI
  *	Note		:	�������Ź�����ز����������������Ź���ʱ����40khz
****************************************************************/
#include "iwdg.h"
#include "sys.h"
/***************************************************************
  *	@brief	���Ź����˳�ʼ������
  *	@param	void
  *	@retval	void
****************************************************************/
void IWDG_Init(void){
	//Enable 
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(IWDG_Prescaler_16);//����Ԥ��Ƶ
	IWDG_SetReload(0xfff);//��������ֵ
	IWDG_ReloadCounter();//���ؼ�����
	
	IWDG_Enable();//�������Ź�
	
}
/***************************************************************
  *	@brief	ι������
  *	@param	void
  *	@retval	void
****************************************************************/
void IWDG_Feed(void){
	IWDG_ReloadCounter();//
}

