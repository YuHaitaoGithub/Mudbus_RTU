#define _CRT_SECURE_NO_WARNINGS
#include "..\include\Data_Transformation.h"

SystemChange::SystemChange()
{

}

SystemChange::~SystemChange()
{

}

uint16_t SystemChange::ChangeNum(uint8_t* str)
{
	int   num[16] = { 0 };
	int   count = 1;
	uint16_t   result = 0;
	for (int i = 1; i >= 0; i--)
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
			num[i] = str[i] - 48;//�ַ�0��ASCIIֵΪ48
		else if ((str[i] >= 'a') && (str[i] <= 'f'))
			num[i] = str[i] - 'a' + 10;
		else if ((str[i] >= 'A') && (str[i] <= 'F'))
			num[i] = str[i] - 'A' + 10;
		else
			num[i] = 0;
		result = result + num[i] * count;
		count = count * 16;//ʮ������(����ǰ˽��ƾ����������8)    
	}
	return result;
}


uint16_t SystemChange::ReceiveLenth(uint8_t* data)
{
	uint16_t d = 0x00ff & data[1];
	switch (d)
	{
	case 1:{
		d = (((data[4] & 0x00ff)<<8)| (data[5] & 0x00ff));
		d = d % 8 == 0 ? d / 8 : d / 8 + 1;
		d += 5;
		break;
	}
	case 3:{
		d = (((data[4] & 0x00ff) << 8) | (data[5] & 0x00ff));
		d = d * 2 + 5;
		break;
	}
	case 15:{d = 8;break;}
	case 16:{d = 8;break;}
	}
	return d;
}


bool SystemChange::ErrorcodeJuage(uint8_t* Sdata, uint8_t* Rdata, int Buflen, int Rlen)
{
	if ((uint16_t)(Sdata[0] & 0x00ff) != (uint16_t)(Rdata[0] & 0x00ff))
	{
		cout << "��վ�Ų���ȷ" << endl;
		return false;
	}
	if (Rlen < Buflen)
	{
		if (((uint16_t)(Rdata[1] & 0x00ff)) != ((uint16_t)((Sdata[1] & 0x00ff) + 0x80)))
		{
			cout << "�쳣�����벻��ȷ" << endl;
			return false;
		}
		else
		{
			if (Rlen == 5)
			{
				switch ((uint16_t)(Rdata[2] & 0x00ff))
				{
				case 1:{cout << "�Ƿ������� �쳣��01" << endl; return true;; }
				case 2:{cout << "�Ƿ����ݵ�ַ �쳣��02" << endl; return true;; }
				case 3:{cout << "�Ƿ�����ֵ �쳣��03" << endl; return true;; }
				case 4:{cout << "��վ�豸���� �쳣��04" << endl; return true;; }
				}
				cout << "�쳣����" << endl;
				return false;
			}
			cout << "�쳣����" << endl;
			return false;
		}
	}
	else if (Rlen == Buflen)
	{
		if (((uint16_t)(Rdata[1] & 0x00ff)) != ((uint16_t)(Sdata[1] & 0x00ff)))
		{
			cout << "���������" << endl;
			return false;
		}
		switch ((uint16_t)(Rdata[1] & 0x00ff))
		{

		case 1:{
			uint16_t d = (((Sdata[4] & 0x00ff) << 8) | (Sdata[5] & 0x00ff));
			d = d % 8 == 0 ? d / 8 : d / 8 + 1;
			if (((uint16_t)(Rdata[2] & 0x00ff)) != d)
			{
				cout << "�ֽ�������" << endl;
				return false;
			}
			break;
		}
		case 3:{
			uint16_t d = (((Sdata[4] & 0x00ff) << 8) | (Sdata[5] & 0x00ff));
			d = d * 2;
			if (((uint16_t)(Rdata[2] & 0x00ff)) != d)
			{
				cout << "�ֽ�������" << endl;
				return false;
			}
			break;
		}
		case 15:{
			if ((uint16_t)((uint16_t)((Sdata[2] & 0x00ff) << 8) | (uint16_t)(Sdata[3] & 0x00ff)) !=
				(uint16_t)((uint16_t)((Rdata[2] & 0x00ff) << 8) | (uint16_t)(Rdata[3] & 0x00ff)))
			{
				cout << "�Ĵ�����ʼ��ַ����" << endl;
				return false;
			}
			if ((uint16_t)((uint16_t)((Sdata[4] & 0x00ff) << 8) | (uint16_t)(Sdata[5] & 0x00ff)) !=
				(uint16_t)((uint16_t)((Rdata[4] & 0x00ff) << 8) | (uint16_t)(Rdata[5] & 0x00ff)))
			{
				cout << "�Ĵ�����������" << endl;
				return false;
			}
			break;
		}
		case 16:{
			if ((uint16_t)((uint16_t)((Sdata[2] & 0x00ff) << 8) | (uint16_t)(Sdata[3] & 0x00ff)) !=
				(uint16_t)((uint16_t)((Rdata[2] & 0x00ff) << 8) | (uint16_t)(Rdata[3] & 0x00ff)))
			{
				cout << "�Ĵ�����ʼ��ַ����" << endl;
				return false;
			}
			if ((uint16_t)((uint16_t)((Sdata[4] & 0x00ff) << 8) | (uint16_t)(Sdata[5] & 0x00ff)) !=
				(uint16_t)((uint16_t)((Rdata[4] & 0x00ff) << 8) | (uint16_t)(Rdata[5] & 0x00ff)))
			{
				cout << "�Ĵ�����������" << endl;
				return false;
			}
			break;
		}
		}
	}
	return true;
}


bool SystemChange::LenthJuage(int rlen, int len)
{
	if (rlen > len)
	{
		cout << "���Ȳ��Ϸ�" << endl;
		return false;
	}
	if (rlen == 0)
	{
		cout << "��ȡ��ʱ,���س�����" << endl;
		return false;
	}
	if (rlen < 5)
	{
		cout << "���ȹ���" << endl;
		return false;
	}
	return true;
}