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

void uart4_init(void);
void UART4_SendStrlen(uint8_t *data,uint16_t lenth);
//����3�Ļ������
extern FIFO_Type uart4_rx_fifo;
extern uint8_t 	uart4_receiving;

#endif

