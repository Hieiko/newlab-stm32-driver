/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.25
  *	Author	:	NIEAOWEI
  *	Note		:			��ʵ��ʵ����T5557��Ƭ�Ķ�д���Ѿ�lf125kģ��Ĳ�
							�����мǲ���������²����޸�д�ĺ�����������ܻ�
							д����Ƭ��
									��ʵ���ʱ��Ҫ��ǳ��ߣ���΢�붼��Ӱ���д������
							����ר�Ų����˶�ʱ��6����ʱ�����ס��ʱ����������
							��Ҫ��
****************************************************************/
#include "usart1.h"
#include "delay.h"
#include <stdio.h>
#include "lf125k.h"
#include "timer.h"
int main(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Usart1_Init(115200);
	delay_init();
	TIM6_Init();
	LF125K_init();
	printf("running...\r\n");
	while(1){
		delay_ms(1500);
		LF125K_demo();
	}

}

