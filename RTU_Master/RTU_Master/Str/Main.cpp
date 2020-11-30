#include"..\include\main.h"

uint8_t receiveBuf[1024] = {};
uint8_t SendBuf[1024] = {};


/*数据输入*************************************/
void Input(uint8_t* in_num,int* ret)
{
	SystemChange data;
	uint8_t l[10] = {};
	int num = 0;
	int num2 = 0;

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
		int i = num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "要写入的线圈个数(十进制整数)" << endl;
		cin >> num;
		int RstNum = num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
		in_num[(*ret)++] = num & 0xff;
		char *filename = "E:\\Modbus agreement\\RTU_Master\\Coil.ini";
		char *section = "Coil";
		printf("请修改 %d 到 %d 的数值", i, RstNum + i-1);
		system("Coil.ini");
		while (num--)
		{
			int number = 0;
			int j = 0;
			for (i; j < 8 && RstNum;j++)
			{
				RstNum--;
				char k[5] = {};
				_itoa_s(i + j, k, 10);
				number = GetPrivateProfileIntA(section, k, -1, filename);
				in_num[*ret] = ((0xff & number) << (j)) | in_num[*ret];
			}
			*ret = *ret + 1;
			if (!RstNum)break;
			j = 0;
			i = i + 8;
		}
		break;
	}
	case 16:{
		cout << "输入寄存器起始地址(十进制整数)" << endl;
		cin >> num;
		int i = num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "要写入寄存器的个数(十进制整数)" << endl;
		cin >> num;
		int RstNum = num;
		in_num[(*ret)++] = (num>>8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		in_num[(*ret)++] = (num * 2) & 0xff;
		char *filename = "E:\\Modbus agreement\\RTU_Master\\Register.ini";
		char *section = "register";
		printf("请修改 %d 到 %d 的数值", i, RstNum + i-1);
		system("Register.ini");
		for (i; i < RstNum; i++, *ret = (*ret) + 2)
		{
			char k[10] = {};
			_itoa_s(i, k, 10);
			num = GetPrivateProfileIntA(section, k, -1, filename);
			in_num[*ret] = (0xff & (num >> 8));
			in_num[(*ret) + 1] = 0xff & num;
		}
		break;
	}
	}
}




/*数据发送***********************************/
bool sendDemo(WzSerialPort& wz, int* send_dataLen)
{
	SystemChange data;
	int ret = 0;
	Input(SendBuf, &ret);

	uint16_t crc_ret = crc16table(SendBuf, ret);
	SendBuf[ret++] = crc_ret & 0xff;
	SendBuf[ret++] = (crc_ret >> 8) & 0xff;
	cout << "要发送的消息帧" << endl;
	for (int i = 0; i < ret; i++)
	{
		uint8_t d[5] = {};
		data.nToHexstr(SendBuf[i], d, 2);
		cout << d <<" ";
	}
	cout << endl;
	if (wz.send(SendBuf, ret) == ret)
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





/*数据接收***************************************/
void ReceiveDemo(WzSerialPort& wz, int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);
	int retLenth = wz.receive(receiveBuf, bufLenth);
	if (NULL == receiveBuf)
	{
		cout << "读取超时" << endl;
		return;
	}
	uint16_t d = crc16table(receiveBuf, bufLenth-2);
	uint16_t t = ((uint16_t)((receiveBuf[bufLenth - 1] & 0x00ff) << 8) | (uint16_t)(receiveBuf[bufLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRC校验不一致" << endl;
		return;
	}
	if (!data.ErrorcodeJuage(SendBuf, receiveBuf, bufLenth, retLenth))
		return;
	for (int i = 0; i < retLenth; i++)
	{
		uint8_t r[10] = {};
		data.nToHexstr(receiveBuf[i], r, 2);
		cout << r << " ";
	}
}




/*主函数************************************/
void main()
{
	/*超时时间设置*/
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
	ReceiveDemo(w, send_dataLen);
	system("pause");
	return;
}

