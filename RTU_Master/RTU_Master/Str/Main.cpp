#include"..\include\main.h"

uint8_t receiveBuf[1024] = {};
uint8_t SendBuf[1024] = {};
int tag = 1;

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
		cout << "�����վ��ַ1-255��ʮ��������" << endl;
		while (1){
			cin >> num;
			if (cin.fail()){ 
				cin.sync(); cin.clear(); goto loop1;
			}
			cin.sync();
			if (num > 255 || num < 1)
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
			char *filename = "../../Register.ini";
			char *section = "register";
			printf("���޸� %d �� %d ����ֵ", i, RstNum + i - 1);
			system("Register.ini");
			for (i; i < RstNum; i++, *ret = (*ret) + 2)
			{
				char k[10] = {};
				_itoa_s(i, k, 10);
				num = GetPrivateProfileIntA(section, k, -1, filename);
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
bool sendDemo(WzSerialPort& wz, int* send_dataLen)
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
	if (wz.send(SendBuf, ret) == ret)
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
void ReceiveDemo(WzSerialPort& wz, int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);
	int retLenth = wz.receive(receiveBuf, bufLenth);
	if (retLenth == 0)
	{
		cout << "��ȡ��ʱ" << endl;
		return;
	}
	uint16_t d = crc16table(receiveBuf, bufLenth-2);
	uint16_t t = ((uint16_t)((receiveBuf[bufLenth - 1] & 0x00ff) << 8) | (uint16_t)(receiveBuf[bufLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRCУ�鲻һ��" << endl;
		return;
	}
	if (!data.ErrorcodeJuage(SendBuf, receiveBuf, bufLenth, retLenth))
		return;
	for (int i = 0; i < retLenth; i++)
	{
		uint8_t r[10] = {};
		data.nToHexstr(receiveBuf[i], r, 2);
		cout << r << " ";
	}
	cout << endl;
}


/*��������***************************/
void InPortParameter(LPCOMMTIMEOUTS lptimeout, SelportParameters* lpconfigport)
{
	WzSerialPort Rcom;
	cout << "���ô���" << endl;
	Rcom.AvailableCOM();
	cout << "������ô��ں�" << endl;
	int port = 0;
	cin >> port;
	char p[20] = {};
	sprintf(p, "\\\\.\\COM%d", port);
	lpconfigport->portname = (char*)calloc(strlen(p)+1,sizeof(char));
	memcpy(lpconfigport->portname, p, strlen(p));
	cout << "��ǰ����Ĭ�ϲ�����������9600������λ8����У�飬ֹͣλ1,��ʱʱ��2��" << endl;
	int chaeck = 1;
	cout << "�޸Ĵ��ڲ���������Ҫ��ر�" << endl;
	system("SportParameter.ini");
	char *f = "../../SportParameter.ini";
	char *sec = "ParameterText";

	lpconfigport->baudrate = GetPrivateProfileIntA(sec, "Baudrate", -1, f); 
	lpconfigport->databit = GetPrivateProfileIntA(sec, "Databit", -1, f);
	lpconfigport->parity = GetPrivateProfileIntA(sec, "Parity", -1, f); 
	lpconfigport->stopbit = GetPrivateProfileIntA(sec, "Stopbit", -1, f); 

	lptimeout->ReadTotalTimeoutConstant = GetPrivateProfileIntA(sec, "Timeout", -1, f); 	
	lptimeout->ReadIntervalTimeout = 2; //�������ʱ
	lptimeout->ReadTotalTimeoutMultiplier = 0; //��ʱ��ϵ��
	lptimeout->WriteTotalTimeoutMultiplier = 500; // дʱ��ϵ��
	lptimeout->WriteTotalTimeoutConstant = 2000; //дʱ�䳣��
	return;
}

void main();

/*���ڼ����߳�************************************/
void SportListen(void* Lconfigport)
{
	HANDLE hCom = NULL;
	while (1)
	{
		hCom = CreateFileA((char*)Lconfigport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCom == INVALID_HANDLE_VALUE )
		{
			if (GetLastError() == 2 && tag == 1)
			{
				/*printf("���ڲ�����\n�ȴ�����\n");*/
				char str[20] = {};
				MessageBoxA(0, "���ڲ�����", str, 0);
				cout << "�ȴ�����.........." << endl;
				tag = 0;
				CloseHandle(hCom);
			}
		}
		if (hCom != INVALID_HANDLE_VALUE && tag == 0)
		{
			tag = 1;
			cout << "������" << endl;
			cout << "�Ͽ�ǰ����δ�����������ɵ�ǰĬ�Ϸ���ʧ����һ��ѭ����Ч"<< endl;
			CloseHandle(hCom);
			atexit(main);
			break;
		}
		Sleep(200);
	}
	_endthread();
}



/*������************************************/
void main()
{
	COMMTIMEOUTS TimeOuts;
	WzSerialPort w;
	SelportParameters configport;
	InPortParameter(&TimeOuts, &configport);
	char c[50] = {};
	memcpy(c, configport.portname, strlen(configport.portname));
	bool open_sign = w.open(&configport, 1, &TimeOuts);
	if (!open_sign)
	{
		cout << "open serial port failed..." << endl;
		return;
	}
	_beginthread(SportListen, 0, (void *)c);
	while (1)
	{
		int send_dataLen = 0;
		if (!sendDemo(w, &send_dataLen))
			return;
		cout << "��ȡ������........" << endl;
		int receive_dataLen = 0;
		ReceiveDemo(w, send_dataLen);


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
