#ifndef _WZSERIALPORT_H
#define _WZSERIALPORT_H

typedef struct SelportParameters{
	char* portname;
	int baudrate;
	char parity;
	char databit;
	char stopbit;
};

#include <stdio.h>
#include <string.h>

#include <WinSock2.h>
#include <windows.h>


class WzSerialPort
{
public:
	WzSerialPort();
	~WzSerialPort();

	// �򿪴���,�ɹ�����true��ʧ�ܷ���false
	
	// synchronizeflag(ͬ�����첽,��������windows): 0Ϊ�첽��1Ϊͬ��
	bool open();

	//�رմ��ڣ���������
	void close();

	//�������ݻ�д���ݣ��ɹ����ط������ݳ��ȣ�ʧ�ܷ���0
	int send(const void *buf, int len);

	//�������ݻ�����ݣ��ɹ����ض�ȡʵ�����ݵĳ��ȣ�ʧ�ܷ���0
	int receive(void *buf, int maxlen);

	void AvailableCOM();

	int pHandle[16];

	SelportParameters lpconfigport;

	DCB p;

	COMMTIMEOUTS TimeOuts;


private:
	int synchronizeflag = 1;
};
#endif