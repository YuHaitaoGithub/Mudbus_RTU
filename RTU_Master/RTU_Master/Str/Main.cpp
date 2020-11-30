#include"..\include\main.h"

uint8_t receiveBuf[1024] = {};
uint8_t SendBuf[1024] = {};


/*��������*************************************/
void Input(uint8_t* in_num,int* ret)
{
	SystemChange data;
	uint8_t l[10] = {};
	int num = 0;
	int num2 = 0;

	cout << "�����վ��ַ��ʮ��������" << endl;
	cin >> num;
	in_num[(*ret)++] = num&0xff;

	cout << "���빦����01��03��0f��10" << endl;
	cin >> l;
	num2 = data.ChangeNum(l);
	in_num[(*ret)++] = 0xff&num2;
	memset(l, 0, 10);

	switch (num2)
	{
	case 1:{
		cout << "������Ȧ��ʼ��ַ(ʮ��������)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "Ҫ����Ȧ������ʮ����������" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		break;
	}
	case 3:{
		cout << "����Ĵ�����ʼ��ַ��ʮ����������" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "Ҫ���Ĵ����ĸ���(ʮ��������)" << endl;
		cin >> num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		break;
	}
	case 15:{
		cout << "������Ȧ��ʼ��ַ(ʮ��������)" << endl;
		cin >> num;
		int i = num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "Ҫд�����Ȧ����(ʮ��������)" << endl;
		cin >> num;
		int RstNum = num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		num = num % 8 != 0 ? num = num / 8 + 1 : num /= 8;
		in_num[(*ret)++] = num & 0xff;
		char *filename = "E:\\Modbus agreement\\RTU_Master\\Coil.ini";
		char *section = "Coil";
		printf("���޸� %d �� %d ����ֵ", i, RstNum + i-1);
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
		cout << "����Ĵ�����ʼ��ַ(ʮ��������)" << endl;
		cin >> num;
		int i = num;
		in_num[(*ret)++] = (num >> 8) & 0xff;
		in_num[(*ret)++] = num & 0xff;

		cout << "Ҫд��Ĵ����ĸ���(ʮ��������)" << endl;
		cin >> num;
		int RstNum = num;
		in_num[(*ret)++] = (num>>8) & 0xff;
		in_num[(*ret)++] = num & 0xff;
		in_num[(*ret)++] = (num * 2) & 0xff;
		char *filename = "E:\\Modbus agreement\\RTU_Master\\Register.ini";
		char *section = "register";
		printf("���޸� %d �� %d ����ֵ", i, RstNum + i-1);
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




/*���ݷ���***********************************/
bool sendDemo(WzSerialPort& wz, int* send_dataLen)
{
	SystemChange data;
	int ret = 0;
	Input(SendBuf, &ret);

	uint16_t crc_ret = crc16table(SendBuf, ret);
	SendBuf[ret++] = crc_ret & 0xff;
	SendBuf[ret++] = (crc_ret >> 8) & 0xff;
	cout << "Ҫ���͵���Ϣ֡" << endl;
	for (int i = 0; i < ret; i++)
	{
		uint8_t d[5] = {};
		data.nToHexstr(SendBuf[i], d, 2);
		cout << d <<" ";
	}
	cout << endl;
	if (wz.send(SendBuf, ret) == ret)
	{
		cout << "�ѷ���" << endl;
		*send_dataLen = ret;
		return true;
	}
	else
	{
		cout << "����ʧ��" << endl;
		return false;
	}
}





/*���ݽ���***************************************/
void ReceiveDemo(WzSerialPort& wz, int send_numLen)
{
	SystemChange data;
	int bufLenth = data.ReceiveLenth(SendBuf);
	int retLenth = wz.receive(receiveBuf, bufLenth);
	if (NULL == receiveBuf)
	{
		cout << "��ȡ��ʱ" << endl;
		return;
	}
	uint16_t d = crc16table(receiveBuf, bufLenth-2);
	uint16_t t = ((uint16_t)((receiveBuf[bufLenth - 1] & 0x00ff) << 8) | (uint16_t)(receiveBuf[bufLenth - 2] & 0x00ff));
	if (d != t)
	{
		cout << "CRCУ�鲻һ��" << endl;
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




/*������************************************/
void main()
{
	/*��ʱʱ������*/
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 2; //�������ʱ
	TimeOuts.ReadTotalTimeoutMultiplier = 10; //��ʱ��ϵ��
	TimeOuts.ReadTotalTimeoutConstant = 5000; //��ʱ�䳣��
	TimeOuts.WriteTotalTimeoutMultiplier = 500; // дʱ��ϵ��
	TimeOuts.WriteTotalTimeoutConstant = 2000; //дʱ�䳣��
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
	cout << "��ȡ������" << endl;
	int receive_dataLen = 0;
	ReceiveDemo(w, send_dataLen);
	system("pause");
	return;
}

