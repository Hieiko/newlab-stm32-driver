/***************************************************************
  *	Name		:	AS608.C
  *	Data		:	2019.8.2
  *	Author	:	NIEAOWEI
  *	Note		:	ָ��ģ��������ʵ��
****************************************************************/
#include "as608.h"
#include "sys.h"
#include "uart5.h"
#include "delay.h"

#define AS608ADDR	0xffffffff

//as608��16λоƬ����¼ָ��оƬ����Ϣ
struct asinfo{
	uint16_t sys_reg;//ϵͳ״̬�Ĵ��� 
	uint16_t sensor_type;//���������ͣ�ƫ��1��
	uint16_t store_size;//ָ�ƿ��С ƫ��2��
	uint16_t safe;//��ȫ�ȼ� ƫ��3��
	uint32_t addr;//�豸��ַ ƫ��4��
	uint16_t data_size;//���ݰ���С ƫ��6��
	uint16_t baudrate;//������ ƫ��7��
	uint8_t Store[300];//ָ�ƿ��¼
	uint16_t ValidNum;//��Ч����
}AS608_Info;

typedef enum{
	CMD_PACKAGE=0X01,
	DATA_PACKAGE=0X02,
	FINISH_PACKAGE=0X08,
	ACK_PACKAGE=0X07,
}PACKAGE_Type;
//��¼ָ�ƿ������е�λ��
/*
		����Ӧ���
*/
typedef enum{
	AS608_ACK_OK,
	AS608_ACK_DATA_REC_ERROR,
	AS608_ACK_NO_FINGER,
	AS608_ACK_RECORD_FAIL,
	AS608_ACK_IMAGE_DRY,
	AS608_ACK_IAMGE_WET,
	AS608_ACK_IMAGE_CHAOS,
	AS608_ACK_IMAGE_FEATURE_LESS,
	AS608_ACK_MATCH_FAIL,
	AS608_ACK_SEARCH_FAIL,
	AS608_ACK_MEIGE_FAIL,
	AS608_ACK_NUM_OUVERFLOW,
	AS608_ACK_READ_TEMPLATE_FAIL,
	AS608_ACK_UPLOAD_FEATURE_FAIL,
	AS608_ACK_DATA_REC_BREAK,
	AS608_ACK_UPLOAD_IMAGE_FAIL,
	AS608_ACK_DELETE_TEMPLATE_FAIL,
	AS608_ACK_EMPTY_STORE_FAIL,
	AS608_ACK_LOWPORWER_FAIL,
	AS608_ACK_PASSWORD_ERROR,
	AS608_ACK_RESET_FAIL,
	AS608_ACK_BUFFER_NO_ORIGINAL_IAMGE,
	AS608_ACK_ONLINE_UPDATE_FAIL,
	AS608_ACK_FINGER_NO_CHANGE,
	AS608_ACK_READWRITE_FLASH_ERROR,
	AS608_ACK_FOLLOW_DATA,
	AS608_ACK_FOLLOW_CMD,
	AS608_ACK_WRITE_FLASH_PARITY_ERROR,
	AS608_ACK_WRITE_FLASH_ID_ERROR,
	AS608_ACK_WRITE_FLASH_LENTH_ERROR,
	AS608_ACK_WRITE_FLASH_CODELENTH_ERROR,
	AS608_ACK_WRITE_FLASH_ERROR,
	AS608_ACK_UNKNOWN_ERROR,
	AS608_ACK_NO_REGISTER,
	AS608_ACK_REGISTER_CONTENT_ERROR,
	AS608_ACK_TEXT_PAGE_ERROR,
	AS608_ACK_PORT_ERROR,
	AS608_ACK_ENROLL_ERROR,
	AS608_ACK_STORE_FULL,	
}AS608_ACK_State;

