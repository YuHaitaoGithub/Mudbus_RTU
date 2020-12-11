#include"..\include\main.h"

uint8_t receiveBuf[1024] = {};
uint8_t SendBuf[1024] = {};
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


/*数据输入*************************************/
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
	/*从站地址*/
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
		char *filename = "../../Coil.ini";
		char *section = "Coil";
		printf("请修改 %d 到 %d 的数值", addr, Coilnum + addr - 1);
		system("Coil.ini");
		while (num--)
		{
			int number = 0;
			int j = 0;
			for (addr; j < 8 && Coilnum; j++)
			{
				Coilnum--;
				char k[5] = {};
				_itoa_s(addr + j, k, 10);
				number = GetPrivateProfileIntA(section, k, -1, filename);
				SendBuf[retlen] = ((0xff & number) << (j)) | SendBuf[retlen];
			}
			retlen = retlen + 1;
			if (!Coilnum)break;
			j = 0;
			addr = addr + 8;
		}
		break;
	}

	case 16:{
		int retaddr = 0; int RstNum = 0;
		injuage.In_10_Data(&retaddr, &RstNum);
		SendBuf[retlen++] = (RstNum * 2) & 0xff;

		char *filename = "../../Register.ini";
		char *section = "register";
		printf("请修改 %d 到 %d 的数值", retaddr, RstNum + retaddr - 1);
		system("Register.ini");
		for (retaddr; retaddr < RstNum; retaddr++, retlen = retlen + 2)
		{
			char k[10] = {};
			_itoa_s(retaddr, k, 10);
			int num = GetPrivateProfileIntA(section, k, -1, filename);
			SendBuf[retlen] = (0xff & (num >> 8));
			SendBuf[retlen + 1] = 0xff & num;
		}
		break;
	}
	}
	return retlen;

}




/*数据发送***********************************/
bool sendDemo(int* send_dataLen)
{
	SystemChange data;
	int ret = Input();

	uint16_t crc_ret = crc16table(SendBuf, ret);
	SendBuf[ret++] = crc_ret & 0xff;
	SendBuf[ret++] = (crc_ret >> 8) & 0xff;

	cout << "要发送的消息帧" << endl;
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
		cout << "已发送" << endl;
		*send_dataLen = ret;
		return true;
	}
	else
	{
		cout << "发送失败" << endl;
		return false;
	}
}





