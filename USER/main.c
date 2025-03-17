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

//变量定义
uint8_t Card_Type1[2]; 
uint16_t system_time=0;
uint8_t lockflag=0;
//Environmet data
u8 tempH;						//温度整数部分
u8 tempL;						//温度小数部分
u8 humiH;						//湿度整数部分
u8 humiL;						//湿度小数部分
uint32_t val;				// adc val
float smoke;				// smoke 0-100%
float light;				//光照
uint32_t car = 1000;//停车场车位
char pub_buf[256];	//发送数据的buf
u8 Fire_flag=0;

int main(void)
{	
	unsigned int timeCount1 = 0;	//发送间隔变量
	unsigned int timeCount2 = 0;	//发送间隔变量
	unsigned int pingCount = 0;	//发送间隔变量
	u8 i;
	unsigned char *dataPtr = NULL;
	//RFID Card ID
	Card_Type1[0]=0x04;
	Card_Type1[1]=0x00;
	//System Init
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Usart1_init(115200);	 //串口初始化为115200
	Usart2_Init(115200);	//串口2，驱动SIM800C用
	Usart3_Init(9600);		//串口3，用于树莓派通信	
	TIM4_Int_Init(1000-1,72-1);			//中断1ms
	EXTIX_Init();
	//模块Init
 	Command_GPIO_Init();	//通用GPIO初始化
	beep=1;
	DHT11_Init();			//初始化dht11温湿度传感器
	BH1750_Init();		//初始化bh1750光照传感器
	ADC_INIT();				// MQ2初始化
	RC522_IO_Init();		//RC522初始化
	PcdReset(); 			//复位RC522
	printf("Hardware init OK\r\n");
	delay_ms(100);

	//初始化M5310并接入服务器
//-----------------------连接OneNET云服务器-----------------------------//			
	M5310_Init();
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(300);

	//连入服务器提示
	beep=0;
	delay_ms(300);
	beep=1;
	printf("成功接入服务器\r\n");
	while(1)			//一个while间隔50ms
	{	
//-----------------------数据采集5s-----------------------------//			
		if(++timeCount1 >= 10)			//1s
		{
			DHT11_Read_Data(&humiH,&humiL,&tempH,&tempL);
			if(!i2c_CheckDevice(BH1750_Addr))
				light=LIght_Intensity();
			val = get_adc_val();					//返回最近一次ADC1规则组的转换结果
			smoke = val * 100.0 / 4096;		//归一化
			//printf("temp:%d.%d  humi:%d.%d light:%.2f lux    smoke: %.2f\r\n",tempH,tempL,humiH,humiL,light,smoke);
			timeCount1=0;
		}	

//-----------------------心跳包发送间隔 50s-----------------------------//				
		if(++pingCount >= 1200)			//60s		
		{
			if(OneNet_Ping()==0)
				printf("已成功发送一个心跳包！\r\n");
			pingCount=0;
		}
//-----------------------数据上传 间隔 5s-----------------------------//		
		if(++timeCount2 >= 30)			//2s
		{
			OneNet_SendData();									//发送数据
			//UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			timeCount2 = 0;
			M5310_Clear();
		}		
//-----------------------接收下发指令-----------------------------//
		dataPtr = M5310_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		delay_ms(10);
//-----------------------RFID门禁系统-----------------------------//	
//		if(MI_OK==PcdRequest(0x52, Card_Type1))  //寻卡函数，如果成功返回MI_OK  打印多次卡号
//		{
//			uint16_t cardType = (Card_Type1[0]<<8)|Card_Type1[1];//读不同卡的类型
//			printf("卡类型：(0x%04X)\r\n",cardType);  //"Card Type(0x%04X):"
//			printf("carType=%d\r\n",cardType);
//			if(cardType==1024)
//			{	
//				car--;
//				//蜂鸣器提示
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
//-----------------------中断检测到火灾信号-----------------------------//	
//		if(Fire_flag > 1)
//		{
//				OneNet_SendData();
//				UsartPrintf(USART_DEBUG, "OneNet_SendData in fire\r\n");
//				Usart_SendString(USART3, (unsigned char *)"fire",4);		//给树莓派发送火灾数据
//				UsartPrintf(USART_DEBUG, "*********fire********\r\n");	
//			 	beep=0;
//				delay_ms(200);
//				beep=1;
//				Fire_flag = 0; 
//		}
	}		
}		
