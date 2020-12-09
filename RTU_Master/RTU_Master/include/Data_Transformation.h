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

	uint16_t SystemChange::ChangeNum(uint8_t* str);//16进制字符串转整数

	uint16_t ReceiveLenth(uint8_t* data);//理论接收字节长度计算

	bool ErrorcodeJuage(uint8_t* Sdata, uint8_t* Rdata, int Buflen, int Rlen);//接收报文解析

	bool LenthJuage(int rlen, int len);//接收报文长度判断

};

#endif