
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
	while(1){
		if(!FIFO_Is_Empty(&usart1_rx_fifo) && usart1_Receiving==0 ){
			if(FIFO_Out_Bytes(&usart1_rx_fifo,temp)!=FIFO_OP_FAIL){
				printf("%s\r\n",temp);
			}else{
				printf("running...\r\n");
				delay_ms(500);
			}
		}
		delay_ms(1000);
	}
}


