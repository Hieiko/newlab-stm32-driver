/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.8
  *	Author	:	NIEAOWEI
  *	Note		:	��ʵ����ͨ��IO�ڿ��Ƽ̵������̵���ԭ�����С����
							���ƴ������صıպϡ�ʵ���Ͼ��Ǽ򵥵�IO�����ʵ�顣
****************************************************************/
#include "relay.h"
#include "delay.h"

int main(){

	delay_init();
	Relay_Init();
	
	while(1)
	{
		delay_ms(250);
		Relay_ON();
		delay_ms(250);
		Relay_OFF();
	}
}

