/***************************************************************
  *	Name		:	IIC.H
  *	Data		:	2019.7.30
  *	Author	:	NIEAOWEI
  *	Note		:	
							���������IICЭ�����оƬ����Ҫд��IIC������
****************************************************************/
#include "xfs5152ce.h"
#include "iic.h"
#include "string.h"
#include "stdio.h"
#include "delay.h"
/*
	��ض���
*/
#define XFS5152CE_ADDR	0x40 //�豸��ַ
#define XFS_TIMEOUT			1000	//ȫ�ֳ�ʱʱ��ms
/*
	оƬָ��
*/
typedef enum {
	XFS_CMD_STATUS = 0x21,//��ѯ״̬
	XFS_CMD_SLEEP = 0x88,//����͹���ģʽ
	XFS_CMD_WAKE_UP = 0xFF,//�ӵ͹��ķ�������ģʽ
	XFS_CMD_VOICE_START = 0x01,//��ʼ�����ϳ�
	XFS_CMD_VOICE_STOP = 0x02,//ֹͣ�����ϳ�
	XFS_CMD_VOICE_PAUSE = 0x03,//��ͣ�����ϳ�
	XFS_CMD_VOICE_RESTORE = 0x04,//�ָ������ϳ�
	XFS_CMD_SEND_TO_CACHE = 0x31,//�����ı���������
	XFS_CMD_PLAY_FROM_CACHE = 0x32,//�ӻ����������ı�
	XFS_CMD_RECOGNIZE_START = 0x10,
	XFS_CMD_RECOGNIZE_STOP = 0x1F
}xfsCommandEnum;

/*
	����ʶ��ӳ���
*/
const char *cmd_word_list[] = {
	"���ڳԷ�",  //0
	"�����޳�",  //1
	"���ڼ���",  //2
	"������Ϣ",//3
	"ͬ��",//4
	"��ͬ��",//5
	"��ȥ",//6
	"���ڼ���",//7
	"���켸��",//8
	"����Ϣ",//9
	"��ʼ��",//10
	"�����Ķ�",//11
	"�򿪹㲥",//12
	"�ص��㲥",//13
	"������",//14
	"�ص�����",//15
	"����һ��",//16
	"�ٶ�һ��",//17
	"������",//18
	"С����",//19
	"������",//20
	"��Ԥ����Ϣ",//21
	"������������",//22
	"����Ԥ����Ϣ",//23
	"��ʼ",//24
	"ֹͣ",//25
	"��ͣ",//26
	"������",//27
	"ȷ����ʼ",//28
	"ȡ��"//29
};


/***************************************************************
  *	@brief	��ʼ��оƬ��ʹ�õ�Э����������
  *	@param	void
  *	@retval	void
****************************************************************/
void XFS_Init(void){
	IIC_Init();
	if(XFS_WaitResult(XFS_ACK_INIT_SUCCESS,XFS_TIMEOUT)==1){
		printf("����ģ�鸴λ���ʼ��");
	}else{
		printf("����ģ���ϵ�λ��ʼ��");
	}
}
/***************************************************************
  *	@brief	����ָ���ı�
  *	@param	*text �����ŵ��ı���Ϣ
	* @param	format �ı���Ϣ�ı����ʽ
					@arg 	XFS_FORMAT_GB2312 = 0x00,
								XFS_FORMAT_GBK = 0x01,
								XFS_FORMAT_BIG5 = 0x02,
								XFS_FORMAT_UNICODE = 0x03,
  *	@retval	void
****************************************************************/
void XFS_Play(uint8_t* text,uint8_t format){
	uint8_t data[200];
	uint8_t lenth;
	lenth=strlen((char*)text);
	data[0]=0xfd;
	data[1]=0x00;
	data[2]=lenth+2;
	data[3]=0x01;
	data[4]=format;
	memcpy(&data[5],text,lenth);
	if(IIC_Write_Bytes(XFS5152CE_ADDR,data,lenth+5)!=0){
		return ;
	}else{
		if(XFS_WaitResult(XFS_ACK_IDEL,XFS_TIMEOUT)!=0){
			return;
		}
	}
}

