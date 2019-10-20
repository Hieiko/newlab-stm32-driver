
/***************************************************************
  *	Name		:	FIFO.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	������ز�����������Ҫ���ڴ��ڽ���
****************************************************************/
#include "fifo.h"

/***************************************************************
  *	@brief	��ʼ������
  *	@param	*FIFO ��Ҫ��ʼ���Ķ���ָ����ַ
  *	@retval	void
****************************************************************/
void FIFO_Init(FIFO_Type *FIFO){
	FIFO->in=FIFO->out=0;
	FIFO->is_full=0;
}
/***************************************************************
  *	@brief	�ж϶����Ƿ�Ϊ��
  *	@param	*FIFO ��Ҫ�жϵĶ���ָ����ַ
  *	@retval	uint8_t
										0:�ǿ�
										��0����
****************************************************************/
uint8_t FIFO_Is_Empty(FIFO_Type *FIFO){
	return FIFO->in==FIFO->out;
}
/***************************************************************
  *	@brief	���ض��е�����־���ж϶����Ƿ�����
  *	@param	*FIFO 
  *	@retval	uint8_t
										1:��
										0:δ��
****************************************************************/
uint8_t FIFO_Is_Full(FIFO_Type *FIFO){
	return FIFO->is_full;
}
/***************************************************************
  *	@brief	���һ�����ݻ�һ���ֽ�
  *	@param	*FIFO ��Ҫ�����Ķ��е�ַ��ָ��
						byte	��ӵĵ�����
  *	@retval	uint16_t
											FIFO_OP_FAIL:ʧ��
											��FIFO_OP_FAIL:�ɹ�
****************************************************************/
uint16_t FIFO_In_Byte(FIFO_Type *FIFO,Byte byte){
	if(FIFO_Is_Full(FIFO)){
		return FIFO_OP_FAIL;
	}
	FIFO->data[FIFO->in&FIFO_MASK]=byte;
	FIFO->in++;
	if((FIFO->in&FIFO_MASK)==(FIFO->out&FIFO_MASK)){
		FIFO->is_full=1;
	}
	return FIFO->in&FIFO_MASK;
}
/***************************************************************
  *	@brief	����һ�����ݻ�һ���ֽ�
  *	@param	*FIFO ��Ҫ�����Ķ��е�ַ��ָ��
						*byte	�洢�������ݵĵ�ַ����ָ��
  *	@retval	uint16_t
											FIFO_OP_FAIL:ʧ��
											��FIFO_OP_FAIL:�ɹ�											
****************************************************************/
uint16_t FIFO_Out_Byte(FIFO_Type *FIFO,Byte *byte){
	if(FIFO_Is_Empty(FIFO)){
		return FIFO_OP_FAIL;
	}
	if(FIFO_Is_Full(FIFO)){
		FIFO->is_full=0;
	}
	*byte=FIFO->data[FIFO->out&FIFO_MASK];
	FIFO->out++;
	return FIFO->out&FIFO_MASK;
}
/***************************************************************
  *	@brief	����һ�����ݣ�������Ϊ���ݽ�����־
  *	@param	*byte �洢���ݵ������ַ
  *	@retval	uint16_t
											FIFO_OP_FAIL:ʧ��
											��FIFO_OP_FAIL:�ɹ�
****************************************************************/
uint16_t FIFO_Out_Bytes(FIFO_Type *FIFO,Byte *byte){
	uint16_t length=0;
	Byte current=0,former=0;
	while(1){
		former=current;
		if(FIFO_Out_Byte(FIFO,&current)==FIFO_OP_FAIL){
			return FIFO_OP_FAIL;
		}else{
			if(former==0x0d && current==0x0a){
				byte[length-1]='\0';
				return length;
			}
			byte[length++]=current;
		}
	}
}



