/**************************************************************************************************************
*
*
*
*
*
*
*
**************************************************************************************************************/

#include "74hc595.h"
#include "sys.h"
#include "delay.h"
#define _74HC595_NOP_()	delay_us(1)

#define IC74HC595_DAT_PORT			GPIOB
#define IC74HC595_DAT_CLK				RCC_APB2Periph_GPIOB
#define IC74HC595_DAT_PIN				GPIO_Pin_0
#define IC74HC595_DAT_Init			IC74HC595_DAT_PORT,IC74HC595_DAT_PIN
#define IC74HC595_DAT(newstaut)						SetBits(IC74HC595_DAT_Init,newstaut)

#define IC74HC595_SCK_PORT		GPIOC
#define IC74HC595_SCK_CLK			RCC_APB2Periph_GPIOC
#define IC74HC595_SCK_PIN			GPIO_Pin_5
#define IC74HC595_SCK_Init		IC74HC595_SCK_PORT,IC74HC595_SCK_PIN
#define IC74HC595_SCK(newstaut)					SetBits(IC74HC595_SCK_Init,newstaut)

#define IC74HC595_RCK_PORT		GPIOC
#define IC74HC595_RCK_CLK			RCC_APB2Periph_GPIOC
#define IC74HC595_RCK_PIN			GPIO_Pin_4
#define IC74HC595_RCK_Init		IC74HC595_RCK_PORT,IC74HC595_RCK_PIN
#define IC74HC595_RCK(newstaut)					SetBits(IC74HC595_RCK_Init,newstaut)

#define IC74HC595_ALL_CLK			IC74HC595_DAT_CLK|IC74HC595_SCK_CLK|IC74HC595_RCK_CLK
/***************************************************************
  *	@brief	��ָ��IO������Ϊ�������ģʽ
  *	@param	port GPIOx
	*	@param	pin	GPIO_Pin_x
  *	@retval	void
****************************************************************/
void HC_InitGpioToOutput(GPIO_TypeDef* port,uint16_t pin){
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_2MHz;
	gpio.GPIO_Pin=pin;
	GPIO_Init(port,&gpio);
	
}

void SetBits(GPIO_TypeDef* port,uint16_t pin,uint8_t staus){
	if(staus==1){
		GPIO_SetBits(port,pin);
	}else{
		GPIO_ResetBits(port,pin);
	}
}

/************************************************************
*������void HC595_GpioInit(void)
*���ܣ�74HC595�ź����Ŷ�Ӧ��I/O�ڹ��ܳ�ʼ��
*���룺��
*�������
*����˵������
************************************************************/
void HC595_GpioInit(void)
{
	RCC_APB2PeriphClockCmd(IC74HC595_ALL_CLK,ENABLE);
	HC_InitGpioToOutput(IC74HC595_DAT_Init);
	HC_InitGpioToOutput(IC74HC595_SCK_Init);
	HC_InitGpioToOutput(IC74HC595_RCK_Init);
	IC74HC595_DAT(1);
	IC74HC595_SCK(1);
	IC74HC595_RCK(1);
}

void hc595_out(uint8_t dH, uint8_t dL)
{
   uint16_t dout;
   uint8_t i;
   dout = (dH<<8) | dL;
   IC74HC595_RCK(0) ;
   IC74HC595_SCK(0);
   // ������λ���
   for(i=0; i<16; i++)
   {
        IC74HC595_SCK(0) ;
        if(dout &0x8000) 
           IC74HC595_DAT(1);
        else
           IC74HC595_DAT(0);
		_74HC595_NOP_();
        IC74HC595_SCK(1) ;
       	_74HC595_NOP_();
	    dout = dout<<1;
   }
   IC74HC595_SCK(0);
   // �������
   IC74HC595_RCK(1) ;
   _74HC595_NOP_();
   _74HC595_NOP_();
   IC74HC595_RCK(0) ;
}

void hc595_out_for_int(uint8_t dH, uint8_t dL)
{
   uint16_t dout;
   uint8_t i;
   dout = (dH<<8) | dL;
   IC74HC595_RCK(0) ;
   IC74HC595_SCK(0);
   // ������λ���
   for(i=0; i<16; i++)
   {
        IC74HC595_SCK(0) ;
        if(dout &0x8000) 
           IC74HC595_DAT(1);
        else
           IC74HC595_DAT(0);
		_74HC595_NOP_();
        IC74HC595_SCK(1) ;
       	_74HC595_NOP_();
	    dout = dout<<1;
   }
   IC74HC595_SCK(0);
   // �������
   IC74HC595_RCK(1) ;
   _74HC595_NOP_();
   _74HC595_NOP_();
   IC74HC595_RCK(0) ;
}
/******************* (C) COPYRIGHT xxx *****END OF FILE****/









