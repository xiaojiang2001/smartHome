/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	onenet.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸
#include "M5310.h"

//Э���ļ�
#include "onenet.h"
#include "mqttkit.h"

//Ӳ������
#include "usart.h"
#include "delay.h"
//#include "adxl362.h"
#include "led.h"

//C��
#include <string.h>
#include <stdio.h>

//key    FbsV1uMCUIw4+UTzftB9pLjCz4cKIiVr1WZ0dUhZU58=
#define PROID		"454469"					//��ƷID
#define AUTH_INFO	"NBhome666"			//��Ȩ��Ϣ
#define DEVID		"770099829"				//�豸ID

//�ⲿ���ݼ�
extern unsigned char buf[128];
extern u8 tempH;				//�¶���������
extern u8 tempL;				//�¶�С������
extern u8 humiH;				//ʪ����������
extern u8 humiL;				//ʪ��С������
extern float light;
extern float smoke;			// smoke 0-100%
extern uint32_t car;		//ͣ������
extern u8 Fire_flag;
//==========================================================
//	�������ƣ�	OneNet_DevLink
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_DevLink(void)
{
	//Э���
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					
	unsigned char *dataPtr;
	_Bool status = 1;
	UsartPrintf(USART_DEBUG, "OneNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0){
		M5310_SendData(mqttPacket._data, mqttPacket._len);	 //�ϴ�ƽ̨
		dataPtr = M5310_GetIPD(300);					//�ȴ�ƽ̨��Ӧ250
		if(dataPtr == NULL)    								//���dataPtrΪ��
			printf("dataPtr�ǿյ�\r\n");
		//���dataPtr��Ϊ��
		if(dataPtr != NULL){
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK){
				switch(MQTT_UnPacketConnectAck(dataPtr)){
					case 0:UsartPrintf(USART_DEBUG, "Tips:	���ӳɹ�\r\n");status = 0;break;
					case 1:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:UsartPrintf(USART_DEBUG, "WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;
					default:UsartPrintf(USART_DEBUG, "ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
		UsartPrintf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	return status;
}


//==========================================================
//	�������ƣ�	OneNet_Ping
//
//	�������ܣ�	��onenet��������
//
//	��ڲ�����	��
//
//	���ز�����	1-�ɹ�	0-ʧ��
//
//	˵����		��onenetƽ̨��������
//==========================================================
_Bool OneNet_Ping(void)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
//	MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	if(MQTT_PacketPing(&mqttPacket)== 0)
	{
		M5310_SendData(mqttPacket._data, mqttPacket._len);				//�ϴ�ƽ̨
		dataPtr = M5310_GetIPD(250);									//�ȴ�ƽ̨��Ӧ250
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_PINGRESP)
			{
				status=0;
			}
		}
		else
		{
			return status;
		}
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��		
	}
	return status;
}

unsigned char OneNet_FillBuf1(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));
	strcpy(buf, "{\"datastreams\":[");
	
	//��ʪ��
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"id\":\"temperature\",\"datapoints\":[{\"value\":%0.1f}]},", tempH+tempL*0.1);
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"id\":\"humidity\",\"datapoints\":[{\"value\":%0.1f}]},", humiH+humiL*0.1);
	strcat(buf, text);
	
	//����
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"id\":\"light\",\"datapoints\":[{\"value\":%0.1f}]},", light);
	strcat(buf, text);
	
	//����
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"id\":\"smoke\",\"datapoints\":[{\"value\":%0.2f}]}", smoke);
	strcat(buf, text);
	
	strcat(buf, "]}");
	return strlen(buf);
}

unsigned char OneNet_FillBuf2(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));
	strcpy(buf, "{\"datastreams\":[");
	
	//ͣ����
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"id\":\"car\",\"datapoints\":[{\"value\":%d}]}", car);
	strcat(buf, text);
	
	strcat(buf, "]}");
	return strlen(buf);
}

unsigned char OneNet_FillLocationBuf(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));
	
	//��γ����Ϣ
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":106.613922,\"lat\":29.53832}}]}]}");
	strcat(buf, text);
	
	return strlen(buf);
}

unsigned char OneNet_FillWarningBuf(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));
	strcpy(buf, "{\"datastreams\":[");
	
	//�쳣����
	memset(text, 0, sizeof(text));
	sprintf(text, "{\"id\":\"fire\",\"datapoints\":[{\"value\":%d}]}", Fire_flag);
	strcat(buf, text);
	
	strcat(buf, "]}");
	return strlen(buf);
}

