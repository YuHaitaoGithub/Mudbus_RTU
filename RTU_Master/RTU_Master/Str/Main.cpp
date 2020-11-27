#include"..\include\main.h"



void Input(vector<uint8_t>& in_num)
{
	uint16_t num = 0;
	
	cout << "�����վ��ַ" << endl;
	cin >>hex>> num;
	in_num.push_back(0x00ff & num);
	cout << "���빦����" << endl;
	cin >>num;
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
	}
}


void sendDemo(WzSerialPort& wz)
{
	vector<uint8_t>dataIn;
	uint16_t CRC_num;
	/*�ȴ�����*******************/
	Input(dataIn);
	uint16_t len = dataIn.size();

	/*У����********************/
	CRC_num = crc16table(dataIn, len);

	/*���ݴ��******************/
	dataIn.push_back(0x00ff & CRC_num);
	dataIn.push_back((0x00ff & (CRC_num>>8)));
	len = dataIn.size();
	uint8_t* pp = new uint8_t[len + 1];
	if (!dataIn.empty())
	{
		memcpy(pp, &dataIn[0], len*sizeof(uint8_t));
	}

	/*���ݷ���**********************/
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
	TimeOuts.ReadIntervalTimeout = 1000; //�������ʱ
	TimeOuts.ReadTotalTimeoutMultiplier = 500; //��ʱ��ϵ��
	TimeOuts.ReadTotalTimeoutConstant = 5000; //��ʱ�䳣��
	TimeOuts.WriteTotalTimeoutMultiplier = 500; // дʱ��ϵ��
	TimeOuts.WriteTotalTimeoutConstant = 2000; //дʱ�䳣��
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

