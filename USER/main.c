#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "onenet.h"
#include "M5310.h"
#include <string.h>
#include "exti.h"
#include "RC522.h"
#include "tim.h"
#include "dht11.h"
#include "bh1750.h"
#include "adc.h"

//��������
uint8_t Card_Type1[2]; 
uint16_t system_time=0;
uint8_t lockflag=0;
//Environmet data
u8 tempH;						//�¶���������
u8 tempL;						//�¶�С������
u8 humiH;						//ʪ����������
u8 humiL;						//ʪ��С������
uint32_t val;				// adc val
float smoke;				// smoke 0-100%
float light;				//����
uint32_t car = 1000;//ͣ������λ
char pub_buf[256];	//�������ݵ�buf
u8 Fire_flag=0;

int main(void)
{	
	unsigned int timeCount1 = 0;	//���ͼ������
	unsigned int timeCount2 = 0;	//���ͼ������
	unsigned int pingCount = 0;	//���ͼ������
	u8 i;
	unsigned char *dataPtr = NULL;
	//RFID Card ID
	Card_Type1[0]=0x04;
	Card_Type1[1]=0x00;
	//System Init
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Usart1_init(115200);	 //���ڳ�ʼ��Ϊ115200
	Usart2_Init(115200);	//����2������SIM800C��
	Usart3_Init(9600);		//����3��������ݮ��ͨ��	
	TIM4_Int_Init(1000-1,72-1);			//�ж�1ms
	EXTIX_Init();
	//ģ��Init
 	Command_GPIO_Init();	//ͨ��GPIO��ʼ��
	beep=1;
	DHT11_Init();			//��ʼ��dht11��ʪ�ȴ�����
	BH1750_Init();		//��ʼ��bh1750���մ�����
	ADC_INIT();				// MQ2��ʼ��
	RC522_IO_Init();		//RC522��ʼ��
	PcdReset(); 			//��λRC522
	printf("Hardware init OK\r\n");
	delay_ms(100);

	//��ʼ��M5310�����������
//-----------------------����OneNET�Ʒ�����-----------------------------//			
	M5310_Init();
	while(OneNet_DevLink())			//����OneNET
		delay_ms(300);

	//�����������ʾ
	beep=0;
	delay_ms(300);
	beep=1;
	printf("�ɹ����������\r\n");
	while(1)			//һ��while���50ms
	{	
//-----------------------���ݲɼ�5s-----------------------------//			
		if(++timeCount1 >= 10)			//1s
		{
			DHT11_Read_Data(&humiH,&humiL,&tempH,&tempL);
			if(!i2c_CheckDevice(BH1750_Addr))
				light=LIght_Intensity();
			val = get_adc_val();					//�������һ��ADC1�������ת�����
			smoke = val * 100.0 / 4096;		//��һ��
			//printf("temp:%d.%d  humi:%d.%d light:%.2f lux    smoke: %.2f\r\n",tempH,tempL,humiH,humiL,light,smoke);
			timeCount1=0;
		}	

//-----------------------���������ͼ�� 50s-----------------------------//				
		if(++pingCount >= 1200)			//60s		
		{
			if(OneNet_Ping()==0)
				printf("�ѳɹ�����һ����������\r\n");
			pingCount=0;
		}
//-----------------------�����ϴ� ��� 5s-----------------------------//		
		if(++timeCount2 >= 30)			//2s
		{
			OneNet_SendData();									//��������
			//UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			timeCount2 = 0;
			M5310_Clear();
		}		
//-----------------------�����·�ָ��-----------------------------//
		dataPtr = M5310_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		delay_ms(10);
//-----------------------RFID�Ž�ϵͳ-----------------------------//	
//		if(MI_OK==PcdRequest(0x52, Card_Type1))  //Ѱ������������ɹ�����MI_OK  ��ӡ��ο���
//		{
//			uint16_t cardType = (Card_Type1[0]<<8)|Card_Type1[1];//����ͬ��������
//			printf("�����ͣ�(0x%04X)\r\n",cardType);  //"Card Type(0x%04X):"
//			printf("carType=%d\r\n",cardType);
//			if(cardType==1024)
//			{	
//				car--;
//				//��������ʾ
//				for(i=0;i<6;i++)
//				{
//					beep=~beep;
//					delay_ms(250);
//				}
//				lock=0;
//				delay_ms(3000);
//				lock=1;
//			}
//			cardType=0;
//		}
//-----------------------�жϼ�⵽�����ź�-----------------------------//	
//		if(Fire_flag > 1)
//		{
//				OneNet_SendData();
//				UsartPrintf(USART_DEBUG, "OneNet_SendData in fire\r\n");
//				Usart_SendString(USART3, (unsigned char *)"fire",4);		//����ݮ�ɷ��ͻ�������
//				UsartPrintf(USART_DEBUG, "*********fire********\r\n");	
//			 	beep=0;
//				delay_ms(200);
//				beep=1;
//				Fire_flag = 0; 
//		}
	}		
}		
