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
#include<string>
#include<set>

#include <windows.h>

#define NAME_LEN 100

using namespace std;

class WzSerialPort
{
public:
	WzSerialPort();
	~WzSerialPort();

	// �򿪴���,�ɹ�����true��ʧ�ܷ���false
	bool open();

	//�رմ��ڣ���������
	void close();

	//�������ݻ�д���ݣ��ɹ����ط������ݳ��ȣ�ʧ�ܷ���0
	int send(const void *buf, int len);

	//�������ݻ�����ݣ��ɹ����ض�ȡʵ�����ݵĳ��ȣ�ʧ�ܷ���0
	int receive(void *buf, int maxlen);

	void AvailableCOM(set<string>& myset);//COM���ж�

	int pHandle[16];//���洮�ں�

	SelportParameters lpconfigport;//���ڲ����ṹ��

	DCB p;

	COMMTIMEOUTS TimeOuts;


private:
	int synchronizeflag = 1;
};
#endif