//==========================================================
//	�������ƣ�	OneNet_SendData
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//	��ڲ�����	type���������ݵĸ�ʽ
//	���ز�����	��	
//==========================================================
void OneNet_SendData(void)
{
	static u8 cnt = 0 ;
	//Э���
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												
	char buf[512];
	short body_len = 0, i = 0;
	//UsartPrintf(USART_DEBUG, "Tips:	OneNet_SendData-MQTT\r\n");
	memset(buf, 0, sizeof(buf));
	
	
  //��ȡ��ǰ��Ҫ���͵����������ܳ���
	if(Fire_flag == 1)
	{
		body_len = OneNet_FillWarningBuf(buf);
		//UsartPrintf(USART_DEBUG, "OneNet_SendData_FillWarningBuf	 body_len = %d \r\n",body_len);			
	}
	else
	{
		switch(cnt % 3)
		{
			case 0:	
				body_len = OneNet_FillBuf1(buf);
				//UsartPrintf(USART_DEBUG, "OneNet_SendData_FillBuf1		 body_len = %d \r\n",body_len);			
			break;
			
			case 1:	
				body_len = OneNet_FillBuf2(buf);	
				//UsartPrintf(USART_DEBUG, "OneNet_SendData-FillBuf2		 body_len = %d \r\n",body_len);					
			break;
			
			case 2:	
				body_len = OneNet_FillLocationBuf(buf);	
				//UsartPrintf(USART_DEBUG, "OneNet_SendData_LocationBuf	 	body_len = %d \r\n",body_len);			
			break;
			default:	break;
		}		
		cnt++;
	}
	//printf("body_len:%d \r\n",body_len);
	if(body_len)
	{
		//���
		if(MQTT_PacketSaveData(DEVID, body_len, NULL, 1, &mqttPacket) == 0)						
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			//�ϴ����ݵ�ƽ̨
			M5310_SendData(mqttPacket._data, mqttPacket._len);										
			//UsartPrintf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//ɾ��
		}
		else
			UsartPrintf(USART_DEBUG, "WARN:	MQTT_NewBuffer Failed\r\n");
	}
	memset(buf,0,512);
}

//==========================================================
//	�������ƣ�	OneNet_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	unsigned short req_len = 0;
	unsigned char type = 0;
	short result = 0;
	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
			case MQTT_PKT_CMD:															//�����·�
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//���topic����Ϣ��
			if(result == 0)
			{
					UsartPrintf(USART_DEBUG, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
					if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
					{
						UsartPrintf(USART_DEBUG, "Tips:	Send CmdResp\r\n");
						M5310_SendData(mqttPacket._data, mqttPacket._len);		//�ظ�����
						MQTT_DeleteBuffer(&mqttPacket);				//ɾ��
					}
			}     break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
			if(MQTT_UnPacketPublishAck(cmd) == 0)
						UsartPrintf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");		break;
		default:	result = -1;	break;
	}
	M5310_Clear();										//��ջ���
	if(result == -1)
			return;
	//printf("get cmd: %s\r\n",req_payload);
	dataPtr = strchr(req_payload, 'D');					//����'D'
	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
			dataPtr++;
			while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
			{
				numBuf[num++] = *dataPtr++;
			}
			numBuf[num] = 0;
			num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
	}
	printf("number is:%d\r\n",num);
	switch(num)
	{
		// �û�1��ƹ�
		case 11:	Usart_SendString(USART3, (unsigned char *)"openLED1",8);	break;
		case 10:	Usart_SendString(USART3, (unsigned char *)"closeLED1",9);	break;
		//�û�2��ƹ�
		case 21:	Usart_SendString(USART3, (unsigned char *)"openLED2",8);	break;
		case 20:	Usart_SendString(USART3, (unsigned char *)"closeLED2",9);	break;
		// ·��
		case 31:	roadLED1 = 0; 	break;
		case 30:	roadLED1 = 1; 	break;
		case 41:	roadLED2 = 0; 	break;
		case 40:	roadLED2 = 1; 	break;
		
		//�칫 + ����
		case 51:	led1=0; 	break;
		case 50:	led1=1;		break;
		case 61:	led2=0;		break;
		case 60:	led2=1;		break;
		case 71:	led3=0; 	break;
		case 70:	led3=1;		break;
		case 81:	lock=0; 	break;
		case 80:	lock=1;		break;	
	}

	
	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
