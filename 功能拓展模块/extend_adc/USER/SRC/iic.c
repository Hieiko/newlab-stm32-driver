
/***************************************************************
  *	Name		:	IIC.C
  *	Data		:	2019.7.30
  *	Author	:	NIEAOWEI
  *	Note		:	
							1.��дIICЭ��ʱ�мǲ���ʹ�ô��ڵ��ԣ�
							���ڷ���ռ��ʱ������IICʱ��Ӱ��
							ʱ��Ƶ�ʡ�
							
****************************************************************/
#include "iic.h"
#include "sys.h"
#include "delay.h"

//IIC�����������
#define IIC_SCL_PORT	GPIOB
#define IIC_SCL_PIN		GPIO_Pin_6
#define IIC_SCL_CLK		RCC_APB2Periph_GPIOB
#define SCL(n)				GPIO_WriteBit(IIC_SCL_PORT,IIC_SCL_PIN,(BitAction)n)

#define IIC_SDA_PORT	GPIOB
#define IIC_SDA_PIN		GPIO_Pin_7
#define IIC_SDA_CLK		RCC_APB2Periph_GPIOB
#define SDAW(n)				GPIO_WriteBit(IIC_SDA_PORT,IIC_SDA_PIN,(BitAction)n)
#define SDAR()				GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN)

#define IIC_RATE			2//us

