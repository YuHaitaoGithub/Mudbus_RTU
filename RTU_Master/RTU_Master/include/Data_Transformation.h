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

	uint16_t SystemChange::ChangeNum(uint8_t* str);

	uint16_t ReceiveLenth(uint8_t* data);

	bool ErrorcodeJuage(uint8_t* Sdata, uint8_t* Rdata, int Buflen, int Rlen);

	bool LenthJuage(int rlen, int len);
};

#endif