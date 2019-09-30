/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.10
  *	Author	:	NIEAOWEI
  *	Note		:	��ʵ��ΪIO����ʵ�飬�ṩ����ʱ����������״̬������
							������
							��ʵ��Ч��Ϊ����1�������������У�2���̵������ط�ת
****************************************************************/
#include "delay.h"
#include "key.h"
#include "beep.h"
#include "relay.h"


int main(){
	uint8_t key;
	delay_init();
	KEY_Init();
	Relay_Init();
	BEEP_Init();
	BEEP_Play(200);
	
	while(1){
		key=Key_Scan();
		if(key==KEY1_PRESSED){
			BEEP_Play(200);
		}
		if(key==KEY2_PRESSED){
			Relay_Toggle();
		}
	}
}

