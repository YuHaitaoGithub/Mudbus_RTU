#include"..\include\main.h"

uint8_t receiveBuf[ReceiveBufLen] = {};
uint8_t SendBuf[SendBufLen] = {};
int tag = 1;

WzSerialPort w;

void nToHexstr(uint8_t n, uint8_t * hexstr, uint8_t strlen)
{
	uint8_t hexChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	uint8_t i;
	uint8_t dis;
	unsigned long nTemp = (long)n;
	for (i = 1; i <= strlen; i++)
	{
		dis = nTemp & 0x0f;
		hexstr[strlen - i] = hexChar[dis];
		nTemp = nTemp >> 4;
	}
	hexstr[strlen] = '\0';
}


/*��������*************************************/
int Input()
{
	char c = '0';
	while(c != '\n')
		 c = getchar();
	int retlen = 0;
	DataInput injuage;
	SystemChange data;
	injuage.len = &retlen;
	injuage.buf = SendBuf;
	/*��վ��ַ*/
	injuage.In_SlaveAddress();

	int fun_code = injuage.FunctionCode();

	switch (fun_code)
	{
	case 1:{injuage.In_01_Data(); break; }

	case 3:{injuage.In_03_Data(); break; }

	case 15:{
		int addr = 0; int Coilnum = 0;
		injuage.In_0f_Data(&addr, &Coilnum);
		int num = Coilnum % 8 != 0 ? Coilnum / 8 + 1 : Coilnum / 8;
		SendBuf[retlen++] = num & 0xff;
		printf("������%d���ֽڵ����ݣ�ʮ�����ƣ�\n",num);
		injuage.RW_InJauge(num);
		break;
	}
		
	case 16:{
		int retaddr = 0; int RstNum = 0;
		injuage.In_10_Data(&retaddr, &RstNum);
		SendBuf[retlen++] = (RstNum * 2) & 0xff;
		printf("������%d���ֽڵ����ݣ�ʮ�����ƣ�\n", RstNum * 2);
		injuage.RW_InJauge(RstNum * 2);
		break;
	}
	}
	return retlen;

}



/*���ݷ���***********************************/
bool sendDemo(int* send_dataLen)
{
	SystemChange data;
	int ret = Input();

	uint16_t crc_ret = crc16table(SendBuf, ret);
	SendBuf[ret++] = crc_ret & 0xff;
	SendBuf[ret++] = (crc_ret >> 8) & 0xff;

	cout << "Ҫ���͵���Ϣ֡" << endl;
	for (int i = 0; i < ret; i++)
	{
		uint8_t d[5] = {};
		nToHexstr(SendBuf[i], d, 2);
		cout << d <<" ";
	}
	cout << endl;
	HANDLE hCom = *(HANDLE*)w.pHandle;
	PurgeComm(hCom, PURGE_TXCLEAR);
	if (w.send(SendBuf, ret) == ret)
	{
		cout << "�ѷ���" << endl;
		*send_dataLen = ret;
		return true;
	}
	else
	{
		if (tag == 0)
			cout << "���ڲ����ڣ�����ʧ��" << endl;
		else
			cout << "����ʧ��" << endl;
		return false;
	}
}





