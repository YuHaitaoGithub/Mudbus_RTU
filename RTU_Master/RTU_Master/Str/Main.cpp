#include"..\include\main.h"



void Input(vector<uint8_t>& in_num)
{
	SystemChange data;
	uint16_t num = 0;
	uint16_t num2 = 0;
	cout << "输入从站地址" << endl;
	cin >> num;
	in_num.push_back(0x00ff & num);
	cout << "输入功能码" << endl;
	cin >> num;
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
	case 3:{
		cout << "输入寄存器起始地址" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "要读寄存器的个数" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		break;
	}
	case 15:{
		cout << "输入线圈起始地址" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "要写入的线圈个数" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
		in_num.push_back(0x00ff & num);
		while (num--)
		{
			uint16_t i = num + 1;
			printf("要写入第%d个字节的数值\n", i - num);
			cin >> hex >> num2;
			in_num.push_back(0x00ff & num2);
		}
		break;
	}
	case 16:{
		cout << "输入寄存器起始地址" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "要写入寄存器的个数" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		num *= 2;
		in_num.push_back(0x00ff & num);
		num /= 2;
		while (num--)
		{
			uint16_t i = num+1;
			printf("要写入第%d个寄存器的数值的高位\n",i - num);
			cin >> hex >> num2;
			in_num.push_back(0x00ff & num2);
			printf("要写入第%d个寄存器的数值的低位\n", i - num);
			cin >> hex >> num2;
			in_num.push_back(0x00ff & num2);
		}
		break;
	}
	}
}


void sendDemo(WzSerialPort& wz)
{
	vector<uint8_t> datanum;
	Input(datanum);
	wz.send(&datanum, datanum.size());
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
	bool open_sign = w.open(ComPort, 9600, 0, 8, 1, 0, TimeOuts);
	if (!open_sign)
	{
		cout << "open serial port failed..." << endl;
		return;
	}
	sendDemo(w);
	system("pause");
	return;
}