uint8_t AS608_GetAck(uint8_t ack){
	switch(ack){
		case AS608_ACK_OK:return AS608_ACK_OK;
		case AS608_ACK_DATA_REC_ERROR:return AS608_ACK_DATA_REC_ERROR;
		case AS608_ACK_NO_FINGER:return AS608_ACK_NO_FINGER;
		case AS608_ACK_RECORD_FAIL:return AS608_ACK_RECORD_FAIL;
		case AS608_ACK_IMAGE_DRY:return AS608_ACK_IMAGE_DRY;
		case AS608_ACK_IAMGE_WET:return AS608_ACK_IAMGE_WET;
		case AS608_ACK_IMAGE_CHAOS:return AS608_ACK_IMAGE_CHAOS;
		case AS608_ACK_IMAGE_FEATURE_LESS:return AS608_ACK_IMAGE_FEATURE_LESS;
		case AS608_ACK_MATCH_FAIL:return AS608_ACK_MATCH_FAIL;
		case AS608_ACK_SEARCH_FAIL:return AS608_ACK_SEARCH_FAIL;
		case AS608_ACK_MEIGE_FAIL:return AS608_ACK_MEIGE_FAIL;
		case AS608_ACK_NUM_OUVERFLOW:return AS608_ACK_NUM_OUVERFLOW;
		case AS608_ACK_READ_TEMPLATE_FAIL:return AS608_ACK_READ_TEMPLATE_FAIL;
		case AS608_ACK_UPLOAD_FEATURE_FAIL:return AS608_ACK_UPLOAD_FEATURE_FAIL;
		case AS608_ACK_DATA_REC_BREAK:return AS608_ACK_DATA_REC_BREAK;
		case AS608_ACK_UPLOAD_IMAGE_FAIL:return AS608_ACK_UPLOAD_IMAGE_FAIL;
		case AS608_ACK_DELETE_TEMPLATE_FAIL:return AS608_ACK_DELETE_TEMPLATE_FAIL;
		case AS608_ACK_EMPTY_STORE_FAIL:return AS608_ACK_EMPTY_STORE_FAIL;
		case AS608_ACK_LOWPORWER_FAIL:return AS608_ACK_LOWPORWER_FAIL;
		case AS608_ACK_PASSWORD_ERROR:return AS608_ACK_PASSWORD_ERROR;
		case AS608_ACK_RESET_FAIL:return AS608_ACK_RESET_FAIL;
		case AS608_ACK_BUFFER_NO_ORIGINAL_IAMGE:return AS608_ACK_BUFFER_NO_ORIGINAL_IAMGE;
		case AS608_ACK_ONLINE_UPDATE_FAIL:return AS608_ACK_ONLINE_UPDATE_FAIL;
		case AS608_ACK_FINGER_NO_CHANGE:return AS608_ACK_FINGER_NO_CHANGE;
		case AS608_ACK_READWRITE_FLASH_ERROR:return AS608_ACK_READWRITE_FLASH_ERROR;
		case AS608_ACK_FOLLOW_DATA:return AS608_ACK_FOLLOW_DATA;
		case AS608_ACK_FOLLOW_CMD:return AS608_ACK_FOLLOW_CMD;
		case AS608_ACK_WRITE_FLASH_PARITY_ERROR:return AS608_ACK_WRITE_FLASH_PARITY_ERROR;
		case AS608_ACK_WRITE_FLASH_ID_ERROR:return AS608_ACK_WRITE_FLASH_ID_ERROR;
		case AS608_ACK_WRITE_FLASH_LENTH_ERROR:return AS608_ACK_WRITE_FLASH_LENTH_ERROR;
		case AS608_ACK_WRITE_FLASH_CODELENTH_ERROR:return AS608_ACK_WRITE_FLASH_CODELENTH_ERROR;
		case AS608_ACK_WRITE_FLASH_ERROR:return AS608_ACK_WRITE_FLASH_ERROR;
		case AS608_ACK_UNKNOWN_ERROR:return AS608_ACK_UNKNOWN_ERROR;
		case AS608_ACK_NO_REGISTER:return AS608_ACK_NO_REGISTER;
		case AS608_ACK_REGISTER_CONTENT_ERROR:return AS608_ACK_REGISTER_CONTENT_ERROR;
		case AS608_ACK_TEXT_PAGE_ERROR:return AS608_ACK_TEXT_PAGE_ERROR;
		case AS608_ACK_PORT_ERROR:return AS608_ACK_PORT_ERROR;
		case AS608_ACK_ENROLL_ERROR:return AS608_ACK_ENROLL_ERROR;
		case AS608_ACK_STORE_FULL:return AS608_ACK_STORE_FULL;	
	}
	return 0xff;
}


