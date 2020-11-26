#include"..\include\main.h"


void sendDemo(WzSerialPort& wz)
{
	vector<uint16_t>dataIn;
	
	/*�û�����************************/
	uint16_t num;
	while (cin >> num)
	{
		if (confirm == num)
			break;
		dataIn.push_back(num);
	}
	/*У��*****************************/
	int length = dataIn.size();
	dataIn.push_back(CRC16(dataIn, length));
	/*����*****************************/
	
	

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

	return;
}