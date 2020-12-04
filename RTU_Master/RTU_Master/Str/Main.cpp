#include"..\include\main.h"

uint8_t receiveBuf[1024] = {};
uint8_t SendBuf[1024] = {};
int tag = 1;

WzSerialPort w;

/*��������*************************************/
void Input(uint8_t* in_num,int* ret)
{
	SystemChange data;
	while (1)
	{
loop1:	uint8_t l[10] = {};
		int num = 0;
		int num2 = 0;
		int a = 100;
		cout << "�����վ��ַ0-255��ʮ��������" << endl;
		while (1){
			cin >> num;
			if (cin.fail()){ 
				cin.sync(); cin.clear(); goto loop1;
			}
			cin.sync();
			if (num > 255 || num < 0)
				cout << "�Ĵ�����ַ������Χ����������" << endl;
			else break;
		}
		in_num[(*ret)++] = num & 0xff;

  loop:	cout << "���빦����01��03��0f��10" << endl;
		cin >> l;
		if (strlen((char*)l) > 2){
			cout << "���������벻�Ϸ�����������" << endl; goto loop;
		}
		cin.sync();
		num2 = data.ChangeNum(l);
		if (!(num2 == 1||num2 == 3||num2 == 15||num2 == 16||num2 == 21)){
			cout << "���������벻�Ϸ�����������" << endl; goto loop;
		}
		if (num2 == 21)num2 = 15;
		in_num[(*ret)++] = 0xff & num2;
		memset(l, 0, 10);

		switch (num2)
		{
		case 1:{
			cout << "������Ȧ��ʼ��ַ0-65535(ʮ��������)" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 65535 || num < 0)
					cout << "�Ĵ�����ַ������Χ����������" << endl;
				else break;
			}
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;

			cout << "Ҫ����Ȧ����1-2000��ʮ����������" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 2000 || num < 1)
					cout << "�Ĵ�������������Χ����������" << endl;
				else break;
			}
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;
			break;
		}
		case 3:{
			cout << "����Ĵ�����ʼ��ַ0-65535��ʮ����������" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 65535 || num < 0)
					cout << "�Ĵ�����ַ������Χ����������" << endl;
				else break;
			}
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;

			cout << "Ҫ���Ĵ����ĸ���1-125(ʮ��������)" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 125 || num < 1)
					cout << "�Ĵ�������������Χ����������" << endl;
				else break;
			}
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;
			break;
		}
		case 15:{
			cout << "������Ȧ��ʼ��ַ0-65535(ʮ��������)" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 65535 || num < 0)
					cout << "�Ĵ�����ַ������Χ����������" << endl;
				else break;
			}
			int i = num;
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;

			cout << "Ҫд�����Ȧ����1-1968(ʮ��������)" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 1968 || num < 1)
					cout << "�Ĵ�������������Χ����������" << endl;
				else break;
			}
			int RstNum = num;
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;
			num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
			in_num[(*ret)++] = num & 0xff;
			char *filename = "../../Coil.ini";
			char *section = "Coil";
			printf("���޸� %d �� %d ����ֵ", i, RstNum + i - 1);
			system("Coil.ini");
			while (num--)
			{
				int number = 0;
				int j = 0;
				for (i; j < 8 && RstNum; j++)
				{
					RstNum--;
					char k[5] = {};
					_itoa_s(i + j, k, 10);
					number = GetPrivateProfileIntA(section, k, -1, filename);
					in_num[*ret] = ((0xff & number) << (j)) | in_num[*ret];
				}
				*ret = *ret + 1;
				if (!RstNum)break;
				j = 0;
				i = i + 8;
			}
			break;
		}
		case 16:{
			cout << "����Ĵ���0-65535֮�����ʼ��ַ(ʮ��������)" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 65535 || num < 0)
					cout << "�Ĵ�����ַ������Χ����������" << endl;
				else break;
			}
			int i = num;
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;

			cout << "Ҫд��Ĵ���1-123֮��ĵĸ���(ʮ��������)" << endl;
			while (1){
				cin >> num;
				cin.sync();
				if (num > 123 || num < 1)
					cout << "�Ĵ�������������Χ����������" << endl;
				else break;
			}
			int RstNum = num;
			in_num[(*ret)++] = (num >> 8) & 0xff;
			in_num[(*ret)++] = num & 0xff;
			in_num[(*ret)++] = (num * 2) & 0xff;
			int c = *ret;

loop5:		char *filename = "../../Register.ini";
			char *section = "register";
			printf("���޸� %d �� %d ����ֵ", i, RstNum + i - 1);
			system("Register.ini");
			for (i; i < RstNum; i++, *ret = (*ret) + 2)
			{
				char k[10] = {};
				_itoa_s(i, k, 10);
				num = GetPrivateProfileIntA(section, k, -1, filename);
				if (num > 65535)
				{
					*ret = c;
					memset(in_num + c, 0, 1023 - c);
					cout << "��ֵ���������¸�ֵ" << endl;
					goto loop5;
				}
				in_num[*ret] = (0xff & (num >> 8));
				in_num[(*ret) + 1] = 0xff & num;
			}
			break;
		}
		}
		break;
	}
}




