
/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	����1��ʹ�ã����ö��л���������ݣ���֤��������
							���ܱ�����λ��
****************************************************************/
#include "usart1.h"
#include "delay.h"
#include <stdio.h>

int main(){
	Byte temp[20];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Usart1_Init(115200);
	delay_init();
	printf("running...\r\n");
	while(1){
		
		delay_ms(1000);
	}
}


