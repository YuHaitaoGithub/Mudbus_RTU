#include"..\include\main.h"


void sendDemo(WzSerialPort& wz)
{
	vector<uint16_t>dataIn;
	
	/*用户输入************************/
	/*char ss = '\0';
	int i = 0;
	string s = "";
	while (1)
	{
		ss = getchar();
		if (ss != '\n')
			s.push_back(ss);
		else
			break;
	}
	int len = s.size();
	char *p = (char *)malloc((len + 1)*sizeof(char));
	int len1 = strlen(p);
	unsigned char *pp = (unsigned char*)(strcpy(p, s.c_str()));*/
	uint16_t num;
	while (cin >> num)
	{
		if (confirm == num)
			break;
		dataIn.push_back(num);
	}
	char *buffer = new char[sizeof(uint16_t) + 1];
	vector<uint8_t *>buf;
	uint8_t *buf = new uint8_t[sizeof(uint8_t)];
	for (int i = 0; i < dataIn.size(); i++)
	{
		_itoa(dataIn[i], buffer, 16);
		buf.push_back((uint8_t)buffer);
		memset(buffer, 0, sizeof(uint16_t) + 1);
	}

	/*校验*****************************/
	int length = dataIn.size();
	uint16_t result = CRC16(buf, length);
	_itoa(0x00ff & result, buffer, 16);
	buf[length] = (uint8_t)buffer[length++];
	memset(buffer, 0, sizeof(uint16_t) + 1);
	_itoa(0xff00 & result, buffer, 16);
	buffer[2] = '\0';
	buf[length] = (uint8_t)buffer[length++];
	memset(buffer, 0, sizeof(uint16_t) + 1);
	/*发送*****************************/
	wz.send(buf, length);
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
	if (!open_sign)
	{
		cout << "open serial port failed..." << endl;
		return;
	}

	sendDemo(w);

	return;
}