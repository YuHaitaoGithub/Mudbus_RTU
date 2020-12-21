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
		printf("请输入%d个字节的数据（十六进制）\n",num);
		injuage.RW_InJauge(num);
		break;
	}
		
	case 16:{
		int retaddr = 0; int RstNum = 0;
		injuage.In_10_Data(&retaddr, &RstNum);
		SendBuf[retlen++] = (RstNum * 2) & 0xff;
		printf("请输入%d个字节的数据（十六进制）\n", RstNum * 2);
		injuage.RW_InJauge(RstNum * 2);
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
		if (tag == 0)
			cout << "串口不存在，发送失败" << endl;
		else
			cout << "发送失败" << endl;
		return false;
	}
}





/*数据接收***************************************/
bool ReceiveDemo(int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);

	HANDLE hCom = *(HANDLE*)w.pHandle;
	PurgeComm(hCom,PURGE_RXCLEAR);
	int retLenth = w.receive(receiveBuf, bufLenth+1);
	if (tag == 0 || GetLastError() == 995)
	{
		cout << "串口不存在，接收失败" << endl;
		return false;
	}
	if (!data.LenthJuage(retLenth, bufLenth))
		return false;

	/*CRC校验*/
	uint16_t d = crc16table(receiveBuf, retLenth - 2);
	uint16_t t = ((uint16_t)((receiveBuf[retLenth - 1] & 0x00ff) << 8) 
		| (uint16_t)(receiveBuf[retLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRC校验不一致" <<endl;
		return false;
	}

	/*逐位判断***/
	if (!data.ErrorcodeJuage(SendBuf, receiveBuf, bufLenth, retLenth))
		return false;

	/*十六进制打印*/
	cout << "从站响应消息帧" << endl;
	for (int i = 0; i < retLenth; i++)
	{
		uint8_t r[10] = {};
		nToHexstr(receiveBuf[i], r, 2);
		cout << r << " ";
	}
	cout << endl;
	return true;
}



/*串口配置***************************/
void InPortParameter(WzSerialPort *Rcom)
{
	/*串口号选择**********************/
	int port = 0;
	for (;;)
	{
		set<int>myset;
		cout << "可用串口" << endl;
		Rcom->AvailableCOM(myset);
		cout << "输入可用串口号,重新选择按 -1" << endl;
		port = 0;
		cin >> port;
		cin.clear();cin.sync();
		if (port == -1)continue;
		if (!myset.count(port))
		{
			cout << "无此串口，请重新输入串口号" << endl;
			continue;
		}
		break;
	}
	char p[20] = {};
	sprintf(p, "\\\\.\\COM%d", port);
	Rcom->lpconfigport.portname = (char*)calloc(strlen(p) + 1, sizeof(char));
	memcpy(Rcom->lpconfigport.portname, p, strlen(p));
	memset(p, 0, 20);

	/*波特率等参数设置**************************/
	char *f = "../../SportParameter.ini";
	char *sec = "ParameterText";
	printf("当前串口默认参数：波特率%u，数据位%u，无校验%u，停止位%u, 超时时间%u秒\n", GetPrivateProfileIntA(sec, "Baudrate", -1, f), 
		GetPrivateProfileIntA(sec, "Databit", -1, f), GetPrivateProfileIntA(sec, "Parity", -1, f),
		GetPrivateProfileIntA(sec, "Stopbit", -1, f), GetPrivateProfileIntA(sec, "Timeout", -1, f));
	int chaeck = 0;
	cout << "默认参数按1，修改按2" << endl;
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
			printf("输入%s\n", myvector[j].first);
			cin >> pp;
			if (!myvector[j].second.count(pp)){
				cout << "输入错误请重新";
				--j;
				continue;
			}
			strcpy(p, pp.c_str());
			WritePrivateProfileStringA(sec, myvector[j].first, p, f);
			memset(p, 0, 20);
		}
		cout << "输入超时时间" << endl;
		cin >> p;
		WritePrivateProfileStringA(sec, "Timeout", p, f);
	}
	/*参数赋值*/
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
		if (hCom == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == 2)
			{
				char str[20] = {};
				cout << "串口被拔出" << endl;
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



/*主函数************************************/
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
			/*开启线程监控串口*/
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
			cout << "读取数据中........" << endl;
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
				cout << "输入数字 0 结束程序，输入数字 1 继续" << endl;
				cin >> t;
				if (t == '0')return;
				if (t == '1')break;
			}
		}
		w.close();
	}
	return;
}
