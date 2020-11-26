#include"..\include\main.h"

#define toascii(c) (((unsigned char)(c))&0x7f)






void sendDemo(WzSerialPort& wz)
{
	vector<uint16_t>dataIn;
	
	/*用户输入************************/
	char ss[3] = {};
	int i = 0;
	vector<string>s;
	uint16_t a;
	vector<uint16_t>input;
	while (1)
	{
		cin >> hex >> a;
		cout << hex << a << endl;
		char v[10] = {};
		_itoa(0x00ff&a,v,10);
		uint16_t x = CRC16((uint8_t*)v, 3);
		cout << hex<<x<<endl;
		char n1[10] = {};
		_itoa(0x00ff & x, n1, 10);
		cout <<hex<< n1 << endl;
		char n2[10] = {};
		_itoa(0x00ff & (x>>8), n2, 10);
		cout << hex<<n2 << endl;
	/*	cout << hex<<v<<endl;
		sprintf(ss, "%x", a);
		(uint8_t)ss;
		cout << ss << endl;*/
	}
	//char *p = (char *)malloc((i + 1)*sizeof(char));
	//int len1 = strlen(p);
	//unsigned char *pp = (unsigned char*)(strcpy(p, s.c_str()));

	//wz.send(pp,len);
}



void main()
{
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 1000; //读间隔超时
	TimeOuts.ReadTotalTimeoutMultiplier = 500; //读时间系数
	TimeOuts.ReadTotalTimeoutConstant = 5000; //读时间常量
	TimeOuts.WriteTotalTimeoutMultiplier = 500; // 写时间系数
	TimeOuts.WriteTotalTimeoutConstant = 2000; //写时间常量
	WzSerialPort w;
	bool open_sign = w.open(ComPort, 9600, 0, 8, 1, 1, TimeOuts);
	//if (!open_sign)
	//{
	//	cout << "open serial port failed..." << endl;
	//	return;
	//}

	sendDemo(w);

	return;
}

