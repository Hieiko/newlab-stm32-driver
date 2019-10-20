/***************************************************************
  *	Name		:	LCD12864.C
  *	Data		:	2019.7.28
  *	Author	:	NIEAOWEI
  *	Note		:	lcd12864����
****************************************************************/
#include 	"lcd12864.h"
#include	"sys.h"
#include	"delay.h"
#include 	"logo2518.h"
#include 	"logo2518_2.h"

//12864�Ĳ�������
#define CONCTROLLER_CS1					1
#define CONCTROLLER_CS2					2

#define LCD_PAGE_ROW_NUM				8

#define CHAR_ROW_PIXEL					16
#define CHAR_COLUMN_PIXEL				8
#define LCD_ROW_PIXEL_SUM				64
#define LCD_COLUMN_PIXEL_SUM		128
#define LCD_ROW_NUM						(LCD_ROW_PIXEL_SUM/CHAR_ROW_PIXEL)
#define LCD_COLUMN_NUM				(LCD_COLUMN_PIXEL_SUM/CHAR_COLUMN_PIXEL)

/*
���к������ڿ��ٳ�ʼ��IO
		����IO�ڵĸ���
*/
/***************************************************************
  *	@brief	��ָ��IO������Ϊ�������ģʽ
  *	@param	port GPIOx
	*	@param	pin	GPIO_Pin_x
  *	@retval	void
****************************************************************/
void InitGpioToOutput(GPIO_TypeDef* port,uint16_t pin){
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Pin=pin;
	GPIO_Init(port,&gpio);
	
}
/***************************************************************
  *	@brief	��ָ��IO������Ϊ��������ģʽ
  *	@param	port GPIOx
	*	@param	pin	GPIO_Pin_x
  *	@retval	void
****************************************************************/
void InitGpioToInput(GPIO_TypeDef* port,uint16_t pin){
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_IPU;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Pin=pin;
	GPIO_Init(port,&gpio);
	
}
/*
�´�½�������ӱ�
BL		-> 	CS
RST		->	DB15
CS2		->	DB14
CS1		->	DB13
E			->	DB4
RW		->	DB3
RS		->	DB2

*/
//���õĶ��壬��������б䶯���޸�һ�����ü���
#define LCD_BL_PIN			GPIO_Pin_7  //����ƣ�0��ʹ�ܣ�1��ʧ��
#define LCD_BL_PORT			GPIOD //CS
#define LCD_BL_CLK			RCC_APB2Periph_GPIOE
#define LCD_BL_INIT			LCD_BL_PORT,LCD_BL_PIN

#define LCD_RST_PIN			GPIO_Pin_10	//��λ��0����λ
#define LCD_RST_PORT		GPIOD //
#define LCD_RST_CLK			RCC_APB2Periph_GPIOE
#define LCD_RST_INIT		LCD_RST_PORT,LCD_RST_PIN

#define LCD_CS2_PIN			GPIO_Pin_9	//оƬ2ѡ��1��ѡ��
#define LCD_CS2_PORT		GPIOD
#define LCD_CS2_CLK			RCC_APB2Periph_GPIOE
#define LCD_CS2_INIT		LCD_CS2_PORT,LCD_CS2_PIN

#define LCD_CS1_PIN			GPIO_Pin_8	//оƬ2ѡ��1��ѡ��
#define LCD_CS1_PORT		GPIOD
#define LCD_CS1_CLK			RCC_APB2Periph_GPIOE
#define LCD_CS1_INIT		LCD_CS1_PORT,LCD_CS1_PIN

#define LCD_E_PIN   		GPIO_Pin_7	//ʹ�ܣ�0��ʹ�ܣ�1��ʧ��
#define LCD_E_PORT			GPIOE
#define LCD_E_CLK				RCC_APB2Periph_GPIOE
#define LCD_E_INIT			LCD_E_PORT,LCD_E_PIN

