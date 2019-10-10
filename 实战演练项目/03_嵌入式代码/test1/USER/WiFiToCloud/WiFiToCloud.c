/******************************************************************************
*
*
*
*
*
*
*
*
*
*
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "delay.h"
#include "lcd12864.h"
#include "rtc.h"
#include "relay.h"
#include "WiFiToCloud.h"


uint8_t wait_result(const char *result,uint16_t timeout){
	uint8_t temp[128];
	while(timeout--){
		FIFO_Out_Bytes(&uart4_rx_fifo,temp);
		if(strstr((const char*)temp,result)!=NULL){
			printf("res2: %s \r\n",temp);
			return 0;
		}
		ESP8266_DataAnalysisProcess(temp);
		memset(temp,0,128);
		delay_ms(100);
		//delay_ms(1);
	}
	return 1;
}

/*******************************************************************
*������int8_t ESP8266_SetStation(void)
*���ܣ�ESP8266����Ϊstationģʽ
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SetStation(void)
{
	//ClrAtRxBuf();//��ջ���
	uint8_t temp[50];
	SendAtCmd((uint8_t *)AT_CWMODE,strlen(AT_CWMODE));
	delay_ms(200);
	if(wait_result("OK",1000)){
		return -1;
	}
	return 0;
}

/*******************************************************************
*������int8_t ESP8266_SetAP(void)
*���ܣ�����ESP8266Ҫ���ӵ��ȵ����ƺ�����
*���룺char *wifi-�ȵ����� char *pwd-�ȵ�����
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd)
{
	uint8_t temp[50];
	uint8_t AtCwjap[MAX_AT_TX_LEN];
	memset(AtCwjap, 0x00, MAX_AT_TX_LEN);//��ջ���
	//ClrAtRxBuf();//��ջ���
	sprintf((char *)AtCwjap,"AT+CWJAP_CUR=\"%s\",\"%s\"",wifi, pwd);
	SendAtCmd((uint8_t *)AtCwjap,strlen((const char *)AtCwjap));
	if(wait_result((const char*)"OK",5500)){
		return -1;
	}
	return 0;
}

/*******************************************************************
*������int8_t ESP8266_SetStation(void)
*���ܣ�ESP8266����TCP����
*���룺
	char *IpAddr-IP��ַ�����磺120.77.58.34
	uint16_t port-�˿ںţ�ȡֵ0~65535
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port)
{
	uint8_t IpStart[MAX_AT_TX_LEN];
	memset(IpStart, 0x00, MAX_AT_TX_LEN);//��ջ���
	//ClrAtRxBuf();//��ջ���
	sprintf((char *)IpStart,"AT+CIPSTART=\"TCP\",\"%s\",%d",IpAddr, port);
	SendAtCmd((uint8_t *)IpStart,strlen((const char *)IpStart));
	if(wait_result("OK",1500))
	{
		return -1;
	}
	return 0;
}

/*******************************************************************
*������int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
*���ܣ�ESP8266��������
*���룺
		char *IpBuf-IP����
		uint8_t len-���ݳ���
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t IpSend[MAX_AT_TX_LEN];
	memset(IpSend, 0x00, MAX_AT_TX_LEN);//��ջ���
	//ClrAtRxBuf();//��ջ���
	sprintf((char *)IpSend,"AT+CIPSEND=%d",len);
	//printf("%s\r\n",IpSend);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)IpSend,strlen((const char *)IpSend));
	
	if(wait_result("OK",500))
	{
		return -1;
	}
	//ClrAtRxBuf();//��ջ���
	SendStrLen((uint8_t *)IpBuf, len);
	//printf("%s\r\n",IpBuf);////////////////////////////////////////////////////////////
	for(TryGo = 0; TryGo<60; TryGo++)//���ȴ�ʱ��100*60=6000ms
	{
		
		if(wait_result("SEND OK",200))
		{
			error = -2;
		}
		else
		{
			error = 0;
			break;
		}
	}
	return error;
}

/*******************************************************************
*������int8_t ConnectToServer(void)
*���ܣ����ӵ�������
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey)
{
	uint16_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	printf("����WiFiͨѶģ�鹤��ģʽ\r\n");
//	LCD_WriteEnglishString(4,0,(unsigned char *)"Set WiFi Station");//                
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_SetStation() == 0)//����WiFiͨѶģ�鹤��ģʽ
		{
			error = 0;
			break;
		}
		else
		{
			error = -1;
		}
	}
	if(error < 0)
	{
		return error;
	}
	printf("�����ȵ����ƺ�����\r\n");
//	LCD_WriteEnglishString(4,0,(unsigned char *)"Connect To WiFi ");//                
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_SetAP((char *)WIFI_AP, (char *)WIFI_PWD) == 0)//�����ȵ����ƺ�����
		{
			error = 0;
			break;
		}
		else
		{
			error = -2;
		}
	}
	if(error < 0)
	{
		return error;
	}
	printf("���ӷ�����\r\n");
//	LCD_WriteEnglishString(4,0,(unsigned char *)"ConnectToServer ");//                
	for(TryGo = 0; TryGo<3; TryGo++)
	{
		if(ESP8266_IpStart((char *)SERVER_IP,SERVER_PORT) == 0)//���ӷ�����IP��ַ���˿ڣ�120.77.58.34,8600
		{
			error = 0;
			break;
		}
		else
		{
			error = -3;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	sprintf((char *)TxetBuf,"{\"t\":1,\"device\":\"%s\",\"key\":\"%s\",\"ver\":\"v0.0.0.0\"}",DeviceID,SecretKey);
	printf("������֤��%s\r\n",TxetBuf);  
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-4;
	}else
	{//���ͳɹ�
		for(TryGo = 0; TryGo<2; TryGo++)//���ȴ�ʱ��500*10=5000ms
		{
			if(wait_result((const char *)"\"status\":0",500))//�����Ӧ״̬�Ƿ�Ϊ���ֳɹ�
			{
				printf("error\r\n");
				error = -5;
			}
			else
			{
				error = 0;
				break;
			}
		}
	}
	
	return error;
}

/*******************************************************************
*������int8_t ESP8266_SendSensor(char *ApiTag, 
									uint32_t sensor, char *TimeStr)
*���ܣ����ʹ������ݵ�������
*���룺
	char *ApiTag ʵ������ǩ����
	uint32_t sensor, ������ֵ
	char *TimeStr���ɼ�����ʱ��ʱ�䣬����yyyy-mm-dd hh:mm:ss��ʽ
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SendSensor(char *ApiTag, uint32_t sensor, char *TimeStr)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	sprintf((char *)TxetBuf,"{\"t\":3,\"datatype\":2,\"datas\":{\"%s\":{\"%s\":%d}},\"msgid\":001}",ApiTag,TimeStr,sensor);
	//printf("%s\r\n",TxetBuf);////////////////////////////////////////////////////////////
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-1;
	}
	else
	{//���ͳɹ�
		for(TryGo = 0; TryGo<10; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(wait_result((const char *)"\"status\":0",10))//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				error = -1;
			}
			else
			{
				printf("send ok\r\n");
				error = 0;
				break;
			}
			//delay_ms(10);
		}
	}
	return error;
}

/*******************************************************************
*������int8_t ESP8266_SendSensorTry3rd(char *SensorTag, char *TimeStr, uint8_t sensor)
*���ܣ����ʹ������ݵ�������
*���룺
	char *SensorTag, �����ǩ����
	uint8_t sensor, ������ֵ
	char *TimeStr,�ɼ�����ʱ��ʱ�䣬����yyyy-mm-dd hh:mm:ss��ʽ
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵��������ʧ�ܻ��ط����ط��������
*******************************************************************/
int8_t ESP8266_SendSensorTry3rd(char *SensorTag, uint8_t sensor, char *TimeStr)
{
	uint8_t TryGo = 0;
	uint8_t TrySend = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	sprintf((char *)TxetBuf,"{\"t\":3,\"datatype\":2,\"datas\":{\"%s\":{\"%s\":%d}},\"msgid\":001}",SensorTag, TimeStr, sensor);
	//printf("%s\r\n",TxetBuf);////////////////////////////////////////////////////////////
SendAgain:
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-1;
	}
	else
	{//���ͳɹ�
		for(TryGo = 0; TryGo<50; TryGo++)//���ȴ�ʱ��50*10=500ms
		{
			if(wait_result((const char *)"\"status\":0",10))//�����Ӧ״̬�Ƿ�Ϊ"�ϱ��ɹ�"
			{
				error = -1;
			}
			else
			{
				error = 0;
				break;
			}
			delay_ms(10);
		}
	}
	if((error != 0)&&(TrySend<2))
	{
		printf("�ط���������%d��\r\n",TrySend+1);
		TrySend++;
		goto SendAgain;
	}
	return error;
}