/***************************************************************
	*	@brief	����ָ�������оƬ
  *	@param	cmd ������
					@arg		XFS_CMD_STATUS = 0x21,//��ѯ״̬
									XFS_CMD_SLEEP = 0x88,//����͹���ģʽ
									XFS_CMD_WAKE_UP = 0xFF,//�ӵ͹��ķ�������ģʽ
									XFS_CMD_VOICE_START = 0x01,//��ʼ�����ϳ�
									XFS_CMD_VOICE_STOP = 0x02,//ֹͣ�����ϳ�
									XFS_CMD_VOICE_PAUSE = 0x03,//��ͣ�����ϳ�
									XFS_CMD_VOICE_RESTORE = 0x04,//�ָ������ϳ�
									XFS_CMD_SEND_TO_CACHE = 0x31,//�����ı���������
									XFS_CMD_PLAY_FROM_CACHE = 0x32,//�ӻ����������ı�
									XFS_CMD_RECOGNIZE_START = 0x10,
									XFS_CMD_RECOGNIZE_STOP = 0x1F
  *	@retval	int
						0���ɹ�
						1��ʧ�ܳ�ʱ
****************************************************************/
uint8_t XFS_SendCmd(uint8_t cmd){
	uint8_t temp_cmd[4];
	temp_cmd[0]=0xfd;
	temp_cmd[1]=0x00;
	temp_cmd[2]=0x01;
	temp_cmd[3]=cmd;
	
	if(IIC_Write_Bytes(XFS5152CE_ADDR,temp_cmd,4)!=0){
		return 1;
	}
		printf("cmd scusee");
	if(XFS_WaitResult(XFS_ACK_CMD_RIGHT,XFS_TIMEOUT)!=0){
		return 1;
	}
	printf("cmd scusee");
	return 0;
	
}
/***************************************************************
  *	@brief	�ȴ��ش���Ϣ
  *	@param	result ��Ҫ�ȴ��Ľ��
	*	@param	waittime	��ʱʱ��
  *	@retval	uint8_t
										0���ɹ�
										1����ʱʧ��
****************************************************************/
uint8_t XFS_WaitResult(uint8_t result, uint16_t waittime){
	uint8_t temp;
	while(waittime--){
		delay_ms(1);
		if(IIC_ReadBytes_Ack(XFS5152CE_ADDR,&temp,1)!=0){
			continue;
		}
		if(temp==result){
			printf("%x ",temp);
			return 0;//�ɹ�
		}
	}
	return 1;//��ʱ
}
/***************************************************************
  *	@brief	����ʶ��
  *	@param	*result ����ʶ�����洢��ַ
	* @param 	waittime �ȴ�ʱ��
  *	@retval	uint8_t
								XFS_RECO_UNKNOWN=0X00,
								XFS_RECO_SUCCESS=0X01,
								XFS_RECO_TIMEOUT=0X02,
								XFS_RECO_REJECT=0X03,
								XFS_RECO_ERROR=0X04,
***************************************************************/
uint8_t XFS_Recogonize(uint8_t *result,uint32_t waittime ){
	uint8_t rec_data[10];
	if(XFS_SendCmd(XFS_CMD_RECOGNIZE_START)==1){
		return XFS_RECO_UNKNOWN;
	}
	while(waittime--){
		delay_ms(1);
		if(IIC_ReadBytes_Ack(XFS5152CE_ADDR,&rec_data[0],5)==0){
			if(rec_data[0]==0xfc){
				printf("%x ,,",rec_data[0]);
				break;
			}
		}
	}
	
	if(waittime>0 && rec_data[0]==0xfc){
		
		switch(rec_data[3]){
			case XFS_RECO_TIMEOUT:
					return XFS_RECO_TIMEOUT;
			case XFS_RECO_REJECT:
					return XFS_RECO_REJECT;
			case XFS_RECO_ERROR:
					return XFS_RECO_ERROR;
			case XFS_RECO_SUCCESS:
					*result=rec_data[4];
					break;
		}
	}
	
	if(XFS_SendCmd(XFS_CMD_RECOGNIZE_STOP)!=0){
		return XFS_RECO_UNKNOWN;
	}
	printf("%x,%x,%x,%x,%x",rec_data[0],rec_data[1],rec_data[2],rec_data[3],rec_data[4]);
	return XFS_RECO_SUCCESS;
}

