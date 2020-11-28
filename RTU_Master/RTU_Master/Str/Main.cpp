#include"..\include\main.h"



void Input(uint8_t* in_num,int* ret)
{
	SystemChange data;
	uint8_t l[3] = {};
	uint16_t num = 0;
	uint16_t num2 = 0;
	cout << "输入从站地址" << endl;
	cin >> num;
	in_num[(*ret)++] = 0xff & num;
	data.nToHexstr(num, l, 2);
	in_num[(*ret)++] = l[0];
	in_num[(*ret)++] = l[1];
	memset(l, 0, 2);

	cout << "输入功能码" << endl;
	cin >> num;
	data.nToHexstr(num, l, 2);
	in_num[(*ret)++] = l[0];
	in_num[(*ret)++] = l[1];
	memset(l, 0, 2);

	switch (num)
	{
	case 1:{
		cout << "输入线圈起始地址" << endl;
		cin >> num;
		data.nToHexstr((num>>8)&0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num  & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "要读线圈个数" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		break;
	}
	case 3:{
		cout << "输入寄存器起始地址" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "要读寄存器的个数" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		break;
	}
	case 15:{
		cout << "输入线圈起始地址" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "要写入的线圈个数" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		while (num--)
		{
			uint16_t i = num + 1;
			printf("要写入第%d个字节的数值\n", i - num);
			cin >> hex >> num2;
			data.nToHexstr(num2 & 0xff, l, 2);
			in_num[(*ret)++] = l[0];
			in_num[(*ret)++] = l[1];
			memset(l, 0, 2);
		}
		break;
	}
	case 16:{
		cout << "输入寄存器起始地址" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "要写入寄存器的个数" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		num *= 2;
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		num /= 2;
		while (num--)
		{
			uint16_t i = num+1;
			printf("要写入第%d个寄存器的数值的高位\n",i - num);
			cin >> hex >> num2;
			data.nToHexstr(num2 & 0xff, l, 2);
			in_num[(*ret)++] = l[0];
			in_num[(*ret)++] = l[1];
			memset(l, 0, 2);
			printf("要写入第%d个寄存器的数值的低位\n", i - num);
			cin >> hex >> num2;
			data.nToHexstr(num2 & 0xff, l, 2);
			in_num[(*ret)++] = l[0];
			in_num[(*ret)++] = l[1];
			memset(l, 0, 2);
		}
		break;
	}
	}
}


void sendDemo(WzSerialPort& wz)
{
	uint8_t datanum[1024] = {};
	int ret = 0;
	Input(datanum,&ret);
	wz.send(datanum, ret);
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

