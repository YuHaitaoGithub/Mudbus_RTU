#include "..\include\Serial_port_API.h"

WzSerialPort::WzSerialPort()
{

}

WzSerialPort::~WzSerialPort()
{

}

bool WzSerialPort::open()
{
	synchronizeflag = 1;
	this->synchronizeflag = synchronizeflag;
	HANDLE hCom = NULL;
	if (this->synchronizeflag)
	{
		//同步方式
		hCom = CreateFileA(lpconfigport.portname, //串口名
			GENERIC_READ | GENERIC_WRITE, //支持读写
			0, //独占方式，串口不支持共享
			NULL,//安全属性指针，默认值为NULL
			OPEN_EXISTING, //打开现有的串口文件
			0, //0：同步方式，FILE_FLAG_OVERLAPPED：异步方式
			NULL);//用于复制文件句柄，默认值为NULL，对串口而言该参数必须置为NULL
	
	}

	if (hCom == (HANDLE)-1)
	{
		return false;
	}

	//配置缓冲区大小 
	if (!SetupComm(hCom, 1024, 1024))
	{
		return false;
	}

	// 配置参数 
	memset(&p, 0, sizeof(p));
	p.DCBlength = sizeof(p);
	p.BaudRate = lpconfigport.baudrate; // 波特率
	p.ByteSize = lpconfigport.databit; // 数据位

	switch (lpconfigport.parity) //校验位
	{
	case 0:
		p.Parity = NOPARITY; //无校验
		break;
	case 1:
		p.Parity = ODDPARITY; //奇校验
		break;
	case 2:
		p.Parity = EVENPARITY; //偶校验
		break;
	case 3:
		p.Parity = MARKPARITY; //标记校验
		break;
	}

	switch (lpconfigport.stopbit) //停止位
	{
	case 1:
		p.StopBits = ONESTOPBIT; //1位停止位
		break;
	case 2:
		p.StopBits = TWOSTOPBITS; //2位停止位
		break;
	case 3:
		p.StopBits = ONE5STOPBITS; //1.5位停止位
		break;
	}

	if (!SetCommState(hCom, &p))
	{
		// 设置参数失败
		return false;
	}

	//GetCommTimeouts(hCom, LpTimeOuts);

	SetCommTimeouts(hCom, &TimeOuts);

	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);//清空串口缓冲区

	memcpy(pHandle, &hCom, sizeof(hCom));// 保存句柄

	return true;
}

void WzSerialPort::close()
{
	HANDLE hCom = *(HANDLE*)pHandle;
	CloseHandle(hCom);
}

int WzSerialPort::send(const void *buf, int len)
{
	HANDLE hCom = *(HANDLE*)pHandle;

		// 同步方式
	DWORD dwBytesWrite = len; //成功写入的数据字节数
	BOOL bWriteStat = WriteFile(hCom, //串口句柄
		buf, //数据首地址
		dwBytesWrite, //要发送的数据字节数
		&dwBytesWrite, //DWORD*，用来接收返回成功发送的数据字节数
		NULL); //NULL为同步发送，OVERLAPPED*为异步发送
	if (!bWriteStat)
	{
		return 0;
	}
	return dwBytesWrite;
}

int WzSerialPort::receive(void *buf, int maxlen)
{
	HANDLE hCom = *(HANDLE*)pHandle;

		//同步方式
	DWORD wCount = maxlen; //成功读取的数据字节数
	BOOL bReadStat = ReadFile(hCom, //串口句柄
		buf, //数据首地址
		wCount, //要读取的数据最大字节数
		&wCount, //DWORD*,用来接收返回成功读取的数据字节数
		NULL); //NULL为同步发送，OVERLAPPED*为异步发送
	if (!bReadStat)
	{
		return 0;
	}
	return wCount;
	
}


void WzSerialPort::AvailableCOM(set<int>& myset)
{
	int iCOM = 20;
	for (int i = 0; i <= iCOM; i++)
	{
		char cTemp[MAX_PATH];
		char cTempFull[MAX_PATH];
		HANDLE hCom1; //全局变量，串口句柄 
		sprintf_s(cTemp, "COM%d", i);
		sprintf_s(cTempFull, "\\\\.\\COM%d", i);
		hCom1 = CreateFileA(cTempFull, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCom1 == (HANDLE)-1)
			continue;
		else
		{
			myset.insert(i);
			printf("%s ", cTemp);
		}
		CloseHandle(hCom1);
	}
	printf("\n");
}


