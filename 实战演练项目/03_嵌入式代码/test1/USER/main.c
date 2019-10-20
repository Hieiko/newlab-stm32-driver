
/***************************************************************
  *	Name		:	MAIN.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	����1��ʹ�ã����ö��л���������ݣ���֤��������
							���ܱ�����λ��
****************************************************************/
#include "usart1.h"
#include "delay.h"
#include <stdio.h>
#include "WiFiToCloud.h"
#include "lcd12864.h"
#include "rtc.h"
#include "digital_tube.h"
#include "relay.h"
#include "temp_light.h"
#include "key.h"
#include "timer.h"
#include "lf125k.h"
#include "uart5.h"
#include "timer7.h"
#include "uart4.h"


#define default_time 2019,1,14,17,42,55,1

void connect_cloud_process();
void lcd_display_process();
void digital_tube_process();
void rtc_process();
void door_process();
void key_process();
void cloud_process();

uint16_t key_counter=0;
uint16_t cloud_counter=0;
uint16_t tube_counter=0;

int main(){
	Byte temp[20];
	uint8_t key;
	//�жϹ����ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	//�����ʼ��
	delay_init();//ʱ�ӳ�ʼ��
	Usart1_Init(115200);//����1��ʼ��
	uart4_init();
	UART5_Init(115200);
	
	connect_cloud_process();//������ƽ̨
	
	TEMP_LIGHT_Init();//�¶ȹ��ճ�ʼ��
	
	Relay_Init();//�̵�����ʼ��
	
	RTC_Init(default_time);//ʱ���ʼ��
	
	HC595_GpioInit();//����ܳ�ʼ��
	
	lcd_init();//��ʾ����ʼ��
	//��Ƶģ���ʼ��
	TIM7_Init();
	TIM6_Init();
	LF125K_init();
	
	//
	rtc_process();
	
	//digital_tube_process();
	while(1){
		if(key_counter>20){
			key_process();
			key_counter=0;
		}
		
		if(tube_counter>1){
			digital_tube_process();
			tube_counter=0;
		}
		
	
			cloud_process();
			
		
	}
}

void connect_cloud_process(){
	uint8_t ack;
	ack=ConnectToServer((char *)MY_DEVICE_ID, (char *)MY_SECRET_KEY);
	printf("wifi ack %d \r\n",ack);
}

void lcd_display_process(){
	
	//show_logo();
}
//���������ˢ�½���
void digital_tube_process(){
	uint8_t led_0,led_1,led_2,led_3;
	led_0=calendar.w_year/1000;
	led_1=calendar.w_year%1000/100;
	led_2=calendar.w_year%1000%100/10;
	led_3=calendar.w_year%1000%100%10;
	uint8_t date[8]={led_0,led_1,led_2,led_3|B_dot,calendar.w_month/10,calendar.w_month%10|B_dot,calendar.w_date/10,calendar.w_date%10};
	set_led_dispbuf(date,0,8);
	led_scan();
}

void rtc_process(){
	//delay_ms(1000);
	RTC_Init(2029,1,14,17,42,55,1);
	printf("%d-%d-%d \r\n",calendar.w_year,calendar.w_month,calendar.w_date);
}

void door_process(){
	door_ON();
	latern_OFF();
}


void cloud_process(){
	
	if(cloud_counter>2000){
		
		printf("cloud process\r\n");
		temp_value=GetTemp();
		ESP8266_SendSensor("door_status",door_status,"");
		ESP8266_SendSensor("latern_status",latern_status,"");
		ESP8266_SendSensor("temp_value",temp_value,"");
		cloud_counter=0;
	}
	
	
}

void key_process(){
		uint8_t key;
		key = Key_Scan();
		if(key==KEY1_PRESSED){
			if(door_status==0){
				door_ON();
				
			}else{
				door_OFF();
			}
		}
		if(key==KEY2_PRESSED){
			if(latern_status==0){
				latern_ON();
			}else{
				latern_OFF();
			}
		}
}

