#define _CRT_SECURE_NO_WARNINGS
#include "..\include\Data_Input.h"

DataInput::DataInput()
{

}

DataInput::~DataInput()
{

}

int DataInput::InputJuage(int numMax,int numMin)
{
	char ch = '0';
	memset(inbuf, 0, 10);
	int i = 0;
	while (1)
	{
		ch = getchar();
		if (ch == '\n')break;
		if (i > 4 || ch - '0' < 0 || ch - '0' > 9)
		{
			cout << "���벻�Ϸ�,����������" << endl;
			memset(inbuf, 0, 10);
			for (; ch != '\n';)
				ch = getchar();
			i = 0;
			continue;
		}
		inbuf[i++] = ch;
	}
	int retnum = atoi(inbuf);
	if (retnum > numMax || retnum < numMin)
	{
		cout << "���볬����Χ����������" << endl;
		return -1;
	}
	else return retnum;
}


void DataInput::In_SlaveAddress()
{
	while (1)
	{
		cout << "�����վ��ַ1-247��ʮ��������" << endl;
		int retnum = InputJuage(247, 1);
		if (retnum == -1)
			continue;
		else {
			buf[(*len)++] = 0xff & retnum;
			break;
		}
	}
}

int DataInput::FunctionCode()
{
	int fun_code = 0;
	SystemChange data;
	unsigned char l[50];
loop:cout << "���빦����01��03��0f��10" << endl;
	cin >> l;
	if (strlen((char*)l) != 2){
		cout << "���������벻�Ϸ�����������" << endl; goto loop;
	}
	cin.sync();
	fun_code = data.ChangeNum(l);
	if (!(fun_code == 1 || fun_code == 3 || fun_code == 15 || fun_code == 16 || fun_code == 21)){
		cout << "���������벻�Ϸ�����������" << endl; goto loop;
	}
	if (fun_code == 21)fun_code = 15;
	buf[(*len)++] = 0xff & fun_code;
	memset(l, 0, 50);
	return fun_code;
}

void DataInput::In_01_Data()
{
	while (1)
	{
		cout << "������Ȧ��ʼ��ַ0-1999(ʮ��������)" << endl;
		int retnum = InputJuage(1999, 0);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			break;
		}
	}
	while (1)
	{
		cout << "Ҫ����Ȧ����1-2000��ʮ����������" << endl;
		int retnum = InputJuage(2000, 1);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			break;
		}
	}
}


void DataInput::In_03_Data()
{
	while (1)
	{
		cout << "����Ĵ�����ʼ��ַ0-124��ʮ����������" << endl;
		int retnum = InputJuage(124, 0);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			break;
		}
	}
	while (1)
	{
		cout << "Ҫ���Ĵ����ĸ���1-125(ʮ��������)" << endl;
		int retnum = InputJuage(125, 1);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			break;
		}
	}
}



void DataInput::In_0f_Data(int *lpaddr, int *lpCoilnum)
{
	while (1)
	{
		cout << "����Ĵ�����ʼ��ַ0-1999��ʮ����������" << endl;
		int retnum = InputJuage(1999, 0);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			*lpaddr = retnum;
			break;
		}
	}
	while (1)
	{
		cout << "Ҫ���Ĵ����ĸ���1-1968(ʮ��������)" << endl;
		int retnum = InputJuage(1968, 1);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			*lpCoilnum = retnum;
			break;
		}
	}
}



void DataInput::In_10_Data(int *lprestaddr, int *lpRestnum)
{
	while (1)
	{
		cout << "����Ĵ���0-124֮�����ʼ��ַ(ʮ��������)" << endl;
		int retnum = InputJuage(124, 0);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			*lprestaddr = retnum;
			break;
		}
	}
	while (1)
	{
		cout << "Ҫд��Ĵ���1-123֮��ĵĸ���(ʮ��������)" << endl;
		int retnum = InputJuage(123, 1);
		if (retnum == -1)
			continue;
		else{
			buf[(*len)++] = (retnum >> 8) & 0xff;
			buf[(*len)++] = retnum & 0xff;
			*lpRestnum = retnum;
			break;
		}
	}
}