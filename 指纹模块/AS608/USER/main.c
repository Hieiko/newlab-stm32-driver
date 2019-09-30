
/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.8.2
  *	Author	:	NIEAOWEI
  *	Note		:	��ʵ��ΪAS608ָ��ʶ��оƬ������ʵ�飬ʹ�ô���5��Ϊ
							ͨ�Ŷ˿ڡ�
****************************************************************/
#include "usart1.h"
#include "uart5.h"
#include "delay.h"
#include <stdio.h>
#include "as608.h"

int main(){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Usart1_Init(115200);
	UART5_Init(57600);
	delay_init();
	delay_ms(100);
	printf("running.\r\n");
	AS608_Init();
	AS608_Demo();
	while(1){
		
	}
}


