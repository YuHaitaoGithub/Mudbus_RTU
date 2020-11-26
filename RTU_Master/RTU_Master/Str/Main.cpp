#include"..\include\main.h"


void sendDemo(WzSerialPort& wz)
{
	vector<uint16_t>dataIn;
	
	/*用户输入************************/
	uint16_t num;
	while (cin >> num)
	{
		if (confirm == num)
			break;
		dataIn.push_back(num);
	}
	/*校验*****************************/
	int length = dataIn.size();
	dataIn.push_back(CRC16(dataIn, length));
	/*发送*****************************/
	
	

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