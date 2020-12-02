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

	// 打开串口,成功返回true，失败返回false
	
	// synchronizeflag(同步、异步,仅适用与windows): 0为异步，1为同步
	bool open();

	//关闭串口，参数待定
	void close();

	//发送数据或写数据，成功返回发送数据长度，失败返回0
	int send(const void *buf, int len);

	//接受数据或读数据，成功返回读取实际数据的长度，失败返回0
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