/***************************************************************
  *	@brief	�ӽ��ջ�������ȡһ����Ӧ��
  *	@param	*rec ����������ݵĵ�ַ
  *	@retval	uint8_t
									���յ��İ�������
									CMD_PACKAGE=0X01,
									DATA_PACKAGE=0X02,
									FINISH_PACKAGE=0X08,
									ACK_PACKAGE=0X07,
									0��ʧ��
****************************************************************/
uint8_t PS_GetAckData(uint8_t *rec,uint16_t mtime){
	uint16_t index=0,i;
	//�ȴ���ͷ
	do{
		if(FIFO_Out_Byte(&uart5_rx_fifo,&rec[index])!=FIFO_OP_FAIL){
			if(rec[index]==0xef){
				index++;
				break;
			}
		}
		delay_ms(1);
	}while(mtime--);
	//�����ʱ
	if(mtime==0){
		return 0;
	}
	delay_ms(100);//�ȴ��������
	//���պ�������
	do{
		if(FIFO_Out_Byte(&uart5_rx_fifo,&rec[index])!=FIFO_OP_FAIL){
			index++;
		}
	}while(uart5_rx_fifo.data[uart5_rx_fifo.out+1]!=0xEF&&!FIFO_Is_Empty(&uart5_rx_fifo));
	//��������ݰ����������պ�������
	if(rec[6]==DATA_PACKAGE){
		while(index<(12+AS608_Info.data_size)){
			if(FIFO_Out_Byte(&uart5_rx_fifo,&rec[index])!=FIFO_OP_FAIL){
				index++;
			}
		}
	}
	printf("index:%d\r\n",index);
	for(i=0;i<index;i++){
				printf("%X ",rec[i]);
			}
	printf("\r\n");
	return rec[6];
}
/***************************************************************
	*	@brief	���������
  *	@param	*cmd ��������ݵ�ַ
	*	@param  type ��������
	* @param	lenth ���ݰ��ĳ���
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_SendCmd(uint8_t *cmd,PACKAGE_Type type,uint16_t lenth){
	uint8_t data[256],index=0,i;
	data[index++]=0XEF;
	data[index++]=0x01;
	data[index++]=AS608ADDR>>24;
	data[index++]=(uint8_t)(AS608ADDR>>16);
	data[index++]=(uint8_t)(AS608ADDR>>8);
	data[index++]=(uint8_t)(AS608ADDR);
	data[index++]=type;
	data[index++]=(lenth)>>8;
	data[index++]=(lenth);
	memcpy(&data[index],cmd,lenth);
	for(i=0;i<lenth+index;i++){
		printf("%X ",data[i]);
	}
	printf("\r\n");
	if(UART5_SendBytes(data,index+lenth)==0){
		return 1;
	}else{
		return 0;
	}
}

/***************************************************************
  *	@brief	̽����ָ��̽�⵽��¼��ָ��ͼ����� ImageBuffer
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_GetImage(){
	uint8_t data[3],rec[12];
	data[0]=0x01;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		delay_ms(1000);
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�� ImageBuffer �е�ԭʼͼ������ָ����
  *	@param	BufferID 0x01,0x02
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_GenChar(uint8_t BufferID){
	uint8_t data[3+1],rec[12];
	data[0]=0x02;
	data[1]=BufferID;
	data[2]=(data[0]+data[1]+4+CMD_PACKAGE)>>8;
	data[3]=(data[0]+data[1]+4+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,4)){
		delay_ms(1000);
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��ȷ�ȶ� CharBuffer1 �� CharBuffer2 �е�
  *	@param	score ƥ��÷ִ�ŵ�ַ
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_Match(uint16_t	*score){
	uint8_t data[3],rec[12+2];
	data[0]=0x03;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*score=((uint16_t)rec[10]<<8)|rec[11];
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�� CharBuffer1 �� CharBuffer2 �е������ļ���������
  *	@param	buffer ������0x01 0x02
	* @param	StartPage ��ʼҳ
	*	@param	PageNum	���ҵ�ҳ��
	*	@param	*PageID �ҵ���ҳ��
	*	@param	*MatchScore ָ��ƥ��ĵ÷���
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_Search(uint8_t buffer,uint16_t StartPage,uint16_t PageNum,uint16_t *PageID,uint16_t *MatchScore){
	uint8_t data[3+5],rec[12+4];
	data[0]=0x04;
	data[1]=buffer;
	data[2]=StartPage>>8;
	data[3]=StartPage;
	data[4]=PageNum>>8;
	data[5]=PageNum;
	data[6]=(data[0]+data[1]+data[2]+data[3]+data[4]+data[5]+8+CMD_PACKAGE)>>8;
	data[7]=data[0]+data[1]+data[2]+data[3]+data[4]+data[5]+8+CMD_PACKAGE;
	if(PS_SendCmd(data,CMD_PACKAGE,8)){
		if(PS_GetAckData(rec,1000)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*MatchScore=((uint16_t)rec[12]<<8)|rec[13];
				*PageID=((uint16_t)rec[10]<<8)|rec[11];
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�� CharBuffer1 �� CharBuffer2 �е������ļ��ϲ����� 
						ģ�壬������� CharBuffer1 �� CharBuffer2��
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_RegModel(){
	uint8_t data[3],rec[12];
	data[0]=0x05;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID
						�� flash ���ݿ�λ�á�
  *	@param	BufferID 0x01 0x02 ���������
	* @param 	PageID λ��ҳ
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID){
	uint8_t data[3+3],rec[12];
	data[0]=0x06;
	data[1]=BufferID;
	data[2]=PageID>>8;
	data[3]=PageID;
	data[4]=(data[0]+data[1]+data[2]+data[3]+6+CMD_PACKAGE)>>8;
	data[5]=(data[0]+data[1]+data[2]+data[3]+6+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,6)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				AS608_Info.Store[PageID]=1;
				AS608_Info.ValidNum+=1;
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�� flash ���ݿ���ָ�� ID �ŵ�ָ��ģ����뵽ģ�建��
						�� CharBuffer1 �� CharBuffer2
  *	@param	BufferID �������� 0x01 0x02
	*	@param	PageID ָ�ƿ�ģ���
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_LoadChar(uint8_t BufferID,uint16_t PageID){
	uint8_t data[3+3],rec[12];
	data[0]=0x07;
	data[1]=BufferID;
	data[2]=PageID>>8;
	data[3]=PageID;
	data[4]=(data[0]+data[1]+data[2]+data[3]+6+CMD_PACKAGE)>>8;
	data[5]=(data[0]+data[1]+data[2]+data[3]+6+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,6)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�������������е������ļ��ϴ�����λ��
  *	@param	BufferID
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_UpChar(uint8_t BufferID,uint8_t *chardata){
	uint8_t data[4],rec[12];
	data[0]=0x08;
	data[1]=BufferID;
	data[2]=(data[0]+data[1]+4+CMD_PACKAGE)>>8;
	data[3]=(data[0]+data[1]+4+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,4)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				/*
				��д��������
				*/
				if(PS_GetAckData(chardata,300)==DATA_PACKAGE){
						
				}
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��λ�����������ļ���ģ���һ������������
  *	@param	BufferID
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_DownChar(uint8_t BufferID,uint8_t *chardata){
	uint8_t data[4],rec[12];
	data[0]=0x09;
	data[1]=BufferID;
	data[2]=(data[0]+data[1]+4+CMD_PACKAGE)>>8;
	data[3]=(data[0]+data[1]+4+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,4)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(AS608_GetAck(rec[9])==AS608_ACK_OK){
				/*
				��д��������
				*/
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��ͼ�񻺳����е������ϴ�����λ��
  *	@param	*iamge
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_UpImage(uint8_t *iamge){
	uint8_t data[3],rec[12];
	data[0]=0x0a;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				//��д���ݴ���
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��λ������ͼ�����ݸ�ģ��
  *	@param	iamge
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_DownImage(uint8_t iamge){
	uint8_t data[3],rec[12];
	data[0]=0x0b;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				//��л���ݴ���
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	ɾ�� flash ���ݿ���ָ�� ID �ſ�ʼ�� N ��ָ��ģ��
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint16_t PS_DeletChar(uint16_t PageID,uint16_t N){
	uint8_t data[3+4],rec[12];
	data[0]=0x0c;
	data[1]=PageID>>8;
	data[2]=PageID;
	data[3]=N>>8;
	data[4]=N;
	data[5]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE)>>8;
	data[6]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,7
		)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				memset(&AS608_Info.Store[PageID],0,N);
				AS608_Info.ValidNum-=N;
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	ɾ�� flash ���ݿ�������ָ��ģ��
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_Empty(){
	
	uint8_t data[3],rec[12];
	data[0]=0x0d;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				memset(AS608_Info.Store,0,300);
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	дģ��Ĵ���
  *	@param	regId �Ĵ������ 4~6
	*	@param	content Ҫ�޸ĵ�����	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_WriteReg(uint8_t regId,uint8_t content){
	uint8_t data[3+2],rec[12];
	data[0]=0x0e;
	data[1]=regId;
	data[2]=content;
	data[3]=(data[0]+data[1]+data[2]+5+CMD_PACKAGE)>>8;
	data[4]=(data[0]+data[1]+data[2]+5+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,5)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��ȡģ��Ļ��������������ʣ�����С�ȣ���
  *	@param	rece 16���ֽڵ����ݴ洢
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_ReadSysPara(){
	uint8_t data[3],rec[12+16];
	data[0]=0x0f;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				AS608_Info.sys_reg=(rec[10]<<8)|rec[11];
				AS608_Info.sensor_type=(rec[12]<<8)|rec[13];
				AS608_Info.store_size=(rec[14]<<8)|rec[15];
				AS608_Info.safe=(rec[16]<<8)|rec[17];
				AS608_Info.addr=(rec[18]<<24)|(rec[19]<<16)|(rec[20]<<8)|rec[21];
				AS608_Info.data_size=(rec[22]<<8)|rec[23];
				switch(AS608_Info.data_size){
					case 0:AS608_Info.data_size=32;break;
					case 1:AS608_Info.data_size=64;break;
					case 2:AS608_Info.data_size=128;break;
					case 3:AS608_Info.data_size=256;break;
				}
				AS608_Info.baudrate=(rec[24]<<8)|rec[25];
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�ɼ�һ��ָ��ע��ģ�壬��ָ�ƿ���������λ���洢�����ش洢 ID
  *	@param	PageID �洢��id��
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_Enroll(uint16_t *PageID){
	uint8_t data[3],rec[12+2];
	data[0]=0x10;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		delay_ms(1000);
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*PageID=((uint16_t)rec[10]<<8)|rec[11];
				AS608_Info.Store[*PageID]=1;
				AS608_Info.ValidNum+=1;
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�Զ��ɼ�ָ�ƣ���ָ�ƿ�������Ŀ��ģ�岢������������� �� 
						���Ŀ��ģ��ͬ��ǰ�ɼ���ָ�ƱȶԵ÷ִ�����߷�ֵ��
						����Ŀ��ģ�� Ϊ�������������Բɼ�����������Ŀ��ģ
						��Ŀհ�����
  *	@param	PageID ƥ���id��
	*	@param	MathScore ƥ��ķ���
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_Identify(uint16_t *PageID,uint16_t *MathScore){
	uint8_t data[3],rec[12+4];
	data[0]=0x11;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		delay_ms(1000);
		if(PS_GetAckData(rec,1000)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*PageID=((uint16_t)rec[10]<<8)|rec[11];
				*MathScore=((uint16_t)rec[12]<<8)|rec[13];
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	����ģ�����ֿ���
  *	@param	PassWord ��Ҫ���õĿ���
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_SetPwd(uint32_t PassWord){
	uint8_t data[3+4],rec[12+4];
	data[0]=0x12;
	data[1]=PassWord>>24;
	data[2]=PassWord>>16;
	data[3]=PassWord>>8;
	data[4]=PassWord;
	data[5]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE)>>8;
	data[6]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,7)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��֤ģ�����ֿ���
  *	@param	PassWord ��֤�Ŀ���
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_VfyPwd(uint32_t PassWord){
	uint8_t data[3+4],rec[12+4];
	data[0]=0x13;
	data[1]=PassWord>>24;
	data[2]=PassWord>>16;
	data[3]=PassWord>>8;
	data[4]=PassWord;
	data[5]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE)>>8;
	data[6]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,7)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��оƬ����һ������������ظ���λ��
  *	@param	randomnum �����������
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_GetRandomCode(uint32_t *randomnum){
	uint8_t data[3],rec[12+4];
	data[0]=0x14;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*randomnum=((uint32_t)rec[10]<<24)|((uint32_t)rec[11]<<16)|\
				((uint32_t)rec[12]<<8)|((uint32_t)rec[12]);
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	����оƬ��ַ
  *	@param	addr ��Ҫ���õĵ�ַ
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_SetChipAddr(uint32_t addr){
	uint8_t data[3+4],rec[12+4];
	data[0]=0x15;
	data[1]=addr>>24;
	data[2]=addr>>16;
	data[3]=addr>>8;
	data[4]=addr;
	data[5]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE)>>8;
	data[6]=(data[0]+data[1]+data[2]+data[3]+data[4]+7+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��ȡ FLASH Information Page ���ڵ���Ϣҳ(512bytes)
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_ReadINFpage(uint8_t *rece){
	uint8_t data[3],rec[12];
	data[0]=0x16;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				//���ݴ���
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	���� UART Э�飬������� USB ͨѶ�˿ڽ��п��ؿ���
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_Port_Control(uint8_t control){
	uint8_t data[3+1],rec[12];
	data[0]=0x17;
	data[1]=control;
	data[2]=(data[0]+data[1]+4+CMD_PACKAGE)>>8;
	data[3]=(data[0]+data[1]+4+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	ģ���ڲ�Ϊ�û������� 256bytes �� FLASH �ռ����ڴ��
						�û��� �ݣ��ô洢�ռ��Ϊ�û����±����ü��±��߼���
						���ֳ� 16 ��ҳ��д���� ����������д���û��� 32bytes 
						���ݵ�ָ���ļ��±�ҳ��
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_WriteNotepad(uint8_t userpageid,uint8_t *userdata){
	uint8_t data[3+33],rec[12];
	uint16_t temp=0,i;
	data[0]=0x18;
	data[1]=userpageid;
	memcpy(&data[2],userdata,32);
	for(i=0;i<34;i++){
		temp+=data[i];
	}
	data[34]=temp>>8;
	data[35]=temp;
	if(PS_SendCmd(data,CMD_PACKAGE,36)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){	
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��ȡ FLASH �û����� 128bytes ����
  *	@param	userpageid 0~15
	*	@param	rece ��Ŷ�ȡ������
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_ReadNotepad(uint8_t userpageid,uint8_t *rece){
	uint8_t data[3+1],rec[12];
	data[0]=0x19;
	data[1]=userpageid;
	data[2]=(data[0]+data[1]+4+CMD_PACKAGE)>>8;
	data[3]=(data[0]+data[1]+4+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,4)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				memcpy(rece,&rec[10],32);
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�� CharBuffer1 �� CharBuffer2 �е������ļ�����������
						���򲿷�ָ�� �⡣�����������򷵻�ҳ�롣
�� 					��ָ����ڵ�ȷ������ָ�ƿ��У��ҵ�¼ʱ�����ܺõ�ָ�ƣ�
						��ܿ�� �����������
  *	@param	
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,uint16_t *PageID,uint16_t *MatchScore){
	uint8_t data[3+5],rec[12+4];
	data[0]=0x1b;
	data[1]=BufferID;
	data[2]=StartPage>>8;
	data[3]=StartPage;
	data[4]=PageNum>>8;
	data[5]=PageNum;
	data[6]=(data[0]+data[1]+data[2]+data[3]+data[4]+data[5]+8)>>8;
	data[7]=data[0]+data[1]+data[2]+data[3]+data[4]+data[5]+8;
	if(PS_SendCmd(data,CMD_PACKAGE,8)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*MatchScore=((uint16_t)rec[12]<<8)|rec[13];
				*PageID=((uint16_t)rec[10]<<8)|rec[11];
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	��ͼ�񻺳����е�ָ��ͼ����д�������ϸ��ָ��ͼ��
  *	@param	BinImgTpye 0����ֵ��ͼ��1�������������ͼ��
												2�����������ĵ�ͼ��
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_GenBinImage(uint8_t BinImgTpye){
	uint8_t data[3+1],rec[12];
	data[0]=0x1c;
	data[1]=BinImgTpye;
	data[2]=(data[0]+data[1]+4+CMD_PACKAGE)>>8;
	data[3]=(data[0]+data[1]+4+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,4)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	����Чģ�����
  *	@param	ValidN ��Ч����
  *	@retval	uint8_t
						0��ʧ��
						1���ɹ�
****************************************************************/
uint8_t PS_ValidTempleteNum(uint16_t *ValidN){
	uint8_t data[3],rec[12+2];
	data[0]=0x1d;
	data[1]=(data[0]+3+CMD_PACKAGE)>>8;
	data[2]=(data[0]+3+CMD_PACKAGE);
	if(PS_SendCmd(data,CMD_PACKAGE,3)){
		if(PS_GetAckData(rec,100)==ACK_PACKAGE){
			if(rec[9]==AS608_ACK_OK){
				*ValidN=((uint16_t)rec[10]<<8)|(rec[11]);
				AS608_Info.ValidNum=*ValidN;
				return 1;
			}
		}
	}
	return 0;
}
/***************************************************************
  *	@brief	�������ݰ���С����Ҫ������Ч
  *	@param	size 0��32�ֽ� 1��64�ֽ�  2��128�ֽ� 3��256�ֽ�
  *	@retval	uint8_t
										1���ɹ�
										0��ʧ��
****************************************************************/
uint8_t AS608_SetDataSize(uint8_t size){
	if(PS_WriteReg(6,size)){
		AS608_Info.data_size=size;
		return 1;
	}
	return 0;
}
/***************************************************************
  *	@brief	���öԱ���ֵ������˵�ǰ�ȫ�ȼ�����Ҫ������Ч
  *	@param	level	1~5
  *	@retval	uint8_t
										1���ɹ�
										0��ʧ��
****************************************************************/
uint8_t AS608_SetCompareValue(uint8_t leve1){
	if(PS_WriteReg(5,leve1)){
		AS608_Info.safe=leve1;
		return 1;
	}
	return 0;
}
/***************************************************************
  *	@brief	���ô��ڵĲ����ʣ�����Ϊ9600������������Ҫ������Ч
  *	@param	baudrate ����Ϊ9600��������
  *	@retval	uint8_t
										1���ɹ�
										0��ʧ��
****************************************************************/
uint8_t AS608_SetBaudRate(uint32_t baudrate){
	if(PS_WriteReg(4,(uint8_t)(baudrate/9600))){
		AS608_Info.baudrate=baudrate;
		return 1;
	}
	return 0;
}
/***************************************************************
  *	@brief	����demo
  *	@param	void
  *	@retval	void
****************************************************************/
void AS608_Demo(void){
	uint16_t temp,soc;
	uint8_t rec[512];
	if(PS_DeletChar(1,1)){
		printf("ɾ��ָ�Ƴɹ�\r\n");
	}
	if(PS_GetImage()){
		printf("¼��ָ��ͼ��ɹ�\r\n");
	}else{
		printf("¼��ָ��ͼ��ʧ��\r\n");
	}
	if(PS_GenChar(1)){
		printf("��������ֵ�ɹ�\r\n");
		if(PS_UpChar(1,rec)){
			printf("����ֵ�ϴ��ɹ�\r\n");
		}else{
			printf("����ֵ�ϴ�ʧ��\r\n");
		}
	}else{
		printf("��������ֵʧ��\r\n");
	}
	
	if(PS_Search(1,0,8,&temp,&soc)){
		printf("%d %d �����ɹ�\r\n",temp,soc);
	}else{
			if(PS_Search(1,0,8,&temp,&soc)){
				printf("%d %d �����ɹ�\r\n",temp,soc);
				
			}else{
					printf("����ʧ��\r\n");
					if(PS_Enroll(&temp)){
						printf("%d �Զ�¼��ָ�Ƴɹ�\r\n",temp);
					}
			}
	}
	
	/*
	if(PS_Identify(&temp,&soc)){//���ָ�ƴ���
		printf("%d %d �Զ������ɹ�\r\n",temp,soc);
	}*/
	/*if(PS_DeletChar(1,1)){
		printf("ɾ��ָ�Ƴɹ�\r\n");
	}*/
}
/***************************************************************
  *	@brief	AS608���io�����裬���ݳ�ʼ��
  *	@param	void
  *	@retval	void
****************************************************************/
void AS608_Init(void){
	uint16_t temp;
	if(PS_VfyPwd(0)){
		printf("���ֳɹ�\r\n");
	}
	/*
	if(AS608_SetDataSize(3)){
		printf("�������ݰ���С�ɹ�\r\n");
	}*/
	/*if(PS_Empty()){
		printf("���Flash�ɹ�\r\n");
	}*/
	if(PS_ValidTempleteNum(&temp)){
		printf("��ȡ��Чģ�����ɹ�\r\n");
	}
	if(PS_ReadSysPara()){
		printf("��ȡоƬ��Ϣ�ɹ�\r\n");
		printf("ϵͳ״̬�Ĵ�����%x\r\n",AS608_Info.sys_reg);
		printf("���������ͣ�%d\r\n",AS608_Info.sensor_type);
		printf("ָ�ƿ��С:%d\r\n",AS608_Info.store_size);
		printf("��ȫ�ȼ���%d\r\n",AS608_Info.safe);
		printf("�豸��ַ��0x%X\r\n",AS608_Info.addr);
		printf("���ݰ���С��%d\r\n",AS608_Info.data_size);
		printf("�����ʣ�%d\r\n",AS608_Info.baudrate*9600);
		printf("��Чָ���� ��%d\r\n",AS608_Info.ValidNum);
	}
}

