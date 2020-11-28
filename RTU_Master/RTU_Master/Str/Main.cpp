#include"..\include\main.h"



void Input(uint8_t* in_num,int* ret)
{
	SystemChange data;
	uint8_t l[10] = {};
	uint16_t num = 0;
	uint16_t num2 = 0;
	cout << "输入从站地址（十进制数）" << endl;
	cin >> num;
	in_num[(*ret)++] = num&0xff;

	cout << "输入功能码01、03、0f、10" << endl;
	cin >> l;
	num2 = data.ChangeNum(l);
	in_num[(*ret)++] = 0xff&num2;
	memset(l, 0, 10);

	switch (num2)
	{
	case 1:{
		cout << "输入线圈起始地址(十进制整数)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "要读线圈个数（十进制整数）" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		break;
	}
	case 3:{
		cout << "输入寄存器起始地址（十进制整数）" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "要读寄存器的个数(十进制整数)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		break;
	}
	case 15:{
		cout << "输入线圈起始地址(十进制整数)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "要写入的线圈个数(十进制整数)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
		in_num[(*ret)++] = num & 0xff;
		while (num--)
		{
			uint16_t i = num + 1;
			printf("要写入第%d个字节的数值(十六进制)\n", i - num);
			cin >> l;
			in_num[(*ret)++] = 0xff&data.ChangeNum(l);
			memset(l, 0, 10);
		}
		break;
	}
	case 16:{
		cout << "输入寄存器起始地址(十进制整数)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "要写入寄存器的个数(十进制整数)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num>>8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		if(num>=2)num /= 2;
		while (num--)
		{
			uint16_t i = num+1;
			printf("要写入第%d个寄存器的数值的高位(十六进制)\n",i - num);
			cin >> l;
			in_num[(*ret)++] = data.ChangeNum(l) & 0xff;
			memset(l, 0, 10);
			printf("要写入第%d个寄存器的数值的低位(十六进制)\n", i - num);
			cin >> l;
			in_num[(*ret)++] = data.ChangeNum(l) & 0xff;
			memset(l, 0, 10);
		}
		break;
	}
	}
}


bool sendDemo(WzSerialPort& wz, int* send_dataLen)
{
	SystemChange data;
	uint8_t datanum[1024] = {};
	int ret = 0;
	Input(datanum,&ret);

	uint16_t crc_ret = crc16table(datanum, ret);
	datanum[ret++] = crc_ret&0xff;
	datanum[ret++] = (crc_ret>>8)&0xff;
	cout << "要发送的消息帧" << datanum << endl;
	for (int i = 0; i < ret; i++)
	{
		uint8_t d[5] = {};
		data.nToHexstr(datanum[i], d, 2);
		cout << d <<" ";
	}
	cout << endl;
	if (wz.send(datanum, ret) == ret)
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

void receiveDemo(WzSerialPort& wz, int* receive_dataLen)
{
	SystemChange data;
	int ret = 30;
	uint8_t buf[1000] = {};
	int e = wz.receive(buf, ret);
	for (int i = 0; i < e; i++)
	{
		uint8_t r[10] = {};
		data.nToHexstr(buf[i],r,2);
		cout << r << " ";
	}
}

void main()
{
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 2; //读间隔超时
	TimeOuts.ReadTotalTimeoutMultiplier = 10; //读时间系数
	TimeOuts.ReadTotalTimeoutConstant = 5000; //读时间常量
	TimeOuts.WriteTotalTimeoutMultiplier = 500; // 写时间系数
	TimeOuts.WriteTotalTimeoutConstant = 2000; //写时间常量
	WzSerialPort w;
	bool open_sign = w.open(ComPort, 9600, 0, 8, 1, 1, &TimeOuts);
	if (!open_sign)
	{
		cout << "open serial port failed..." << endl;
		return;
	}
	int send_dataLen = 0;
	if(!sendDemo(w, &send_dataLen))
		return;
	cout << "读取数据中" << endl;
	int receive_dataLen = 0;
	receiveDemo(w, &receive_dataLen);
	system("pause");
	return;
}

