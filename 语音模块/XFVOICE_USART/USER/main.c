/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	��ʵ����Ҫ�ǲ�������ģ�飬�����������٣��ж϶���
							�Լ�����ʶ���ܡ�
****************************************************************/
#include "usart3.h"
#include "delay.h"
#include "fifo.h"
#include "xfs5152ce.h"
#include "usart1.h"

int main(){
	uint8_t data[10];
	uint8_t a;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart1_Init(115200);
	usart3_init();
	
	delay_ms(1);
	
	printf("start...\r\n");
	
	xfs_voice_start((unsigned char *)"��˵������ָ��",XFS_FORMAT_GBK,1);
	xfs_voice_start((unsigned char *)"��˵������ָ��",XFS_FORMAT_GBK,1);
	xfs_voice_start((unsigned char *)"��˵������ָ��",XFS_FORMAT_GBK,1);
	printf("%d,%d\r\n",usart3_rx_fifo.in,usart3_rx_fifo.out);
	
	//*delay_ms(1000);
	if(xfs_recognize(&a, 5000) == 1)
	{
		xfs_voice_start((unsigned char *)"����ʶ��ɹ�", XFS_FORMAT_GBK, 1);
		sprintf((char *)data,"%d",a);
		xfs_voice_start((unsigned char *)data,XFS_FORMAT_GBK,1);
		xfs_voice_start((unsigned char *)cmd_word_list[a],XFS_FORMAT_GBK,1);
	}else{
		xfs_voice_start((unsigned char *)"����ʶ��ʧ��",XFS_FORMAT_GBK,1);
	}
	printf("re :%d\r\n",a);
	for(a=0;a<100;a++){
		printf("%x,",usart3_rx_fifo.data[a]);
	}
	printf("%d,%d\r\n",usart3_rx_fifo.in,usart3_rx_fifo.out);
	
	while(1){
		delay_ms(1000);
		printf("runnig...\r\n");
	};
		
}


