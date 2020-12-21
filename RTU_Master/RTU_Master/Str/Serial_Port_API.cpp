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


void WzSerialPort::AvailableCOM(set<string>& myset)
{
	HKEY hKey;
	char *lpSubKey = "HARDWARE\\DEVICEMAP\\SERIALCOMM\\";

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		return;
	}
	char szValueName[NAME_LEN] = {};
	BYTE szPortName[NAME_LEN] = {};
	LONG status;
	DWORD dwIndex = 0;
	DWORD dwSizeValueName = NAME_LEN;
	DWORD dwSizeofPortName = NAME_LEN;
	DWORD Type;
	dwSizeValueName = NAME_LEN;
	dwSizeofPortName = NAME_LEN;
	do
	{
		status = RegEnumValueA(hKey, dwIndex++, szValueName, &dwSizeValueName, NULL, &Type,
			szPortName, &dwSizeofPortName);
		if ((status == ERROR_SUCCESS))
		{
			string str = ""; 
			printf("%s ", szPortName);
			str.assign((char*)szPortName, strlen((char*)szPortName));
			myset.insert(str);
		}
		//ÿ��ȡһ��dwSizeValueName��dwSizeofPortName���ᱻ�޸�
		//ע��һ��Ҫ����,�������ֺ�����Ĵ���,���˾��Թ���û������,���ֶ�����COM����10���ϵĴ���
		dwSizeValueName = NAME_LEN;
		dwSizeofPortName = NAME_LEN;
	} while ((status != ERROR_NO_MORE_ITEMS));
	RegCloseKey(hKey);
	printf("\n");
}


