

#include "extend_adc.h"
#include "iic.h"
#include "stdio.h"

/*******************************************************************
*��������uint8_t PCF8591_Write(uint8_t DAC_Date)
*���ܣ�PCF8591��DAC_Date�Ĵ�С������ģת������������Ӧ��С��ѹ��
*				DAC������;
*���룺uint8_t DAC_Date��������
*��������� 0�ɹ� ���� 1ʧ��
*����˵������
*******************************************************************/
uint8_t PCF8591_Write(uint8_t DAC_Date)
{
	uint8_t ack;
	uint8_t data[2]={Model_PCF8591,DAC_Date};
	
	return IIC_Write_Bytes(Address_PCF8591,data,2);
	
}

/**********************************************************
*��������uint8_t PCF8591_Read(uint8_t *a, uint8_t ch, uint8_t n)
*���ܣ���PCF8591�ж�����chͨ����ʼ��n��ADCͨ��������
*���룺 uint8_t *Data, ADC���ݻ�������
*								Data[0]���ͨ��ch��ADC����,
*								Data[1]���ͨ��ch+1��ADC����,
*								......,
*								Data[n-1]���ͨ��ch+n-1��ADC���ݣ�
*				uint8_t ch,��ʼͨ����ţ�
*				uint8_t n,ͨ��������
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
**********************************************************/
uint8_t PCF8591_Read(uint8_t *a, uint8_t ch, uint8_t n)
{    
	uint8_t ack,i;
	uint8_t ad[4],cmd[1]={Model_PCF8591auto};
	ch &= 3;
	if(n > 4)
	{
		n = 4;
	}
	ack=IIC_Write_Bytes(Address_PCF8591,cmd,1);
	
	if(ack == 1)                   //д��ʧ�� �ӻ��豸û����Ӧ
	{
		return (1);
	}
	
	ack = IIC_ReadBytes_Ack(Address_PCF8591,ad,4);
	
	
	if(ack == 1)                   
	{
		return (1);
	}
	return 0;
}


/**************************************************************
*��������uint8_t PCF8591_ReadChannel(uint8_t *a, uint8_t ch)
*���ܣ���PCF8591�ж���chͨ����ADC����
*���룺 uint8_t *Data, chͨ����ADC���ݻ�����
*				uint8_t ch��ͨ�����
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
***************************************************************/
uint8_t PCF8591_ReadChannel(uint8_t *a, uint8_t ch)
{    
	uint8_t ack,cmd[1]={Model_PCF8591S | ch},data[4];
	ch &= 3;
	ack = IIC_Write_Bytes(Address_PCF8591,cmd,1);
	if (ack==1){
		return 1;
	}
	ack = IIC_ReadBytes_Ack(Address_PCF8591,data,4);
	if (ack==1){
		return 1;
	}
	
	*a=data[3];

	return (0);	
}

/************************************************************
*��������uint8_t  PCF8591_Init(uint8_t ch)
*���ܣ���ʼ��PCF8591��IIC���ߣ���ָ��PCF8591��ͨ���͹���ģʽ
*���룺 uint8_t *Data, chͨ����ADC���ݻ�����
*				uint8_t ch��ͨ�����
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
************************************************************/
uint8_t PCF8591_Init(uint8_t ch)
{
	uint8_t ack,cmd[1]={Model_PCF8591S | ch};
	ch &= 3;
	IIC_Init();
	ack=IIC_Write_Bytes(Address_PCF8591,cmd,1);
	
	if(ack==1){
		return 0;
	}
	
	ack=IIC_ReadBytes_Ack(Address_PCF8591,cmd,1);
	
	
	return ack;
}

/************************************************************
*��������uint8_t  PCF8591_ReadChNd(uint8_t *a)
*���ܣ���ָ��ͨ������ADC���ݣ���PCF8591_Init���ʹ��
*���룺 uint8_t *Data ��ADC���ݻ�����
*��������� HAL_I2C_OK ��return HAL_I2C_FAIL;
*����˵������
************************************************************/
uint8_t PCF8591_ReadChNd(uint8_t *a)
{
	uint8_t ack;
	ack =IIC_ReadBytes_Ack(Address_PCF8591,a,1);
	printf("adc��ֵ��%d\r\n",*a);
	return ack;	
}
