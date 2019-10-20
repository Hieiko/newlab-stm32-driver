/***************************************************************
  *	Name		:	LF125K.H
  *	Data		:	2019.7.25
  *	Author	:	NIEAOWEI
  *	Note		:	LF125��ز����������мǲ��˽�ԭ�������£���Ҫ�޸�
							д������صĲ�����
****************************************************************/
#ifndef _LF125K_H
#define _LF125K_H

#include "stdint.h"

void LF125K_init(void);
void LF125K_demo(void);

/*
	��Ƭ��ַ�Ķ���
*/
typedef enum {
	LF125_PAGE0_BANK0=0x00,
	LF125_PAGE0_BANK1=0x01,
	LF125_PAGE0_BANK2=0x02,
	LF125_PAGE0_BANK3=0x03,
	LF125_PAGE0_BANK4=0x04,
	LF125_PAGE0_BANK5=0x05,
	LF125_PAGE0_BANK6=0x06,
	LF125_PAGE0_BANK7=0x07,
}LF125_PAGE0_BANKx;


/*
	��Ƭ��ز���
*/
int8_t LF125K_read_0_page(uint8_t adr,uint8_t *buf);
int8_t LF125K_write_0_page(uint8_t adr,uint8_t *buf);
int8_t LF125K_write_0_page_protect(uint8_t adr,uint8_t *buf,uint8_t *pwd);
int8_t LF125K_read_0_page_protect(uint8_t adr,uint8_t *buf,uint8_t *pwd);
int8_t LF125K_read_1_page(uint8_t* buf);

#endif