/*******************************************************************
*������uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
*���ܣ���������������
*���룺
		uint8_t *AtRxBuf ��ԭʼAT���ڻ���
		char *GetIpData ����ȡ������Itcp/ip����
*����������յ���IP���ݳ���
*����˵����
	AT+CIPSEND=76
	{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
	+IPD,29:{"msgid":1,"status":0,"t":4}
*******************************************************************/
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
{
	char *Point = NULL;
	uint8_t len = 0;
	
	Point = strstr((const char *)AtRxBuf, (const char *)"+IPD,");
	if(Point != NULL)//���ģ���յ�TCP/IP���ݰ���
	{
		len=(Point[5]-'0')*10+Point[6]-'0';//����IP���ݳ��Ȳ��ᳬ��100���ʿ��԰�����λ���͡�ʮλ�������յ���IP���ݳ���
		Point = strstr((const char *)AtRxBuf, (const char *)":")+1;
		memcpy(GetIpData, Point, len);
		GetIpData[len]='\0';
		printf("�յ�IP���ݣ�%s\r\n",GetIpData);///////////////////////////////////////////////////////
		printf("�յ�IP���ݳ���=%d\r\n",len);///////////////////////////////////////////////////
	}
	
	return (len);
}

/*******************************************************************
*������void ESP8266_DataAnalysisProcess(char *RxBuf)
*���ܣ���������������
*���룺char *RxBuf �������·�����
*�����
*����˵�����û������ڴ˻����ϸ������չ�ú���������ֻ�Ǹ��򵥵�DEMO
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf)
{
	if(strstr((const char *)RxBuf, (const char *)"\"t\":5") != NULL)//��������
	{
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"latern_cmd\"") != NULL)//����/��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				printf("������\r\n");
				latern_ON();
				;//...
				;//...
				;//...
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				latern_OFF();
				printf("������\r\n");
				;//...
				;//...
				;//...
			}
		}
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"door_cmd\"") != NULL)//����/��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				printf("������\r\n");
				latern_ON();
				;//...
				;//...
				;//...
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				latern_OFF();
				printf("������\r\n");
				;//...
				;//...
				;//...
			}
		}
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"lcd_text_cmd\"") != NULL)//����/��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":") != NULL))//����
			{
				char *temp;
				temp=strstr((const char *)RxBuf, (const char *)"\"data\":");
				lcd_writeLenthChar(3,0,temp+8,strlen(temp+8));
			}
		}
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"time_cmd\"") != NULL)//����/��������
		{
		 if((strstr((const char *)RxBuf, (const char *)"\"data\":") != NULL))//����
			{
				char *temp;
				uint8_t date[8],i;
				temp=strstr((const char *)RxBuf, (const char *)"\"data\":");
				lcd_writeLenthChar(3,0,temp+8,10);
				date[0]=temp[0+8]-'0';
				date[1]=temp[1+8]-'0';
				date[2]=temp[2+8]-'0';
				date[3]=temp[3+8]-'0';
				date[4]=temp[5+8]-'0';
				date[5]=temp[6+8]-'0';
				date[6]=temp[8+8]-'0';
				date[7]=temp[9+8]-'0';
				for(i=0;i<8;i++){
					printf("%d,",date[i]);
				}
				RTC_Init(date[0]*1000+date[1]*100+date[2]*10+date[3],date[4]*10+date[5],date[6]*10+date[7],0,0,0,1);
			}
		}
	}
}
