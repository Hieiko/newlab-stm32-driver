/***************************************************************
  *	Name		:	USART3.H
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	����3����ز���������������
****************************************************************/
#ifndef _USART3_H
#define _USART3_H
#include "stdint.h"
#include "fifo.h"

void usart3_init(void);
void usart3_send_str(uint8_t *data,uint16_t lenth);
//����3�Ļ������
extern FIFO_Type usart3_rx_fifo;

#endif

