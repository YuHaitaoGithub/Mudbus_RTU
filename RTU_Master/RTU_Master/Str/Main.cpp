#include"..\include\main.h"



void Input(vector<uint8_t>& in_num)
{
	uint16_t num = 0;
	
	cout << "输入从站地址" << endl;
	cin >>hex>> num;
	in_num.push_back(0x00ff & num);
	cout << "输入功能码" << endl;
	cin >>num;
	in_num.push_back(0x00ff & num);
	switch (num)
	{
	case 1:{	
		cout << "输入线圈起始地址" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "要读线圈个数" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		break;
	}
	}
}


void sendDemo(WzSerialPort& wz)
{
	vector<uint8_t>dataIn;
	uint16_t CRC_num;
	/*等待接收*******************/
	Input(dataIn);
	uint16_t len = dataIn.size();

	/*校验码********************/
	CRC_num = crc16table(dataIn, len);

	/*数据打包******************/
	dataIn.push_back(0x00ff & CRC_num);
	dataIn.push_back((0x00ff & (CRC_num>>8)));
	len = dataIn.size();
	uint8_t* pp = new uint8_t[len + 1];
	if (!dataIn.empty())
	{
		memcpy(pp, &dataIn[0], len*sizeof(uint8_t));
	}

	/*数据发送**********************/
	wz.send(pp,len);
	uint16_t aa[10] = {};
	int c = 200;
	while (c--);
	{
		Sleep(10);
		if (wz.receive(aa, 10))
		{
			cout << hex << aa;
		}
	}
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
	system("pause");
	return;
}

