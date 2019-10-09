/***************************************************************
  *	Name		:	TEMP_LIGHT.C
  *	Data		:	2019.10.5
  *	Author	:	NIEAOWEI
  *	Note		:	�¶ȹ��ղɼ�
****************************************************************/

#include "temp_light.h"

#include "sys.h"

#define TEMP_PORT	GPIOC
#define TEMP_PIN	GPIO_Pin_0
#define TEMP_CLK	RCC_APB2Periph_GPIOC

#define	LIGHT_PORT	GPIOC
#define	LIGHT_PIN		GPIO_Pin_1
#define LIGHT_CLK		RCC_APB2Periph_GPIOC

#define TEMP_LIGHT_ADC			ADC1
#define TEMP_LIGHT_ADC_CLK 	RCC_APB2Periph_ADC1

#define	LIGHT_CH	ADC_Channel_11
#define TEMP_CH		ADC_Channel_10

uint8_t temp_value;


/*
	ntc��������ο�ֵ
*/
#define AMPLIFY_VALUE ((uint16_t)(1000))
const uint32_t RES_TEMP_Table[] =//��Ӧ�¶ȵĵ���ֵ����λ����
{
	(uint32_t)(98.26*AMPLIFY_VALUE),//���϶ȣ�-20
	(uint32_t)(92.74*AMPLIFY_VALUE),//���϶ȣ�-19
	(uint32_t)(87.55*AMPLIFY_VALUE),//���϶ȣ�-18
	(uint32_t)(82.69*AMPLIFY_VALUE),//���϶ȣ�-17
	(uint32_t)(78.12*AMPLIFY_VALUE),//���϶ȣ�-16
	(uint32_t)(73.83*AMPLIFY_VALUE),//���϶ȣ�-15
	(uint32_t)(69.80*AMPLIFY_VALUE),//���϶ȣ�-14
	(uint32_t)(66.01*AMPLIFY_VALUE),//���϶ȣ�-13
	(uint32_t)(62.45*AMPLIFY_VALUE),//���϶ȣ�-12
	(uint32_t)(59.10*AMPLIFY_VALUE),//���϶ȣ�-11
	(uint32_t)(55.95*AMPLIFY_VALUE),//���϶ȣ�-10
	(uint32_t)(52.98*AMPLIFY_VALUE),//���϶ȣ�-9
	(uint32_t)(50.19*AMPLIFY_VALUE),//���϶ȣ�-8
	(uint32_t)(47.57*AMPLIFY_VALUE),//���϶ȣ�-7
	(uint32_t)(45.09*AMPLIFY_VALUE),//���϶ȣ�-6
	(uint32_t)(42.76*AMPLIFY_VALUE),//���϶ȣ�-5
	(uint32_t)(40.56*AMPLIFY_VALUE),//���϶ȣ�-4
	(uint32_t)(38.49*AMPLIFY_VALUE),//���϶ȣ�-3
	(uint32_t)(36.53*AMPLIFY_VALUE),//���϶ȣ�-2
	(uint32_t)(34.68*AMPLIFY_VALUE),//���϶ȣ�-1
	(uint32_t)(32.94*AMPLIFY_VALUE),//���϶ȣ�0
	(uint32_t)(31.30*AMPLIFY_VALUE),//���϶ȣ�1
	(uint32_t)(29.74*AMPLIFY_VALUE),//���϶ȣ�2
	(uint32_t)(28.28*AMPLIFY_VALUE),//���϶ȣ�3
	(uint32_t)(26.89*AMPLIFY_VALUE),//���϶ȣ�4
	(uint32_t)(25.58*AMPLIFY_VALUE),//���϶ȣ�5
	(uint32_t)(24.34*AMPLIFY_VALUE),//���϶ȣ�6
	(uint32_t)(23.17*AMPLIFY_VALUE),//���϶ȣ�7
	(uint32_t)(22.06*AMPLIFY_VALUE),//���϶ȣ�8
	(uint32_t)(21.01*AMPLIFY_VALUE),//���϶ȣ�9
	(uint32_t)(20.01*AMPLIFY_VALUE),//���϶ȣ�10
	(uint32_t)(19.07*AMPLIFY_VALUE),//���϶ȣ�11
	(uint32_t)(18.18*AMPLIFY_VALUE),//���϶ȣ�12
	(uint32_t)(17.33*AMPLIFY_VALUE),//���϶ȣ�13
	(uint32_t)(16.53*AMPLIFY_VALUE),//���϶ȣ�14
	(uint32_t)(15.77*AMPLIFY_VALUE),//���϶ȣ�15
	(uint32_t)(15.05*AMPLIFY_VALUE),//���϶ȣ�16
	(uint32_t)(14.37*AMPLIFY_VALUE),//���϶ȣ�17
	(uint32_t)(13.72*AMPLIFY_VALUE),//���϶ȣ�18
	(uint32_t)(13.10*AMPLIFY_VALUE),//���϶ȣ�19
	(uint32_t)(12.52*AMPLIFY_VALUE),//���϶ȣ�20
	(uint32_t)(11.96*AMPLIFY_VALUE),//���϶ȣ�21
	(uint32_t)(11.43*AMPLIFY_VALUE),//���϶ȣ�22
	(uint32_t)(10.93*AMPLIFY_VALUE),//���϶ȣ�23
	(uint32_t)(10.45*AMPLIFY_VALUE),//���϶ȣ�24
	(uint32_t)(10.00*AMPLIFY_VALUE),//���϶ȣ�25
	(uint32_t)(9.569*AMPLIFY_VALUE),//���϶ȣ�26
	(uint32_t)(9.158*AMPLIFY_VALUE),//���϶ȣ�27
	(uint32_t)(8.768*AMPLIFY_VALUE),//���϶ȣ�28
	(uint32_t)(8.396*AMPLIFY_VALUE),//���϶ȣ�29
	(uint32_t)(8.042*AMPLIFY_VALUE),//���϶ȣ�30
	(uint32_t)(7.705*AMPLIFY_VALUE),//���϶ȣ�31
	(uint32_t)(7.384*AMPLIFY_VALUE),//���϶ȣ�32
	(uint32_t)(7.078*AMPLIFY_VALUE),//���϶ȣ�33
	(uint32_t)(6.786*AMPLIFY_VALUE),//���϶ȣ�34
	(uint32_t)(6.508*AMPLIFY_VALUE),//���϶ȣ�35
	(uint32_t)(6.243*AMPLIFY_VALUE),//���϶ȣ�36
	(uint32_t)(5.990*AMPLIFY_VALUE),//���϶ȣ�37
	(uint32_t)(5.749*AMPLIFY_VALUE),//���϶ȣ�38
	(uint32_t)(5.519*AMPLIFY_VALUE),//���϶ȣ�39
	(uint32_t)(5.299*AMPLIFY_VALUE),//���϶ȣ�40
	(uint32_t)(5.089*AMPLIFY_VALUE),//���϶ȣ�41
	(uint32_t)(4.889*AMPLIFY_VALUE),//���϶ȣ�42
	(uint32_t)(4.697*AMPLIFY_VALUE),//���϶ȣ�43
	(uint32_t)(4.514*AMPLIFY_VALUE),//���϶ȣ�44
	(uint32_t)(4.339*AMPLIFY_VALUE),//���϶ȣ�45
	(uint32_t)(4.172*AMPLIFY_VALUE),//���϶ȣ�46
	(uint32_t)(4.012*AMPLIFY_VALUE),//���϶ȣ�47
	(uint32_t)(3.860*AMPLIFY_VALUE),//���϶ȣ�48
	(uint32_t)(3.713*AMPLIFY_VALUE),//���϶ȣ�49
	(uint32_t)(3.574*AMPLIFY_VALUE),//���϶ȣ�50
	(uint32_t)(3.440*AMPLIFY_VALUE),//���϶ȣ�51
	(uint32_t)(3.312*AMPLIFY_VALUE),//���϶ȣ�52
	(uint32_t)(3.189*AMPLIFY_VALUE),//���϶ȣ�53
	(uint32_t)(3.071*AMPLIFY_VALUE),//���϶ȣ�54
	(uint32_t)(2.959*AMPLIFY_VALUE),//���϶ȣ�55
	(uint32_t)(2.851*AMPLIFY_VALUE),//���϶ȣ�56
	(uint32_t)(2.748*AMPLIFY_VALUE),//���϶ȣ�57
	(uint32_t)(2.649*AMPLIFY_VALUE),//���϶ȣ�58
	(uint32_t)(2.554*AMPLIFY_VALUE),//���϶ȣ�59
	(uint32_t)(2.463*AMPLIFY_VALUE),//���϶ȣ�60
	(uint32_t)(2.375*AMPLIFY_VALUE),//���϶ȣ�61
	(uint32_t)(2.292*AMPLIFY_VALUE),//���϶ȣ�62
	(uint32_t)(2.211*AMPLIFY_VALUE),//���϶ȣ�63
	(uint32_t)(2.134*AMPLIFY_VALUE),//���϶ȣ�64
	(uint32_t)(2.060*AMPLIFY_VALUE),//���϶ȣ�65
	(uint32_t)(1.989*AMPLIFY_VALUE),//���϶ȣ�66
	(uint32_t)(1.921*AMPLIFY_VALUE),//���϶ȣ�67
	(uint32_t)(1.855*AMPLIFY_VALUE),//���϶ȣ�68
	(uint32_t)(1.792*AMPLIFY_VALUE),//���϶ȣ�69
	(uint32_t)(1.731*AMPLIFY_VALUE),//���϶ȣ�70
	(uint32_t)(1.673*AMPLIFY_VALUE),//���϶ȣ�71
	(uint32_t)(1.617*AMPLIFY_VALUE),//���϶ȣ�72
	(uint32_t)(1.563*AMPLIFY_VALUE),//���϶ȣ�73
	(uint32_t)(1.512*AMPLIFY_VALUE),//���϶ȣ�74
	(uint32_t)(1.462*AMPLIFY_VALUE),//���϶ȣ�75
	(uint32_t)(1.414*AMPLIFY_VALUE),//���϶ȣ�76
	(uint32_t)(1.368*AMPLIFY_VALUE),//���϶ȣ�77
	(uint32_t)(1.324*AMPLIFY_VALUE),//���϶ȣ�78
	(uint32_t)(1.281*AMPLIFY_VALUE),//���϶ȣ�79
	(uint32_t)(1.240*AMPLIFY_VALUE),//���϶ȣ�80
	(uint32_t)(1.201*AMPLIFY_VALUE),//���϶ȣ�81
	(uint32_t)(1.163*AMPLIFY_VALUE),//���϶ȣ�82
	(uint32_t)(1.126*AMPLIFY_VALUE),//���϶ȣ�83
	(uint32_t)(1.091*AMPLIFY_VALUE),//���϶ȣ�84
	(uint32_t)(1.056*AMPLIFY_VALUE),//���϶ȣ�85
	(uint32_t)(1.024*AMPLIFY_VALUE),//���϶ȣ�86
	(uint32_t)(0.992*AMPLIFY_VALUE),//���϶ȣ�87
	(uint32_t)(0.9615*AMPLIFY_VALUE),//���϶ȣ�88
	(uint32_t)(0.9321*AMPLIFY_VALUE),//���϶ȣ�89
	(uint32_t)(0.9038*AMPLIFY_VALUE),//���϶ȣ�90
	(uint32_t)(0.8764*AMPLIFY_VALUE),//���϶ȣ�91
	(uint32_t)(0.8500*AMPLIFY_VALUE),//���϶ȣ�92
	(uint32_t)(0.8245*AMPLIFY_VALUE),//���϶ȣ�93
	(uint32_t)(0.8000*AMPLIFY_VALUE),//���϶ȣ�94
	(uint32_t)(0.7762*AMPLIFY_VALUE),//���϶ȣ�95
	(uint32_t)(0.7533*AMPLIFY_VALUE),//���϶ȣ�96
	(uint32_t)(0.7312*AMPLIFY_VALUE),//���϶ȣ�97
	(uint32_t)(0.7099*AMPLIFY_VALUE),//���϶ȣ�98
	(uint32_t)(0.6892*AMPLIFY_VALUE),//���϶ȣ�99
	(uint32_t)(0.6693*AMPLIFY_VALUE),//���϶ȣ�100
	(uint32_t)(0.6500*AMPLIFY_VALUE),//���϶ȣ�101
	(uint32_t)(0.6314*AMPLIFY_VALUE),//���϶ȣ�102
	(uint32_t)(0.6135*AMPLIFY_VALUE),//���϶ȣ�103
	(uint32_t)(0.5961*AMPLIFY_VALUE),//���϶ȣ�104
	(uint32_t)(0.5793*AMPLIFY_VALUE),//���϶ȣ�105
	(uint32_t)(0.5630*AMPLIFY_VALUE),//���϶ȣ�106
	(uint32_t)(0.5473*AMPLIFY_VALUE),//���϶ȣ�107
	(uint32_t)(0.5321*AMPLIFY_VALUE),//���϶ȣ�108
	(uint32_t)(0.5174*AMPLIFY_VALUE),//���϶ȣ�109
	(uint32_t)(0.5031*AMPLIFY_VALUE),//���϶ȣ�110
	(uint32_t)(0.4894*AMPLIFY_VALUE),//���϶ȣ�111
	(uint32_t)(0.4760*AMPLIFY_VALUE),//���϶ȣ�112
	(uint32_t)(0.4631*AMPLIFY_VALUE),//���϶ȣ�113
	(uint32_t)(0.4506*AMPLIFY_VALUE),//���϶ȣ�114
	(uint32_t)(0.4386*AMPLIFY_VALUE),//���϶ȣ�115
	(uint32_t)(0.4268*AMPLIFY_VALUE),//���϶ȣ�116
	(uint32_t)(0.4155*AMPLIFY_VALUE),//���϶ȣ�117
	(uint32_t)(0.4045*AMPLIFY_VALUE),//���϶ȣ�118
	(uint32_t)(0.3939*AMPLIFY_VALUE),//���϶ȣ�119
	(uint32_t)(0.3835*AMPLIFY_VALUE)//���϶ȣ�120
};