#define LCD_RW_PIN			GPIO_Pin_1	//��д�źţ�0��д��1����
#define	LCD_RW_PORT			GPIOD
#define LCD_RW_CLK			RCC_APB2Periph_GPIOE
#define LCD_RW_INIT			LCD_RW_PORT,LCD_RW_PIN

#define LCD_RS_PIN			GPIO_Pin_0	//����ָ��ѡ���źţ�0��ָ�1������
#define LCD_RS_PORT			GPIOD
#define LCD_RS_CLK			RCC_APB2Periph_GPIOD
#define LCD_RS_INIT			LCD_RS_PORT,LCD_RS_PIN

//����Ϊ������
#define LCD_DATA0_PIN		GPIO_Pin_8	//
#define	LCD_DATA0_PORT	GPIOE
#define LCD_DATA0_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA0_INIT	LCD_DATA0_PORT,LCD_DATA0_PIN

#define LCD_DATA1_PIN		GPIO_Pin_9	//
#define	LCD_DATA1_PORT	GPIOE
#define LCD_DATA1_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA1_INIT	LCD_DATA1_PORT,LCD_DATA1_PIN

#define LCD_DATA2_PIN		GPIO_Pin_10	//
#define	LCD_DATA2_PORT	GPIOE
#define LCD_DATA2_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA2_INIT	LCD_DATA2_PORT,LCD_DATA2_PIN

#define LCD_DATA3_PIN		GPIO_Pin_11	//
#define	LCD_DATA3_PORT	GPIOE
#define LCD_DATA3_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA3_INIT	LCD_DATA3_PORT,LCD_DATA3_PIN

#define LCD_DATA4_PIN		GPIO_Pin_12	//
#define	LCD_DATA4_PORT	GPIOE
#define LCD_DATA4_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA4_INIT	LCD_DATA4_PORT,LCD_DATA4_PIN

#define LCD_DATA5_PIN		GPIO_Pin_13	//
#define	LCD_DATA5_PORT	GPIOE
#define LCD_DATA5_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA5_INIT	LCD_DATA5_PORT,LCD_DATA5_PIN

#define LCD_DATA6_PIN		GPIO_Pin_14	//
#define	LCD_DATA6_PORT	GPIOE
#define LCD_DATA6_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA6_INIT	LCD_DATA6_PORT,LCD_DATA6_PIN

#define LCD_DATA7_PIN		GPIO_Pin_15	//
#define	LCD_DATA7_PORT	GPIOE
#define LCD_DATA7_CLK		RCC_APB2Periph_GPIOE
#define LCD_DATA7_INIT	LCD_DATA7_PORT,LCD_DATA7_PIN

#define LCD_GPIOD						GPIOD
#define LCD_GPIOD_PIN				LCD_BL_PIN|LCD_RST_PIN|LCD_CS2_PIN|LCD_CS1_PIN|LCD_E_PIN\
														|LCD_RW_PIN|LCD_RS_PIN

#define LCD_GPIOE						GPIOE
#define LCD_GPIOE_PIN				LCD_DATA0_PIN|LCD_DATA1_PIN|LCD_DATA2_PIN|LCD_DATA3_PIN| \
														LCD_DATA4_PIN|LCD_DATA5_PIN|LCD_DATA6_PIN|LCD_DATA7_PIN| \
														LCD_E_PIN

#define LCD_ALL_CLK					LCD_BL_CLK|LCD_RST_CLK|LCD_CS2_CLK|LCD_CS1_CLK|LCD_E_CLK| \
														LCD_RW_CLK|LCD_RS_CLK|LCD_DATA0_CLK|LCD_DATA1_CLK|LCD_DATA2_CLK| \
														LCD_DATA3_CLK|LCD_DATA4_CLK|LCD_DATA5_CLK|LCD_DATA6_CLK|LCD_DATA7_CLK


