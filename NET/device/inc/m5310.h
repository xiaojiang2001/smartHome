#ifndef _M5310_H_
#define _M5310_H_





#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־


void M5310_Init(void);

void M5310_Clear(void);

void M5310_SendData(unsigned char *data, unsigned short len);

unsigned char *M5310_GetIPD(unsigned short timeOut);


#endif