#define Vref 3.3     //�ο���ѹ����λ��V
#define Res  10000 //�ο����裬��λ����

/***************************************************************
  *	@brief	���ų�ʼ��
  *	@param	
  *	@retval	void
****************************************************************/
void GPIO_Configuration(){
	GPIO_InitTypeDef temp;
	RCC_APB2PeriphClockCmd(TEMP_CLK|LIGHT_CLK,ENABLE);
	temp.GPIO_Mode=GPIO_Mode_AIN;
	temp.GPIO_Pin=TEMP_PIN;
	GPIO_Init(TEMP_PORT,&temp);
	temp.GPIO_Pin=LIGHT_PIN;
	GPIO_Init(LIGHT_PORT,&temp);
}
/***************************************************************
  *	@brief	adc��ʼ��
  *	@param	
  *	@retval	void
****************************************************************/
void TEMP_LIGHT_Init(){
	ADC_InitTypeDef temp;
	
	GPIO_Configuration();
	RCC_APB2PeriphClockCmd(TEMP_LIGHT_ADC_CLK,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	temp.ADC_ContinuousConvMode=DISABLE;
	temp.ADC_DataAlign=ADC_DataAlign_Right;
	temp.ADC_Mode=ADC_Mode_Independent;
	temp.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	temp.ADC_NbrOfChannel=2;
	temp.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&temp);
	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)){};
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)){};
		
}
/***************************************************************
  *	@brief	���ת����ȡһ�βɼ�ֵ
  *	@param	uint8 ch ͨ���� ADC_Channel_x x��0~15
  *	@retval	uint16 ����adc�ɼ���������ֵ
****************************************************************/
uint16_t GetADC(uint8_t ch){
	ADC_RegularChannelConfig(TEMP_LIGHT_ADC,ch,1,ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(TEMP_LIGHT_ADC,ENABLE);
	while(!ADC_GetFlagStatus(TEMP_LIGHT_ADC,ADC_FLAG_EOC)){};
	return ADC_GetConversionValue(TEMP_LIGHT_ADC);
}
/***************************************************************
  *	@brief	��ȡtime�ε�ƽ��ֵ
  *	@param	uint8 ch ͨ���� ADC_Channel_x x��0~15
	*	@param	uint8 time ����
  *	@retval	uint16 ����adc�ɼ���������ƽ��ֵ
****************************************************************/
uint16_t GetADC_Time(uint8_t ch,uint8_t time){
	uint32_t sum=0;
	uint8_t count=time;
	while(count>0){
		sum+=GetADC(ch);
		count--;
	}
	return sum/time;
}

/********************************************************
*�������ƣ�uint32_t calculateResValue(uint16_t ADC_Value)
*���ܣ�����������������ADCֵ�������������ֵ����λ������
*���룺uint16_t ADC_Value��������������ADCֵ��
*�������������ֵ����λ������
*����˵����ADC�ֱ��ʣ�12λ�ֱ��ʣ�2^8=256
**********************************************************/
uint32_t calculateResValue(uint16_t ADC_Value)
{
	float Vrt=0.0;
	uint32_t Rrt=0;
	
	Vrt=(Vref/(4096))*ADC_Value;//�����������˵�ѹ
	Rrt=Vrt/((Vref-Vrt)/Res);//�������裬��ֵ
	return (uint32_t)Rrt;
}

/********************************************************
*�������ƣ�int8_t calculateTemperature(uint32_t Res)
*���ܣ������������裬��λ����������¶�ֵ����λ����
*���룺uint32_t Res����������ֵ����λ����
*���������¶�ֵ����λ����
*����˵��������ֵ��Χ98.26K��~0.3835K�����¶ȷ�Χ-20��~120��
**********************************************************/
int8_t calculateTemperature(uint32_t Rth)
{
	uint8_t i=0;
	int8_t temp=0;
	uint32_t mide=0;
	
	for(i=0;i<140;i++)
	{
		if(i == 139)
		{
			return 120;//127;//�����¶���������Χ
		}
		else if(i == 0)
		{
			if(Rth > RES_TEMP_Table[0])
			{
				return -20;//-128;//�����¶���С������Χ
			}
		}
		
		if(Rth<=RES_TEMP_Table[i] && Rth>=RES_TEMP_Table[i+1])
		{
			mide = (RES_TEMP_Table[i]+RES_TEMP_Table[i+1])/2;
			if(Rth>=mide)
			{
				temp=i;
			}
			else
			{
				temp=i+1;
			}
			temp=i;
			break;
		}
	}
	return (temp-20);
}

/***************************************************************
  *	@brief	��ȡһ���¶�ֵ�����
  *	@param	
  *	@retval	uint8 �¶�ֵ 
****************************************************************/
uint8_t GetTemp(){
	uint16_t temp;
	temp=GetADC_Time(TEMP_CH,10);
	printf("temp adc value:%d\r\n",temp);
	return calculateTemperature(calculateResValue(temp));
}

/***************************************************************
  *	@brief	��ȡһ�ι���ֵ���԰ٷֱȵ���ʽ����
  *	@param	
  *	@retval	uint8 ����ֵ
****************************************************************/
uint8_t GetLight(){
	uint16_t	light;
	light=GetADC_Time(LIGHT_CH,10);
	printf("light adc value:%d\r\n",light);
	return (uint8_t)(100-(light*1000/4096)*100/1000);
}



