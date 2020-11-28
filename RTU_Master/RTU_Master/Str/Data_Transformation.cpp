#define _CRT_SECURE_NO_WARNINGS
#include "..\include\Data_Transformation.h"

SystemChange::SystemChange()
{

}

SystemChange::~SystemChange()
{

}

uint16_t* SystemChange::ChangeNum(char* str, int length, int* sign)
{
	char*  p = (char *)calloc(length + 1, sizeof(char));  //根据十六进制字符串的长度，这里注意数组不要越界
	uint16_t   num[16] = { 0 };
	uint16_t   count = 1;
	uint16_t*  result = (uint16_t *)calloc(length, sizeof(uint16_t));
	strcpy(p, str);
	char *revstr = (char *)calloc(3, sizeof(char));
	int len = length;
	length += 2;

	while (length -= 2)
	{
		strncpy(revstr, p, 2);
		p += 2;
		for (int i = 1; i >= 0; i--)
		{
			if ((revstr[i] >= '0') && (revstr[i] <= '9'))
				num[i] = revstr[i] - 48;//字符0的ASCII值为48
			else if ((revstr[i] >= 'a') && (revstr[i] <= 'f'))
				num[i] = revstr[i] - 'a' + 10;
			else if ((revstr[i] >= 'A') && (revstr[i] <= 'F'))
				num[i] = revstr[i] - 'A' + 10;
			else
				num[i] = 0;
			result[*sign] = result[*sign] + num[i] * count;
			count = count * 16;//十六进制(如果是八进制就在这里乘以8)    
		}
		*sign = *sign + 1;
		memset(revstr, 0, 2);
	}
	p = p - len;
	free(p);
	free(revstr);
	return result;
}

void SystemChange::nToHexstr(uint8_t n, uint8_t * hexstr, uint8_t strlen)
{
	uint8_t hexChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	uint8_t i;
	uint8_t dis;
	unsigned long nTemp = (long)n;
	for (i = 1; i <= strlen; i++)
	{
		dis = nTemp & 0x0f;
		hexstr[strlen - i] = hexChar[dis];
		nTemp = nTemp >> 4;
	}
	hexstr[strlen] = '\0';
}