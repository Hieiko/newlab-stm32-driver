/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.28
  *	Author	:	NIEAOWEI
  *	Note		:	��ʵ��Ϊlcd12864��������ʾʵ��
****************************************************************/
#include "lcd12864.h"
#include "delay.h"

int main(){
	delay_init();
	lcd_init();
	delay_ms(10);
	show_logo();
	
}


