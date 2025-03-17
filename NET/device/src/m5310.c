/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	M5310.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-10-20
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		M5310����
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//�����豸����
#include "M5310.h"

//Ӳ������
#include "delay.h"
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ESP8266_WIFI_INFO		"AT+CWJAP=\"OpenWrt\",\"12345679\"\r\n"
#define M5310_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",6002\r\n"

unsigned char M5310_buf[128];
unsigned short M5310_cnt = 0, M5310_cntPre = 0;

//==========================================================
//	�������ƣ�	M5310_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void M5310_Clear(void)
{
	memset(M5310_buf, 0, sizeof(M5310_buf));
	M5310_cnt = 0;

}

//==========================================================
//	�������ƣ�	M5310_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool M5310_WaitRecive(void)
{

	if(M5310_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(M5310_cnt == M5310_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		M5310_cnt = 0;							//��0���ռ���
			
		return REV_OK;							//���ؽ�����ɱ�־
	}
		
	M5310_cntPre = M5310_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;							//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	M5310_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool M5310_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(M5310_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)M5310_buf, res) != NULL)		//����������ؼ���
			{
				M5310_Clear();										//��ջ���
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	M5310_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void M5310_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	M5310_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);	//��������
	if(!M5310_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		Usart_SendString(USART2, data, len);	//�����豸������������
	}

}

//==========================================================
//	�������ƣ�	M5310_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	timeOut�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
// ��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *M5310_GetIPD(unsigned short timeOut)
{

// 8266
/*
	char *ptrIPD = NULL;
	
	do
	{
		if(M5310_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)M5310_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		delay_ms(5);		//��ʱ�ȴ�
	} while(timeOut--);
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��
*/
// SIM808

	char *ptrIPD;
	do
	{
		if(M5310_WaitRecive() == REV_OK)								//����������
		{
				//ptrIPD = strstr((char *)M5310_buf, "+IPD,");				//������IPDATA��ͷ
			ptrIPD = strstr((char *)M5310_buf, "IPD,");				//������IPDATA��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDATAͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				//ptrIPD = strchr(ptrIPD, '\n');							//�ҵ�'\n'
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
			}
		}
		delay_ms(10);													//��ʱ�ȴ�
	} while(timeOut--);
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��
}

//==========================================================
//	�������ƣ�	M5310_Init
//	�������ܣ�	��ʼ��M5310
//	��ڲ�����	��
//	���ز�����	��
//==========================================================
void M5310_Init(void)
{
	
//8266
	M5310_Clear();
	
	UsartPrintf(USART_DEBUG, "0. AT\r\n");
	while(M5310_SendCmd("AT\r\n", "OK"))
		delay_ms(250);
	
	UsartPrintf(USART_DEBUG, "1. RST\r\n");
	M5310_SendCmd("AT+RST\r\n","");
	delay_ms(250);
	M5310_SendCmd("AT+CIPCLOSE\r\n","");
	delay_ms(250);
	
	UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while(M5310_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_ms(250);
	
	UsartPrintf(USART_DEBUG, "3. AT+CWDHCP\r\n");
	while(M5310_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		delay_ms(250);
	
	UsartPrintf(USART_DEBUG, "4. CWJAP\r\n");
	while(M5310_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_ms(250);
	
	UsartPrintf(USART_DEBUG, "5. CIPSTART\r\n");
	while(M5310_SendCmd(M5310_ONENET_INFO, "CONNECT"))
		delay_ms(250);
	
	UsartPrintf(USART_DEBUG, "6. ESP8266 Init OK\r\n");
	
// SIM808
/*
	M5310_Clear();
	UsartPrintf(USART_DEBUG, "1. AT\r\n");
	while(M5310_SendCmd("AT\r\n", "OK"))                 				//ȷ��ģ�������շ�����
		delay_ms(500);	
	
	UsartPrintf(USART_DEBUG, "2. AT+CPIN?\r\n");
	while(M5310_SendCmd("AT+CPIN?\r\n", "+CPIN: READY"))			//ȷ��SIM��PIN�����������READY����ʾ�����ɹ�	 
		delay_ms(500);

	UsartPrintf(USART_DEBUG, "3. AT+CREG?\r\n");
	while(M5310_SendCmd("AT+CREG?\r\n", "0,1"))  
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "4. AT+CGREG?\r\n");       
//	while(M5310_SendCmd("AT+CGREG?\r\n","OK"))					
	while(M5310_SendCmd("AT+CGREG?\r\n","0,1"))							//�������ע��״̬
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "5. AT+CIPSHUT\r\n");
	while(M5310_SendCmd("AT+CIPSHUT\r\n","SHUT OK"))				//�ر��ƶ����� 		
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "6. AT+CGCLASS=\"B\"\r\n");
	while(M5310_SendCmd("AT+CGCLASS=\"B\"\r\n","OK"))				//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ���	
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "7. AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
	while(M5310_SendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK"))		//����PDP������,��������Э��,��������Ϣ		
		delay_ms(500);	
	
	UsartPrintf(USART_DEBUG, "8. AT+CGATT=1\r\n");
	while(M5310_SendCmd("AT+CGATT=1\r\n","OK"))					       				//����GPRSҵ��
		delay_ms(500);

	UsartPrintf(USART_DEBUG, "9. AT+CIPMUX=0\r\n");
	while(M5310_SendCmd("AT+CIPMUX=0\r\n","OK"))					     				//����Ϊ�����ӣ���Ȼƽ̨IP��������
		delay_ms(500);
	
	UsartPrintf(USART_DEBUG, "10. AT+CIPHEAD=1\r\n");
	while(M5310_SendCmd("AT+CIPHEAD=1\r\n","OK"))				              //��ʾIPͷ
		delay_ms(500); 
	
	UsartPrintf(USART_DEBUG, "11. AT+CIPSTART\r\n");
	while(M5310_SendCmd(M5310_ONENET_INFO,"CONNECT"))				
		delay_ms(500);
	UsartPrintf(USART_DEBUG, "12. M5310 Init OK\r\n");
	
*/
}

//==========================================================
//	�������ƣ�	USART2_IRQHandler
//
//	�������ܣ�	����2�շ��ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(M5310_cnt >= sizeof(M5310_buf))	M5310_cnt = 0; //��ֹ���ڱ�ˢ��
		M5310_buf[M5310_cnt++] = USART2->DR;
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
}