/*���ݽ���***************************************/
bool ReceiveDemo(int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);

	HANDLE hCom = *(HANDLE*)w.pHandle;
	PurgeComm(hCom,PURGE_RXCLEAR);
	int retLenth = w.receive(receiveBuf, bufLenth+1);
	if (tag == 0 || GetLastError() == 995)
	{
		cout << "���ڲ����ڣ�����ʧ��" << endl;
		return false;
	}
	if (!data.LenthJuage(retLenth, bufLenth))
		return false;

	/*CRCУ��*/
	uint16_t d = crc16table(receiveBuf, retLenth - 2);
	uint16_t t = ((uint16_t)((receiveBuf[retLenth - 1] & 0x00ff) << 8) 
		| (uint16_t)(receiveBuf[retLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRCУ�鲻һ��" <<endl;
		return false;
	}

	/*��λ�ж�***/
	if (!data.ErrorcodeJuage(SendBuf, receiveBuf, bufLenth, retLenth))
		return false;

	/*ʮ�����ƴ�ӡ*/
	cout << "��վ��Ӧ��Ϣ֡" << endl;
	for (int i = 0; i < retLenth; i++)
	{
		uint8_t r[10] = {};
		nToHexstr(receiveBuf[i], r, 2);
		cout << r << " ";
	}
	cout << endl;
	return true;
}



/*��������***************************/
void InPortParameter(WzSerialPort *Rcom)
{
	/*���ں�ѡ��**********************/
	int port = 0;
	for (;;)
	{
		set<int>myset;
		cout << "���ô���" << endl;
		Rcom->AvailableCOM(myset);
		cout << "������ô��ں�,����ѡ�� -1" << endl;
		port = 0;
		cin >> port;
		cin.clear();cin.sync();
		if (port == -1)continue;
		if (!myset.count(port))
		{
			cout << "�޴˴��ڣ����������봮�ں�" << endl;
			continue;
		}
		break;
	}
	char p[20] = {};
	sprintf(p, "\\\\.\\COM%d", port);
	Rcom->lpconfigport.portname = (char*)calloc(strlen(p) + 1, sizeof(char));
	memcpy(Rcom->lpconfigport.portname, p, strlen(p));
	memset(p, 0, 20);

	/*�����ʵȲ�������**************************/
	char *f = "../../SportParameter.ini";
	char *sec = "ParameterText";
	printf("��ǰ����Ĭ�ϲ�����������%u������λ%u����У��%u��ֹͣλ%u, ��ʱʱ��%u��\n", GetPrivateProfileIntA(sec, "Baudrate", -1, f), 
		GetPrivateProfileIntA(sec, "Databit", -1, f), GetPrivateProfileIntA(sec, "Parity", -1, f),
		GetPrivateProfileIntA(sec, "Stopbit", -1, f), GetPrivateProfileIntA(sec, "Timeout", -1, f));
	int chaeck = 0;
	cout << "Ĭ�ϲ�����1���޸İ�2" << endl;
	cin >> chaeck;
	set<string>mset1{ "9600", "4800", "115200", "19200" };
	set<string>mset2{ "8" ,"5"};
	set<string>mset3{ "0", "1", "2" };
	set<string>mset4{ "0", "1", "2" };
	vector<pair<char*, set<string>>>myvector{ { "Baudrate(4800,9600,115200,19200)", mset1 }, { "Databit(8)", mset2 }, { "Parity(0,1,2)", mset3 }, { "Stopbit(0,1,2)", mset4 } };
	if (chaeck == 2)
	{
		for (int j = 0; j < 4;j++)
		{
			string pp;
			printf("����%s\n", myvector[j].first);
			cin >> pp;
			if (!myvector[j].second.count(pp)){
				cout << "�������������";
				--j;
				continue;
			}
			strcpy(p, pp.c_str());
			WritePrivateProfileStringA(sec, myvector[j].first, p, f);
			memset(p, 0, 20);
		}
		cout << "���볬ʱʱ��" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Timeout", p, f);
	}
	/*������ֵ*/
	Rcom->lpconfigport.baudrate = GetPrivateProfileIntA(sec, "Baudrate", -1, f);
	Rcom->lpconfigport.databit = GetPrivateProfileIntA(sec, "Databit", -1, f);
	Rcom->lpconfigport.parity = GetPrivateProfileIntA(sec, "Parity", -1, f);
	Rcom->lpconfigport.stopbit = GetPrivateProfileIntA(sec, "Stopbit", -1, f);

	Rcom->TimeOuts.ReadTotalTimeoutConstant = GetPrivateProfileIntA(sec, "Timeout", -1, f);
	Rcom->TimeOuts.ReadIntervalTimeout = 2; //�������ʱ
	Rcom->TimeOuts.ReadTotalTimeoutMultiplier = 0; //��ʱ��ϵ��
	Rcom->TimeOuts.WriteTotalTimeoutMultiplier = 500; // дʱ��ϵ��
	Rcom->TimeOuts.WriteTotalTimeoutConstant = 2000; //дʱ�䳣��
	return;
}



/*���ڼ����߳�************************************/
void SportListen(void*)
{
	WzSerialPort p = w;
	char *Lconfigport = (char *)calloc(strlen(p.lpconfigport.portname)+1,sizeof(char));
	memcpy(Lconfigport, p.lpconfigport.portname, strlen(p.lpconfigport.portname));
	HANDLE hCom = NULL;
	while (1)
	{
		/*�����Ƿ�Ͽ�*/
		hCom = CreateFileA((char*)Lconfigport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCom == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == 2)
			{
				char str[20] = {};
				cout << "���ڱ��γ�" << endl;
				tag = 0;
				memset(&w, 0, sizeof(w));
				CloseHandle(hCom);
				break;
			}
		}
		Sleep(100);
	}
	_endthread();
}



/*������************************************/
void main()
{
	while (1)
	{
		memset(&w, 0, sizeof(WzSerialPort));

		InPortParameter(&w);

		bool open_sign = w.open();
		if (!open_sign)
		{
			cout << "open serial port failed..." << endl;
			continue;
		}

		while (1)
		{
			/*�����̼߳�ش���*/
			_beginthread(SportListen, 0, NULL);
			int send_dataLen = 0;
			if (!sendDemo(&send_dataLen))
			{
				memset(receiveBuf, 0, ReceiveBufLen);
				memset(SendBuf, 0, SendBufLen);
				if (tag == 0){
					tag = 1; break;
				}
				continue;
			}
			cout << "��ȡ������........" << endl;
			int receive_dataLen = 0;

			if (!ReceiveDemo(send_dataLen))
			{
				memset(receiveBuf, 0, ReceiveBufLen);
				memset(SendBuf, 0, SendBufLen);
				if (tag == 0){
					tag = 1; break;
				}
				continue;
			}
			memset(receiveBuf, 0, ReceiveBufLen);
			memset(SendBuf, 0, SendBufLen);
			while (1)
			{
				char t = {};
				cout << "�������� 0 ���������������� 1 ����" << endl;
				cin >> t;
				if (t == '0')return;
				if (t == '1')break;
			}
		}
		w.close();
	}
	return;
}