/*���ݷ���***********************************/
bool sendDemo(int* send_dataLen)
{
	SystemChange data;
	int ret = 0;
	Input(SendBuf, &ret);

	uint16_t crc_ret = crc16table(SendBuf, ret);
	SendBuf[ret++] = crc_ret & 0xff;
	SendBuf[ret++] = (crc_ret >> 8) & 0xff;
	cout << "Ҫ���͵���Ϣ֡" << endl;
	for (int i = 0; i < ret; i++)
	{
		uint8_t d[5] = {};
		data.nToHexstr(SendBuf[i], d, 2);
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
		cout << "����ʧ��" << endl;
		return false;
	}
}




/*���ݽ���***************************************/
void ReceiveDemo(int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);

	HANDLE hCom = *(HANDLE*)w.pHandle;
	PurgeComm(hCom,PURGE_RXCLEAR);
	int retLenth = w.receive(receiveBuf, bufLenth);
	if (retLenth == 0)
	{
		cout << "��ȡ��ʱ" << endl;
		return;
	}
	if (retLenth < 5)
	{
		cout << "���ȹ���" << endl;
		return;
	}

	/*CRCУ��*/
	uint16_t d = crc16table(receiveBuf, retLenth - 2);
	uint16_t t = ((uint16_t)((receiveBuf[retLenth - 1] & 0x00ff) << 8) | (uint16_t)(receiveBuf[retLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRCУ�鲻һ��" <<endl;
		return;
	}

	/*��λ�ж�***/
	if (!data.ErrorcodeJuage(SendBuf, receiveBuf, bufLenth, retLenth))
		return;

	/*ʮ�����ƴ�ӡ*/
	cout << "��վ��Ӧ��Ϣ֡" << endl;
	for (int i = 0; i < retLenth; i++)
	{
		uint8_t r[10] = {};
		data.nToHexstr(receiveBuf[i], r, 2);
		cout << r << " ";
	}
	cout << endl;
}



/*��������***************************/
void InPortParameter(WzSerialPort *Rcom)
{
	cout << "���ô���" << endl;
	Rcom->AvailableCOM();
	cout << "������ô��ں�" << endl;
	int port = 0;
	cin >> port;
	char p[20] = {};
	sprintf(p, "\\\\.\\COM%d", port);
	Rcom->lpconfigport.portname = (char*)calloc(strlen(p) + 1, sizeof(char));
	memcpy(Rcom->lpconfigport.portname, p, strlen(p));
	cout << "��ǰ����Ĭ�ϲ�����������9600������λ8����У�飬ֹͣλ1,��ʱʱ��2��" << endl;
	int chaeck = 1;
	cout << "�޸Ĵ��ڲ���������Ҫ��ر�" << endl;
	system("SportParameter.ini");
	char *f = "../../SportParameter.ini";
	char *sec = "ParameterText";

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
		if (hCom == INVALID_HANDLE_VALUE )
		{
			if (GetLastError() == 2 && tag == 1)
			{
				char str[20] = {};
				MessageBoxA(0, "���ڲ�����", str, 0);
				cout << "�ȴ�����.........." << endl;
				tag = 0;
				memset(&w, 0, sizeof(w));
				CloseHandle(hCom);
			}
		}

		/*�����Ƿ��ٴ�������*/
		if (hCom != INVALID_HANDLE_VALUE && tag == 0)
		{
			memset(w.pHandle, 0, 16);
			w = p;
			memcpy(w.pHandle, &hCom, sizeof(hCom));
			tag = 2;
			cout << "������" << endl;
			cout << "���������"<< endl;
			/*w.pHandle = hCom;*/
			SetCommState(hCom, &w.p);
			SetCommTimeouts(hCom, &w.TimeOuts);
			/*CloseHandle(hCom);*/
			free(Lconfigport);
			Lconfigport = NULL;
			break;
		}
		Sleep(200);
	}
	_endthread();
}



/*������************************************/
void main()
{
	if (tag != 2)
		memset(&w, 0, sizeof(WzSerialPort));

	InPortParameter(&w);

	bool open_sign = w.open();
	if (!open_sign)
	{
		cout << "open serial port failed..." << endl;
		return;
	}
	/*�����̼߳�ش���*/
	_beginthread(SportListen, 0, NULL);

	while (1)
	{
		 _beginthread(SportListen, 0, NULL); 
		int send_dataLen = 0;
		if (!sendDemo(&send_dataLen))
		{
			memset(receiveBuf, 0, 1024);
			memset(SendBuf, 0, 1024);
			continue;
		}
		cout << "��ȡ������........" << endl;
		int receive_dataLen = 0;
		ReceiveDemo(send_dataLen);


		memset(receiveBuf, 0, 1024);
		memset(SendBuf, 0, 1024);
		char t = {};
		cout << "�������� 0 �˳����������� 1 ����" << endl;
		cin >> t;
		if (t != '1')break;
	}
	w.close();
	return;
}