#define ASCILL_TO_CODE_INDEX	32
// 8 x 16 font
// 1 pixel space at left and bottom
// index = ASCII - 32
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
uint8_t char_code_8x16[][16] =
{
/*--  ����:  space --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  !  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x00,0x00,0x00,0x00},

/*--  ����:  "  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x10,0x0C,0x02,0x10,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  #  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x40,0xC0,0x78,0x40,0xC0,0x78,0x00,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x00},

/*--  ����:  $  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x70,0x88,0x88,0xFC,0x08,0x30,0x00,0x00,0x18,0x20,0x20,0xFF,0x21,0x1E,0x00},

/*--  ����:  %  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0xF0,0x08,0xF0,0x80,0x60,0x18,0x00,0x00,0x00,0x31,0x0C,0x03,0x1E,0x21,0x1E,0x00},

/*--  ����:  &  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x2C,0x19,0x27,0x21,0x10},

/*--  ����:  '  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x12,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  (  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00},

/*--  ����:  )  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00},

/*--  ����:  *  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00},

/*--  ����:  +  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x01,0x01,0x01},

/*--  ����:  ,  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x70,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  -  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00},

/*--  ����:  .  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  /  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0xC0,0x38,0x04,0x00,0x00,0x60,0x18,0x07,0x00,0x00,0x00,0x00},

/*--  ����:  0  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},

/*--  ����:  1  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00},

/*--  ����:  2  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},

/*--  ����:  3  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00},

/*--  ����:  4  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24},

/*--  ����:  5  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00},

/*--  ����:  6  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00},

/*--  ����:  7  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00},

/*--  ����:  8  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},

/*--  ����:  9  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00},

/*--  ����:  :  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},

/*--  ����:  ;  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00},

/*--  ����:  <  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00},

/*--  ����:  =  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x00},

/*--  ����:  >  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},

/*--  ����:  ?  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x70,0x48,0x08,0x08,0x88,0x70,0x00,0x00,0x00,0x00,0x30,0x37,0x00,0x00,0x00},

/*--  ����:  @  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x28,0x2F,0x28,0x17,0x00},

/*--  ����:  A  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,
0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},

/*--  ����:  B  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},

/*--  ����:  C  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},

/*--  ����:  D  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},

/*--  ����:  E  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},

/*--  ����:  F  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},

/*--  ����:  G  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},

/*--  ����:  H  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},

/*--  ����:  I  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},

/*--  ����:  J  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},

/*--  ����:  K  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},

/*--  ����:  L  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},

/*--  ����:  M  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x01,0x3E,0x01,0x3F,0x20,0x00},

/*--  ����:  N  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},

/*--  ����:  O  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},

/*--  ����:  P  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},

/*--  ����:  Q  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x28,0x28,0x30,0x50,0x4F,0x00},

/*--  ����:  R  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},

/*--  ����:  S  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},

/*--  ����:  T  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},

/*--  ����:  U  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},

/*--  ����:  V  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},

/*--  ����:  W  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00,0x00,0x03,0x3E,0x01,0x3E,0x03,0x00,0x00},

/*--  ����:  X  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},

/*--  ����:  Y  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},

/*--  ����:  Z  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},

/*--  ����:  [  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00},

/*--  ����:  \  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x04,0x38,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00},

/*--  ����:  ]  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00},

/*--  ����:  ^  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x04,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  _  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},

/*--  ����:  `  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

/*--  ����:  a  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00},

/*--  ����:  b  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},

/*--  ����:  c  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},

/*--  ����:  d  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},

/*--  ����:  e  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00},

/*--  ����:  f  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},

/*--  ����:  g  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},

/*--  ����:  h  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x10,0xF0,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},

/*--  ����:  i  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},

/*--  ����:  j  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},

/*--  ����:  k  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x10,0xF0,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x06,0x29,0x30,0x20,0x00},

/*--  ����:  l  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},

/*--  ����:  m  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},

/*--  ����:  n  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},

/*--  ����:  o  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},

/*--  ����:  p  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0x91,0x20,0x20,0x11,0x0E,0x00},

/*--  ����:  q  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0x91,0xFF,0x80},

/*--  ����:  r  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},

/*--  ����:  s  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},

/*--  ����:  t  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x00},

/*--  ����:  u  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},

/*--  ����:  v  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00},

/*--  ����:  w  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x01,0x0E,0x30,0x0C,0x07,0x38,0x06,0x01},

/*--  ����:  x  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x20,0x31,0x0E,0x2E,0x31,0x20,0x00},

/*--  ����:  y  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x81,0x86,0x78,0x18,0x06,0x01,0x00},

/*--  ����:  z  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},

/*--  ����:  {  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0x00,0xFC,0x02,0x02,0x00,0x00,0x00,0x00,0x01,0x3E,0x40,0x40},

/*--  ����:  |  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},

/*--  ����:  }  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x02,0x02,0xFC,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x3E,0x01,0x00,0x00,0x00,0x00},

/*--  ����:  ~  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --*/
{0x00,0x02,0x01,0x02,0x02,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

/*���º���Ϊ�������������������������ã���װ*/
/***************************************************************
  *	@brief	ʹ�ܱ����
  *	@param	void
  *	@retval	void
****************************************************************/
void enableBL(void){
	GPIO_ResetBits(LCD_BL_PORT,LCD_BL_PIN);
}
/***************************************************************
  *	@brief	ʧ�ܱ����
  *	@param	void
  *	@retval	void
****************************************************************/
void disableBL(void){
	GPIO_SetBits(LCD_BL_PORT,LCD_BL_PIN);
}
/***************************************************************
  *	@brief	ʹ��д
  *	@param	void
  *	@retval	void
****************************************************************/
void enableWrite(void){
	GPIO_ResetBits(LCD_RW_PORT,LCD_RW_PIN);
}
/***************************************************************
  *	@brief	ʹ�ܶ�
  *	@param	void
  *	@retval	void
****************************************************************/
void enableRead(void){
	GPIO_SetBits(LCD_RW_PORT,LCD_RW_PIN);
}
/***************************************************************
  *	@brief	ʹ��lcd
  *	@param	void
  *	@retval	void
****************************************************************/
void enableLCD(void){
	GPIO_ResetBits(LCD_E_PORT,LCD_E_PIN);
}
/***************************************************************
  *	@brief	ʧ��lcd
  *	@param	void
  *	@retval	void
****************************************************************/
void disableLCD(void){
	GPIO_SetBits(LCD_E_PORT,LCD_E_PIN);
}
/***************************************************************
  *	@brief	ʧ�������ź�
  *	@param	void
  *	@retval	void
****************************************************************/
void enableData(void){
	GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN);
}
/***************************************************************
  *	@brief	ʹ�������ź�
  *	@param	void
  *	@retval	void
****************************************************************/
void enableCmd(void){
	GPIO_ResetBits(LCD_RS_PORT,LCD_RS_PIN);
}
/***************************************************************
  *	@brief	�ж�оƬ�Ƿ���ȷ��Χ
  *	@param	controller_csX ���жϵ�оƬ���
  *	@retval	uint8_t
										0:������
										1������
****************************************************************/
uint8_t is_csX(uint8_t controller_csX){
	if(controller_csX!=CONCTROLLER_CS1 && controller_csX!=CONCTROLLER_CS2){
		return 0;
	}else{
		return 1;
	}
}
/***************************************************************
  *	@brief	ʹ��оƬx
  *	@param	controller_csX ��Ҫʹ�ܵ�оƬ���
  *	@retval	void
****************************************************************/
void enableCSx(uint8_t controller_csX){
	if(controller_csX==CONCTROLLER_CS1){
		GPIO_SetBits(LCD_CS1_PORT,LCD_CS1_PIN);
	}else{
		GPIO_SetBits(LCD_CS2_PORT,LCD_CS2_PIN);
	}
}
/***************************************************************
  *	@brief	ʧ��оƬx
  *	@param	controller_csX ��Ҫʧ�ܵ�оƬ���
  *	@retval	void
****************************************************************/
void disableCSx(uint8_t controller_csX){
	if(controller_csX==CONCTROLLER_CS1){
		GPIO_ResetBits(LCD_CS1_PORT,LCD_CS1_PIN);
	}else{
		GPIO_ResetBits(LCD_CS2_PORT,LCD_CS2_PIN);
	}
}
/*����������������*/
/***************************************************************
  *	@brief	lcd���io�ڵĳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void lcd_gpio_init(void){
	
	RCC_APB2PeriphClockCmd(LCD_ALL_CLK,ENABLE);
	
	InitGpioToOutput(LCD_BL_INIT);
	InitGpioToOutput(LCD_RST_INIT);
	InitGpioToOutput(LCD_E_INIT);
	InitGpioToOutput(LCD_CS1_INIT);
	InitGpioToOutput(LCD_CS2_INIT);
	InitGpioToOutput(LCD_RW_INIT);
	InitGpioToOutput(LCD_RS_INIT);
	InitGpioToOutput(LCD_DATA0_INIT);
	InitGpioToOutput(LCD_DATA1_INIT);
	InitGpioToOutput(LCD_DATA2_INIT);
	InitGpioToOutput(LCD_DATA3_INIT);
	InitGpioToOutput(LCD_DATA4_INIT);
	InitGpioToOutput(LCD_DATA5_INIT);
	InitGpioToOutput(LCD_DATA6_INIT);
	InitGpioToOutput(LCD_DATA7_INIT);
	
}
/***************************************************************
  *	@brief	����������Ϊд��ģʽ�������
  *	@param	void
  *	@retval	void
****************************************************************/
void setDataWrite(){
	InitGpioToOutput(LCD_DATA0_INIT);
	InitGpioToOutput(LCD_DATA1_INIT);
	InitGpioToOutput(LCD_DATA2_INIT);
	InitGpioToOutput(LCD_DATA3_INIT);
	InitGpioToOutput(LCD_DATA4_INIT);
	InitGpioToOutput(LCD_DATA5_INIT);
	InitGpioToOutput(LCD_DATA6_INIT);
	InitGpioToOutput(LCD_DATA7_INIT);
}
/***************************************************************
  *	@brief	����������Ϊ��ȡģʽ��������
  *	@param	void
  *	@retval	void
****************************************************************/
void setDataRead(){
	InitGpioToInput(LCD_DATA0_INIT);
	InitGpioToInput(LCD_DATA1_INIT);
	InitGpioToInput(LCD_DATA2_INIT);
	InitGpioToInput(LCD_DATA3_INIT);
	InitGpioToInput(LCD_DATA4_INIT);
	InitGpioToInput(LCD_DATA5_INIT);
	InitGpioToInput(LCD_DATA6_INIT);
	InitGpioToInput(LCD_DATA7_INIT);
}
/***************************************************************
  *	@brief	���lcdоƬ�Ƿ��ڷ�æ״̬
  *	@param	
  *	@retval	uint8_t
										0:����æ
										CXS_BUSY:��æ
****************************************************************/
uint8_t is_lcd_busy(uint8_t controller_csX){
	uint16_t data;
	if(!is_csX(controller_csX)){
		return 0;
	}
	enableWrite();
	setDataRead();
	enableCmd();
	enableCSx(controller_csX);
	enableRead();
	delay_us(1);
	disableLCD();
	delay_us(1);
	enableLCD();
	data=GPIO_ReadInputData(LCD_GPIOE);
	delay_us(1);
	enableWrite();
	disableCSx(controller_csX);
	if((data>>8)&CSX_BUSY){
		return CSX_BUSY;//busy
	}else{
		return 0;//no busy
	}
}
/***************************************************************
  *	@brief	д��ָ���������
  *	@param	cmd ��д�������
	*	@param 	controller_csX ��д���оƬ��� 1~2
  *	@retval	void
****************************************************************/
void lcd_write_cmd(uint8_t cmd,uint8_t controller_csX){
	uint16_t data;
	if(!is_csX(controller_csX)){
		return;
	}
	enableRead();//��
	setDataWrite();
	enableCmd();//ָ��
	enableCSx(controller_csX);
	enableWrite();//д
	
	data=GPIO_ReadInputData(LCD_GPIOE);
	data&=0x00ff;
	data|=(cmd<<8);
	GPIO_Write(LCD_GPIOE,data);
	
	disableLCD();
	delay_us(1);
	enableLCD();
	delay_us(1);
	
	enableRead();//��
	disableCSx(controller_csX);
}

/***************************************************************
  *	@brief	д������������
  *	@param	wdata ��д�������
	* @param	controller_csX ��д���оƬ���
  *	@retval	void
****************************************************************/
void lcd_write_data(uint8_t wdata,uint8_t controller_csX){
	uint16_t data;
	if(!is_csX(controller_csX)){
		return;
	}
	enableRead();//��
	setDataWrite();//
	enableData();//����
	enableCSx(controller_csX);
	enableWrite();//д
	//���ݴ���
	data=GPIO_ReadInputData(LCD_GPIOE);
	data&=0X00FF;
	data|=(wdata<<8);
	GPIO_Write(LCD_GPIOE,data);
	
	disableLCD();
	delay_us(1);
	enableLCD();
	delay_us(1);
	enableRead();//��
	disableCSx(controller_csX);
}

/***************************************************************
  *	@brief	����Ҫ��ʼд�����ݵ�λ�ã�ҳ����
  *	@param	page ҳ�� 0~7
	*	@param	column ���� 0~64
  *	@retval	void
****************************************************************/
void lcd_set_dsp_position(uint8_t page,uint8_t column){
	uint8_t controller_csX;
	if(column>=LCD_COLUMN_PIXEL_SUM/2){
		controller_csX=CONCTROLLER_CS2;
	}else{
		controller_csX=CONCTROLLER_CS1;
	}
	
	lcd_write_cmd(0xc0,controller_csX);//����
	lcd_write_cmd(0x40+(column&0x3f),controller_csX);//������
	lcd_write_cmd(0xb8+(page&0x07),controller_csX);//����ҳ
	
}
/***************************************************************
  *	@brief	�����Ļ����
  *	@param	void
  *	@retval	void
****************************************************************/
void lcd_clr(void){
	uint8_t i, x=0;
    
	for(x=0; x<8; x++){
		lcd_set_dsp_position(x, 0);                       //ѡ������
		for(i=0; i<64; i++)
		{
			lcd_write_data(0, CONCTROLLER_CS1);
		}
        
		lcd_set_dsp_position(x, 64);                      //ѡ������
		for(i=0; i<64; i++)
		{
			lcd_write_data(0, CONCTROLLER_CS2);
		}
	}
}
/***************************************************************
  *	@brief	
  *	@param	
  *	@retval	
****************************************************************/
void lcd_clr_row(uint8_t row)
{
    uint8_t i;
    
    lcd_set_dsp_position(row*(CHAR_ROW_PIXEL/LCD_PAGE_ROW_NUM), 0);                       //ѡ������
    for(i = 0; i < LCD_COLUMN_PIXEL_SUM/2; i++){
        lcd_write_data(0, CONCTROLLER_CS1);
    }
		lcd_set_dsp_position(row*(CHAR_ROW_PIXEL/LCD_PAGE_ROW_NUM) + 1, 0);
		for(i = 0; i < LCD_COLUMN_PIXEL_SUM/2; i++){
        lcd_write_data(0, CONCTROLLER_CS1);
    }
    
    lcd_set_dsp_position(row*(CHAR_ROW_PIXEL/LCD_PAGE_ROW_NUM), 64);                      //ѡ������
    for(i = 0; i < LCD_COLUMN_PIXEL_SUM/2; i++){
         lcd_write_data(0, CONCTROLLER_CS2);
    }
	lcd_set_dsp_position(row*(CHAR_ROW_PIXEL/LCD_PAGE_ROW_NUM) + 1, 64);
	for(i = 0; i < LCD_COLUMN_PIXEL_SUM/2; i++){
         lcd_write_data(0, CONCTROLLER_CS2);
    }
}
/***************************************************************
  *	@brief	lcd��ʼ������ʼ�����io�������������
  *	@param	void
  *	@retval	void
****************************************************************/
void lcd_init (void)
{
	lcd_gpio_init();

  //�򿪱���
	enableBL();
	//reset
	GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);
	delay_ms(100);
	GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);
	delay_ms(1);
	
	enableLCD();
	delay_us(1);
	
	lcd_write_cmd(0xC0, CONCTROLLER_CS1);       //������ʾ��ʼ��
	lcd_write_cmd(0x3F, CONCTROLLER_CS1);       //��ʾ��
	lcd_write_cmd(0xC0, CONCTROLLER_CS2);				//������ʾ��ʼ��
	lcd_write_cmd(0x3F, CONCTROLLER_CS2);				//��ʾ��

	lcd_clr();//����
}

