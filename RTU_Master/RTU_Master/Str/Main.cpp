#include"..\include\main.h"



void Input(uint8_t* in_num,int* ret)
{
	SystemChange data;
	uint8_t l[3] = {};
	uint16_t num = 0;
	uint16_t num2 = 0;
	cout << "�����վ��ַ" << endl;
	cin >> num;
	in_num[(*ret)++] = 0xff & num;
	data.nToHexstr(num, l, 2);
	in_num[(*ret)++] = l[0];
	in_num[(*ret)++] = l[1];
	memset(l, 0, 2);

	cout << "���빦����" << endl;
	cin >> num;
	data.nToHexstr(num, l, 2);
	in_num[(*ret)++] = l[0];
	in_num[(*ret)++] = l[1];
	memset(l, 0, 2);

	switch (num)
	{
	case 1:{
		cout << "������Ȧ��ʼ��ַ" << endl;
		cin >> num;
		data.nToHexstr((num>>8)&0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num  & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "Ҫ����Ȧ����" << endl;
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
		cout << "����Ĵ�����ʼ��ַ" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "Ҫ���Ĵ����ĸ���" << endl;
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
		cout << "������Ȧ��ʼ��ַ" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "Ҫд�����Ȧ����" << endl;
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
			printf("Ҫд���%d���ֽڵ���ֵ\n", i - num);
			cin >> hex >> num2;
			data.nToHexstr(num2 & 0xff, l, 2);
			in_num[(*ret)++] = l[0];
			in_num[(*ret)++] = l[1];
			memset(l, 0, 2);
		}
		break;
	}
	case 16:{
		cout << "����Ĵ�����ʼ��ַ" << endl;
		cin >> num;
		data.nToHexstr((num >> 8) & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);
		data.nToHexstr(num & 0xff, l, 2);
		in_num[(*ret)++] = l[0];
		in_num[(*ret)++] = l[1];
		memset(l, 0, 2);

		cout << "Ҫд��Ĵ����ĸ���" << endl;
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
			printf("Ҫд���%d���Ĵ�������ֵ�ĸ�λ\n",i - num);
			cin >> hex >> num2;
			data.nToHexstr(num2 & 0xff, l, 2);
			in_num[(*ret)++] = l[0];
			in_num[(*ret)++] = l[1];
			memset(l, 0, 2);
			printf("Ҫд���%d���Ĵ�������ֵ�ĵ�λ\n", i - num);
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
	TimeOuts.ReadIntervalTimeout = 1000; //�������ʱ
	TimeOuts.ReadTotalTimeoutMultiplier = 500; //��ʱ��ϵ��
	TimeOuts.ReadTotalTimeoutConstant = 5000; //��ʱ�䳣��
	TimeOuts.WriteTotalTimeoutMultiplier = 500; // дʱ��ϵ��
	TimeOuts.WriteTotalTimeoutConstant = 2000; //дʱ�䳣��
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

