#ifndef _DATA_TRANSFORMATION_H
#define _DATA_TRANSFORMATION_H

#include <stdio.h>
#include <string.h>
#include"stdint.h"
#include "malloc.h"
#include <iostream>

using namespace std;

class SystemChange{
public:
	SystemChange();
	~SystemChange();

	uint16_t SystemChange::ChangeNum(uint8_t* str);//16�����ַ���ת����

	uint16_t ReceiveLenth(uint8_t* data);//���۽����ֽڳ��ȼ���

	bool ErrorcodeJuage(uint8_t* Sdata, uint8_t* Rdata, int Buflen, int Rlen);//���ձ��Ľ���

	bool LenthJuage(int rlen, int len);//���ձ��ĳ����ж�

};

#endif