/***************************************************************
  *	@brief	ʹ��Ļ��ʾһ��ͼƬ��128*64�����������ң������¸�λ
  *	@param	img ͼƬ���ݵ������ַ
  *	@retval	void
****************************************************************/
void lcd_disp_full_img(uint8_t *img)			   //��ʾͼƬ
{
	uint8_t i=0,j=0;
	for(i=0;i<8;i++)
	{
		lcd_set_dsp_position(i, 0);													
		for(j=0;j<128;j++)
		{
			if( j == 64 )
			{
					lcd_set_dsp_position(i, j);                   //ѡ������
			}
			if(j < 64)
				lcd_write_data(*img, CONCTROLLER_CS1);
			else
				lcd_write_data(*img, CONCTROLLER_CS2);
			img++;
		}
	}
}

/***************************************************************
  *	@brief	��ʾ2518��logoͼƬ
  *	@param	void
  *	@retval	void
****************************************************************/
void show_logo(void)
{
	lcd_disp_full_img(logo2518);
	delay_ms(1000);
	lcd_clr();
	lcd_disp_full_img(logo2518_2);
	lcd_writeLenthChar(1,1,"Welcome to 2518.",15);
}
/***************************************************************
  *	@brief	������x,y��д��һ���ַ�
  *	@param	x ������ 0~7
	*	@param	y ������ 0~127
	*	@param	c	�ַ�
  *	@retval	void
****************************************************************/
void lcd_writeOneChar(uint8_t x,uint8_t y, uint8_t c){
	uint8_t i,line,temp;
	c -= 32;
	for(i=0;i<2;i++){
		lcd_set_dsp_position(x+i,y);
		temp=i<<3;
		for(line=0; line<8; line++){
			if((y+line)>=64){
				lcd_write_data(char_code_8x16[c][line+temp],CONCTROLLER_CS2);
			}else{
				lcd_write_data(char_code_8x16[c][line+temp],CONCTROLLER_CS1);//�˴���temp���ڴ��i*8�Ľ����
			}
		}
	}
}
/***************************************************************
  *	@brief	�ڵ�x�У��͵�y����ʾlenth���ַ������Զ�����
  *	@param	x ���� 0~3     	��64/16=4��
	*	@param	y	���� 0~15		 	��128/8=16��
	*	@param	lenth �ַ���
  *	@retval	void
****************************************************************/
void lcd_writeLenthChar(uint8_t x,uint8_t y, char* str,uint8_t lenth){
	uint8_t i=0;
	x*=2;
	y*=8;
	while(lenth--){
		lcd_writeOneChar(x,y,str[i++]);
		y+=8;
		if(y%128==0){
			x+=2;
			y=0;
		}
	}
}

