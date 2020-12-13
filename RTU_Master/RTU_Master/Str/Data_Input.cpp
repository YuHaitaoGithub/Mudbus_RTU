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
			cout << "输入不合法,请重新输入" << endl;
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
		cout << "输入超过范围请重新输入" << endl;
		return -1;
	}
	else return retnum;
}


void DataInput::In_SlaveAddress()
{
	while (1)
	{
		cout << "输入从站地址1-247（十进制数）" << endl;
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
loop:cout << "输入功能码01、03、0f、10" << endl;
	cin >> l;
	if (strlen((char*)l) != 2){
		cout << "功能码输入不合法，重新输入" << endl; goto loop;
	}
	cin.sync();
	fun_code = data.ChangeNum(l);
	if (!(fun_code == 1 || fun_code == 3 || fun_code == 15 || fun_code == 16 || fun_code == 21)){
		cout << "功能码输入不合法，重新输入" << endl; goto loop;
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
		cout << "输入线圈起始地址0-1999(十进制整数)" << endl;
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
		cout << "要读线圈个数1-2000（十进制整数）" << endl;
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
		cout << "输入寄存器起始地址0-124（十进制整数）" << endl;
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
		cout << "要读寄存器的个数1-125(十进制整数)" << endl;
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
		cout << "输入寄存器起始地址0-1999（十进制整数）" << endl;
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
		cout << "要读寄存器的个数1-1968(十进制整数)" << endl;
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
		cout << "输入寄存器0-122之间的起始地址(十进制整数)" << endl;
		int retnum = InputJuage(122, 0);
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
		cout << "要写入寄存器1-123之间的的个数(十进制整数)" << endl;
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

void DataInput::RW_InJauge(int num)
{
	SystemChange data;
first:	char b[2048] = {};
	uint8_t a[100] = {};
	gets(b);
	int Len = *len;
	int i = 0;
	while (1){
		for (int j = 0;j < 2;)
		{
			if (b[i] == '\0'){ i = -1; break; }
			if (b[i] != ' ')
			{
				memcpy(a+j, b + i, 1);
				++j; 
			}
			++i;
		}
		if (i == -1)
		{
			if ((*len) - Len != num)
			{
				*len = Len;
				memset(buf + Len, 0, (*len) - Len);
				cout << "输入不合法，从新输入" << endl;
				goto first;
			}
			else break;
		}
		buf[(*len)++] = data.ChangeNum(a) & 0xff;
		memset(a, 0, 100);
	}
}