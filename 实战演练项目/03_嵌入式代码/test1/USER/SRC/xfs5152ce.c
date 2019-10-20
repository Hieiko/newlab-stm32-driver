/***************************************************************
  *	Name		:	XFS5152CE.C
  *	Data		:	2019.7.15
  *	Author	:	NIEAOWEI
  *	Note		:	������ز���������ʵ��
****************************************************************/
#include "xfs5152ce.h"
#include "usart3.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"

//��Ӧ��ʱʱ��
#define XFS_RESPONSE_TIMEOUT 3000

static unsigned char firstflag=1;
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

//�ȴ�оƬ�ش���Ӧ
int wait_response(uint8_t result, uint16_t waittime){
	unsigned char temp;
	while(waittime--){
		delay_ms(1);
		if(!FIFO_Is_Empty(&usart3_rx_fifo)){
			if(FIFO_Out_Byte(&usart3_rx_fifo,&temp)!=FIFO_OP_FAIL){
				
			}
			if(result==temp){
				printf("result=%x\r\n",temp);
				return 1;//�ȴ��ɹ�
			}
		}
		
	}
	printf("wait...\r\n");
	return -1;//��ʱ
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
						1���ɹ�
						-1��ʧ�ܳ�ʱ
****************************************************************/
static int xfs_send_cmd(uint8_t cmd)
{
	unsigned  char frame_info[8]; //������ı�����
	uint16_t frame_len = 0;
	uint8_t temp;
/*****************֡�̶�������Ϣ**************************************/           
	frame_info[frame_len++] = 0xFD ; 			//����֡ͷFD
	frame_info[frame_len++] = 0x00 ; 			//�������������ȵĸ��ֽ�
	frame_info[frame_len++] = 0x01; 		//�������������ȵĵ��ֽ�
	frame_info[frame_len++] = cmd ; 			//���������� 

	if(firstflag==0){
		if(wait_response(XFS_ACK_CMD_RIGHT,XFS_RESPONSE_TIMEOUT)==1){
			if(wait_response(XFS_ACK_IDEL,XFS_RESPONSE_TIMEOUT)==1){
				usart3_send_str(frame_info, frame_len);
				firstflag=1;
			}
		}
	}else{
		usart3_send_str(frame_info, frame_len);
	}
	return wait_response(XFS_ACK_CMD_RIGHT, XFS_RESPONSE_TIMEOUT);
}
/***************************************************************
  *	@brief	��ʼ��оƬ
  *	@param	void
  *	@retval	int
							1���ɹ�
****************************************************************/
int xfs_init(void){
	usart3_init();
	return 1;
}
/***************************************************************
  *	@brief	��ȡоƬ��ǰ״̬
  *	@param	void
  *	@retval	int 
									XFS_ACK_INIT_SUCCESS = 0x4A,
									XFS_ACK_CMD_RIGHT = 0x41,
									XFS_ACK_CMD_ERROR = 0x45,
									XFS_ACK_BUSY = 0x4E,
									XFS_ACK_IDEL = 0x4F,
****************************************************************/
int xfs_get_status(void){
	unsigned char cmd[4];
	unsigned char temp=0;
	cmd[0]=0xFD;
	cmd[1]=0X00;
	cmd[2]=0X01;
	cmd[3]=0X21;
	usart3_send_str(cmd,4);
	if(FIFO_Out_Byte(&usart3_rx_fifo,&temp)!=FIFO_OP_FAIL)
	return (int)temp;
}
/***************************************************************
  *	@brief	ʹоƬ����˯��״̬
  *	@param	void
  *	@retval	int
							1���ɹ�
							-1����ʱʧ��
****************************************************************/
int xfs_sleep(void){
	return xfs_send_cmd(XFS_CMD_SLEEP);
}
/***************************************************************
  *	@brief	����оƬ
  *	@param	void
  *	@retval	int
							1���ɹ�
							-1����ʱʧ��
****************************************************************/
int xfs_wake_up(void){
	return xfs_send_cmd(XFS_CMD_WAKE_UP);
}
/***************************************************************
  *	@brief	ֹͣ��ǰ�ϳ�
  *	@param	void
  *	@retval	int
							1���ɹ�
							-1����ʱʧ��
****************************************************************/
int xfs_voice_stop(void){
	return xfs_send_cmd(XFS_CMD_VOICE_STOP);
}
/***************************************************************
  *	@brief	��ͣ��ǰ�ϳ�
  *	@param	void
  *	@retval	int
							1���ɹ�
							-1����ʱʧ��
****************************************************************/
int xfs_voice_pause(void){
	return xfs_send_cmd(XFS_CMD_VOICE_PAUSE);
}
/***************************************************************
  *	@brief	�ָ�����ͣ�ĺϳ�
  *	@param	void
  *	@retval	int
							1���ɹ�
							-1����ʱʧ��
****************************************************************/
int xfs_voice_restore(void){
	return xfs_send_cmd(XFS_CMD_VOICE_RESTORE);
}
/***************************************************************
  *	@brief	�����ϳɲ���ָ���ı�
  *	@param	*data ��Ҫ�ϳɲ��ŵ��ı�
	* @param	format �����ʽ
				@arg 	XFS_FORMAT_GB2312 = 0x00,
							XFS_FORMAT_GBK = 0x01,
							XFS_FORMAT_BIG5 = 0x02,
							XFS_FORMAT_UNICODE = 0x03,
	* @param	wait_finish �Ƿ�ȴ��ϴκϳɽ���
  *	@retval	int
								0���ɹ�
								1��ʧ��
****************************************************************/
int xfs_voice_start(unsigned char *data,unsigned char format,int wait_finish){
	unsigned char cmd[100]="";
	unsigned int lenth=strlen((char *)data);

	cmd[0]=0xfd;
	cmd[1]=((lenth+2)>>8)&0xff;
	cmd[2]=(lenth+2)&0xff;
	cmd[3]=0x01;
	cmd[4]=format;
	memcpy(&cmd[5],data,lenth);
	/*usart3_send_str(cmd,lenth+5);
	if(wait_finish==1){
		if(wait_response(XFS_ACK_CMD_RIGHT,XFS_RESPONSE_TIMEOUT)==1){
			if(wait_response(XFS_ACK_IDEL,XFS_RESPONSE_TIMEOUT)==1){
				return 1;
			}
		}
	}*/
	//�ȴ��ϴκϳ����
	if(wait_finish==1 && firstflag==0){
		if(wait_response(XFS_ACK_CMD_RIGHT,XFS_RESPONSE_TIMEOUT)==1){
			if(wait_response(XFS_ACK_IDEL,XFS_RESPONSE_TIMEOUT)==1){
				usart3_send_str(cmd,lenth+5);
			}else{
				return 1;
			}
		}else{
			return 1;
		}
	}else{
		usart3_send_str(cmd,lenth+5);
	}
	if(firstflag==1){
			//FIFO_Out_Byte(&usart3_rx_fifo,&data[99]);
			firstflag=0;
		}
	return 0;
}
/***************************************************************
  *	@brief	����ʶ��
  *	@param	*result ����ʶ�����洢��ַ
	* @param 	waittime �ȴ�ʱ��
  *	@retval	int
								XFS_RECO_UNKNOWN=0X00,
								XFS_RECO_SUCCESS=0X01,
								XFS_RECO_TIMEOUT=0X02,
								XFS_RECO_REJECT=0X03,
								XFS_RECO_ERROR=0X04,
***************************************************************/
int xfs_recognize(unsigned char *result,unsigned int waittime){
	uint8_t temp[10];
	uint8_t lenth=1;
	int err = 1;
	printf("start..");
	if(xfs_send_cmd(XFS_CMD_RECOGNIZE_START)!=1){
		err=XFS_RECO_UNKNOWN;
	}
	
	while(waittime--){
		delay_ms(1);
		if(FIFO_Out_Byte(&usart3_rx_fifo,&temp[0])!=FIFO_OP_FAIL){
			if(temp[0]==0xfc){
				printf("%x\r\n",temp[0]);
				break;
			}
		}
	}
	
	while(lenth<10){
		if(FIFO_Out_Byte(&usart3_rx_fifo,&temp[lenth])!=FIFO_OP_FAIL)
		{
			printf("%x ",temp[lenth]);
			
		}
		lenth++;
	}
	
	if(waittime>0 && temp[0]==0xfc){
		
		switch(temp[3]){
			case XFS_RECO_TIMEOUT:
					err=XFS_RECO_TIMEOUT;
					break;
			case XFS_RECO_REJECT:
					err=XFS_RECO_REJECT;
					break;
			case XFS_RECO_ERROR:
					err=XFS_RECO_ERROR;
					break;
			case XFS_RECO_SUCCESS:
					err=XFS_RECO_SUCCESS;
					*result=temp[4];
					break;
		}
	}
	printf("stop..");
	if(xfs_send_cmd(XFS_CMD_RECOGNIZE_STOP)  != 1){
		err = XFS_RECO_UNKNOWN;
			printf("err:%d\r\n",err);
	}

	return err;
}

