/***************************************************************
  *	Name		:	UART5.H
  *	Data		:	2019.8.2
  *	Author	:	NIEAOWEI
  *	Note		:	����5����ز�������
****************************************************************/
#ifndef _UART5_H
#define _UART5_H
#include "stdint.h"
#include "fifo.h"
extern FIFO_Type uart5_rx_fifo;//����5�Ľ��ջ�������
void UART5_Init(uint32_t baudrate);
uint8_t UART5_SendBytes(uint8_t *data,uint8_t lenth);

#endif


