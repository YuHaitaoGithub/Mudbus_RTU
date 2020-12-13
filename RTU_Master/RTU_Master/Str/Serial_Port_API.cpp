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
		//ͬ����ʽ
		hCom = CreateFileA(lpconfigport.portname, //������
			GENERIC_READ | GENERIC_WRITE, //֧�ֶ�д
			0, //��ռ��ʽ�����ڲ�֧�ֹ���
			NULL,//��ȫ����ָ�룬Ĭ��ֵΪNULL
			OPEN_EXISTING, //�����еĴ����ļ�
			0, //0��ͬ����ʽ��FILE_FLAG_OVERLAPPED���첽��ʽ
			NULL);//���ڸ����ļ������Ĭ��ֵΪNULL���Դ��ڶ��Ըò���������ΪNULL
	
	}

	if (hCom == (HANDLE)-1)
	{
		return false;
	}

	//���û�������С 
	if (!SetupComm(hCom, 1024, 1024))
	{
		return false;
	}

	// ���ò��� 
	memset(&p, 0, sizeof(p));
	p.DCBlength = sizeof(p);
	p.BaudRate = lpconfigport.baudrate; // ������
	p.ByteSize = lpconfigport.databit; // ����λ

	switch (lpconfigport.parity) //У��λ
	{
	case 0:
		p.Parity = NOPARITY; //��У��
		break;
	case 1:
		p.Parity = ODDPARITY; //��У��
		break;
	case 2:
		p.Parity = EVENPARITY; //żУ��
		break;
	case 3:
		p.Parity = MARKPARITY; //���У��
		break;
	}

	switch (lpconfigport.stopbit) //ֹͣλ
	{
	case 1:
		p.StopBits = ONESTOPBIT; //1λֹͣλ
		break;
	case 2:
		p.StopBits = TWOSTOPBITS; //2λֹͣλ
		break;
	case 3:
		p.StopBits = ONE5STOPBITS; //1.5λֹͣλ
		break;
	}

	if (!SetCommState(hCom, &p))
	{
		// ���ò���ʧ��
		return false;
	}

	//GetCommTimeouts(hCom, LpTimeOuts);

	SetCommTimeouts(hCom, &TimeOuts);

	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);//��մ��ڻ�����

	memcpy(pHandle, &hCom, sizeof(hCom));// ������

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

		// ͬ����ʽ
	DWORD dwBytesWrite = len; //�ɹ�д��������ֽ���
	BOOL bWriteStat = WriteFile(hCom, //���ھ��
		buf, //�����׵�ַ
		dwBytesWrite, //Ҫ���͵������ֽ���
		&dwBytesWrite, //DWORD*���������շ��سɹ����͵������ֽ���
		NULL); //NULLΪͬ�����ͣ�OVERLAPPED*Ϊ�첽����
	if (!bWriteStat)
	{
		return 0;
	}
	return dwBytesWrite;
}

int WzSerialPort::receive(void *buf, int maxlen)
{
	HANDLE hCom = *(HANDLE*)pHandle;

		//ͬ����ʽ
	DWORD wCount = maxlen; //�ɹ���ȡ�������ֽ���
	BOOL bReadStat = ReadFile(hCom, //���ھ��
		buf, //�����׵�ַ
		wCount, //Ҫ��ȡ����������ֽ���
		&wCount, //DWORD*,�������շ��سɹ���ȡ�������ֽ���
		NULL); //NULLΪͬ�����ͣ�OVERLAPPED*Ϊ�첽����
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
		HANDLE hCom1; //ȫ�ֱ��������ھ�� 
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