/***************************************************************
  *	@brief	IIC���ų�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void IIC_Init(void){
	
	GPIO_InitTypeDef iic;
	RCC_APB2PeriphClockCmd(IIC_SCL_CLK|IIC_SDA_CLK,ENABLE);
	
	iic.GPIO_Mode=GPIO_Mode_Out_PP;
	iic.GPIO_Speed=GPIO_Speed_50MHz;
	iic.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&iic);
	iic.GPIO_Pin=IIC_SCL_PIN;
	GPIO_Init(IIC_SCL_PORT,&iic);
	SCL(1);
	SDAW(1);
}
/***************************************************************
  *	@brief	����SDA��Ϊ���ģʽ
  *	@param	void.
  *	@retval	void.
****************************************************************/
void enableWrite(void){
	GPIO_InitTypeDef iic;
	iic.GPIO_Mode=GPIO_Mode_Out_PP;
	iic.GPIO_Speed=GPIO_Speed_50MHz;
	iic.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&iic);
}
/***************************************************************
  *	@brief	����SDA��Ϊ�������룬����iicЭ�����ԣ�ֻ��0������
  *	@param	void.
  *	@retval	void.
****************************************************************/
void enableRead(void){
	GPIO_InitTypeDef iic;
	iic.GPIO_Mode=GPIO_Mode_IPU;
	iic.GPIO_Speed=GPIO_Speed_50MHz;
	iic.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&iic);
}
/***************************************************************
  *	@brief	����IIC��ʼ�źš�
														��ʼʱ��
																		SDA _
																				 \__
																		SCL ___
																					 \_
  *	@param	void
  *	@retval	void
****************************************************************/
void IIC_Start(void){
	enableWrite();
	SDAW(1);
	SCL(1);
	delay_us(IIC_RATE);
	SDAW(0);
	delay_us(IIC_RATE);
	SCL(0);
}
/***************************************************************
  *	@brief	����IICֹͣ�ź�
														ֹͣʱ��
																		sda 	 _
																				 _/
																		scl 
																				 ___
  *	@param	
  *	@retval	
****************************************************************/
void IIC_Stop(void){
	enableWrite();
	SCL(0);
	SDAW(0);
	delay_us(IIC_RATE);
	SCL(1);
	SDAW(1);
	delay_us(IIC_RATE);

}
/***************************************************************
  *	@brief	����Ӧ���ź�
												 Ӧ��ʱ��
																sda 
																		x____
																scl		 _
																		__/	\_
  *	@param	
  *	@retval	
****************************************************************/
void IIC_Ack(void){
	SCL(0);
	enableWrite();
	SDAW(0);
	delay_us(IIC_RATE);
	SCL(1);
	delay_us(IIC_RATE);
	SCL(0);
}
/***************************************************************
  *	@brief	������Ӧ���ź�
													��Ӧ��ʱ��
																	sda  _____
																			x
																	scl		 _
																			__/	\_
																		
  *	@param	
  *	@retval	
****************************************************************/
void IIC_NAck(void){
	SCL(0);
	enableWrite();
	SDAW(1);
	delay_us(IIC_RATE);
	SCL(1);
	delay_us(IIC_RATE);
	SCL(0);
}
/***************************************************************
  *	@brief	�ȴ��ӻ��ĵ͵�ƽӦ���ź�
  *	@param	void
  *	@retval	uint8_t.
						1:�ȴ�Ӧ��ʱ
						0:�ȴ�Ӧ��ɹ�
****************************************************************/
uint8_t IIC_Wait_Ack(void){
	u8 ucErrTime=0;
	SDAW(1);
	enableRead();      //SDA����Ϊ����
	delay_us(IIC_RATE);	   
	SCL(1);
	delay_us(IIC_RATE);	 
	while(SDAR())
	{
		
		ucErrTime++;
		if(ucErrTime>IIC_RATE)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL(0);//   
	return 0;  
} 
/***************************************************************
  *	@brief	����һ���ֽڵ����ݲ��ȴ�Ӧ��
  *	@param	byte��һ���ֽڵ�����
  *	@retval	uint8_t.
										1:����ʧ��
										0:���ͳɹ�
****************************************************************/
uint8_t IIC_Write_Byte(uint8_t byte){
	uint8_t count=8;
	enableWrite();
	while(count--){
		if((byte&0x80)!=0){
			SDAW(1);
		}else{
			SDAW(0);
		}
		byte<<=1;
		delay_us(IIC_RATE);
		SCL(1);
		delay_us(IIC_RATE);
		SCL(0);
	}	
	return IIC_Wait_Ack();
}
/***************************************************************
	*	@brief	ͨ��IIC���Ͷ���ֽڵ�����
  *	@param	bytes �����ֽ�����
	* @param	lenth	�����ֽ�����ĳ���
	*	@param	addr	�ӻ��豸��ַ
  *	@retval	uint8_t
										0:���ͳɹ�
										1:����ʧ��
****************************************************************/
uint8_t IIC_Write_Bytes(uint8_t addr,uint8_t* bytes,uint8_t lenth){
	uint8_t index=0;
	IIC_Start();
	if(IIC_Write_Byte(addr<<1)!=0){
		return 1;
	}
	while(lenth--){
		if(IIC_Write_Byte(bytes[index++])==0){
			continue;
		}else{
			return 1;
		}
	}
	IIC_Stop();
	return 0;
}

/***************************************************************
  *	@brief	��ȡһ���ֽڵ����ݣ�����Ӧ���Ӧ��
	*	@param	ack �Ƿ����Ӧ��1����Ӧ��0������Ӧ��
	*	@param	*recv �������մ洢���һ���ֽڵ�����
  *	@retval	uint8_t 
										0:�������
****************************************************************/
uint8_t IIC_Read_Byte(uint8_t ack,uint8_t *recv){
	uint8_t i=8;
	SDAW(1);
	enableRead();
	while(i--){
		SCL(0);
		delay_us(IIC_RATE);
		SCL(1);
		*recv<<=1;
		if(SDAR()){
			(*recv)++;
		}
		delay_us(IIC_RATE);
	}
	if(ack){
		IIC_Ack();
	}else{
		IIC_NAck();
	}
	return 0;
}
/***************************************************************
  *	@brief	��ȡlenth�����ֽڵ����ݴ����rec����
  *	@param	addr �ӻ��豸��ַ
	* @param	*rec �������������ַ
	*	@param	lenth	��Ҫ��ȡ�����ݳ���
  *	@retval	uint8_t
										1:ʧ��
										0:�ɹ�
****************************************************************/
uint8_t IIC_ReadBytes_Ack(uint8_t addr,uint8_t *rec,uint8_t lenth)
{
	uint8_t index = 0;
	IIC_Start();
	
	if(IIC_Write_Byte((addr<<1)|0x01)==1){
		return 1;
	}
	while(lenth--){
		IIC_Read_Byte(1,&rec[index++]);
	}
	IIC_Stop();
	return 0;
}