/*数据接收***************************************/
void ReceiveDemo(int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);

	HANDLE hCom = *(HANDLE*)w.pHandle;
	PurgeComm(hCom,PURGE_RXCLEAR);
	int retLenth = w.receive(receiveBuf, bufLenth+1);
	if (!data.LenthJuage(retLenth, bufLenth))
		return;

	/*CRC校验*/
	uint16_t d = crc16table(receiveBuf, retLenth - 2);
	uint16_t t = ((uint16_t)((receiveBuf[retLenth - 1] & 0x00ff) << 8) 
		| (uint16_t)(receiveBuf[retLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRC校验不一致" <<endl;
		return;
	}

	/*逐位判断***/
	if (!data.ErrorcodeJuage(SendBuf, receiveBuf, bufLenth, retLenth))
		return;

	/*十六进制打印*/
	cout << "从站响应消息帧" << endl;
	for (int i = 0; i < retLenth; i++)
	{
		uint8_t r[10] = {};
		nToHexstr(receiveBuf[i], r, 2);
		cout << r << " ";
	}
	cout << endl;
}



/*串口配置***************************/
void InPortParameter(WzSerialPort *Rcom)
{
lop:set<int>myset;
	cout << "可用串口" << endl;
	Rcom->AvailableCOM(myset);
	cout << "输入可用串口号" << endl;
	int port = 0;
	cin >> port;
	if (!myset.count(port))
	{
		cout << "无此串口，请重新输入串口号" << endl;
		goto lop;
	}
	char p[20] = {};
	sprintf(p, "\\\\.\\COM%d", port);
	Rcom->lpconfigport.portname = (char*)calloc(strlen(p) + 1, sizeof(char));
	memcpy(Rcom->lpconfigport.portname, p, strlen(p));
	memset(p, 0, 20);
	char *f = "../../SportParameter.ini";
	char *sec = "ParameterText";
	printf("当前串口默认参数：波特率%u，数据位%u，无校验%u，停止位%u, 超时时间%u秒\n", GetPrivateProfileIntA(sec, "Baudrate", -1, f), 
		GetPrivateProfileIntA(sec, "Databit", -1, f), GetPrivateProfileIntA(sec, "Parity", -1, f),
		GetPrivateProfileIntA(sec, "Stopbit", -1, f), GetPrivateProfileIntA(sec, "Timeout", -1, f));
	int chaeck = 0;
	cout << "默认参数按1，修改按2" << endl;
	cin >> chaeck;
	if (chaeck == 2)
	{
		cout << "输入波特率" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Baudrate", p, f);
		memset(p, 0, 20);
		cout << "输入数据位" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Databit", p, f);
		memset(p, 0, 20);
		cout << "输入校验位" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Parity", p, f);
		memset(p, 0, 20);
		cout << "输入停止位" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Stopbit", p, f);
		memset(p, 0, 20);
		cout << "输入超时时间" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Timeout", p, f);
		memset(p, 0, 20);
	}

	Rcom->lpconfigport.baudrate = GetPrivateProfileIntA(sec, "Baudrate", -1, f);
	Rcom->lpconfigport.databit = GetPrivateProfileIntA(sec, "Databit", -1, f);
	Rcom->lpconfigport.parity = GetPrivateProfileIntA(sec, "Parity", -1, f);
	Rcom->lpconfigport.stopbit = GetPrivateProfileIntA(sec, "Stopbit", -1, f);

	Rcom->TimeOuts.ReadTotalTimeoutConstant = GetPrivateProfileIntA(sec, "Timeout", -1, f);
	Rcom->TimeOuts.ReadIntervalTimeout = 2; //读间隔超时
	Rcom->TimeOuts.ReadTotalTimeoutMultiplier = 0; //读时间系数
	Rcom->TimeOuts.WriteTotalTimeoutMultiplier = 500; // 写时间系数
	Rcom->TimeOuts.WriteTotalTimeoutConstant = 2000; //写时间常量
	return;
}




/*串口监听线程************************************/
void SportListen(void*)
{
	WzSerialPort p = w;
	
	char *Lconfigport = (char *)calloc(strlen(p.lpconfigport.portname)+1,sizeof(char));
	memcpy(Lconfigport, p.lpconfigport.portname, strlen(p.lpconfigport.portname));
	HANDLE hCom = NULL;
	while (1)
	{
		/*监听是否断开*/
		hCom = CreateFileA((char*)Lconfigport, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCom == INVALID_HANDLE_VALUE )
		{
			if (GetLastError() == 2 && tag == 1)
			{
				char str[20] = {};
				cout << "串口不不存在" << endl;
				cout << "等待连接.........." << endl;
				tag = 0;
				memset(&w, 0, sizeof(w));
				CloseHandle(hCom);
			}
		}

		/*监听是否再次连接上*/
		if (hCom != INVALID_HANDLE_VALUE && tag == 0)
		{
			memset(w.pHandle, 0, 16);
			w = p;
			memcpy(w.pHandle, &hCom, sizeof(hCom));
			tag = 2;
			cout << "已连接" << endl;
			cout << "请继续输入"<< endl;
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



/*主函数************************************/
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
	
	while (1)
	{
		/*开启线程监控串口*/
		 _beginthread(SportListen, 0, NULL); 
		int send_dataLen = 0;
		if (!sendDemo(&send_dataLen))
		{
			memset(receiveBuf, 0, 1024);
			memset(SendBuf, 0, 1024);
			continue;
		}
		cout << "读取数据中........" << endl;
		int receive_dataLen = 0;
		ReceiveDemo(send_dataLen);


		memset(receiveBuf, 0, 1024);
		memset(SendBuf, 0, 1024);
		char t = {};
		cout << "输入数字 0 退出，输入数字 1 继续" << endl;
		cin >> t;
		if (t != '1')break;
	}
	w.close();
	return;
}
