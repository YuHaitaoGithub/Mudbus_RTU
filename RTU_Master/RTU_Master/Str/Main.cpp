#include"..\include\main.h"



void Input(vector<uint8_t>& in_num)
{
	SystemChange data;
	uint16_t num = 0;
	uint16_t num2 = 0;
	cout << "�����վ��ַ" << endl;
	cin >> num;
	in_num.push_back(0x00ff & num);
	cout << "���빦����" << endl;
	cin >> num;
	in_num.push_back(0x00ff & num);
	switch (num)
	{
	case 1:{
		cout << "������Ȧ��ʼ��ַ" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "Ҫ����Ȧ����" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		break;
	}
	case 3:{
		cout << "����Ĵ�����ʼ��ַ" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "Ҫ���Ĵ����ĸ���" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		break;
	}
	case 15:{
		cout << "������Ȧ��ʼ��ַ" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "Ҫд�����Ȧ����" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
		in_num.push_back(0x00ff & num);
		while (num--)
		{
			uint16_t i = num + 1;
			printf("Ҫд���%d���ֽڵ���ֵ\n", i - num);
			cin >> hex >> num2;
			in_num.push_back(0x00ff & num2);
		}
		break;
	}
	case 16:{
		cout << "����Ĵ�����ʼ��ַ" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		cout << "Ҫд��Ĵ����ĸ���" << endl;
		cin >> num;
		in_num.push_back((0x00ff & (num >> 8)) & num);
		in_num.push_back(0x00ff & num);
		num *= 2;
		in_num.push_back(0x00ff & num);
		num /= 2;
		while (num--)
		{
			uint16_t i = num+1;
			printf("Ҫд���%d���Ĵ�������ֵ�ĸ�λ\n",i - num);
			cin >> hex >> num2;
			in_num.push_back(0x00ff & num2);
			printf("Ҫд���%d���Ĵ�������ֵ�ĵ�λ\n", i - num);
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

