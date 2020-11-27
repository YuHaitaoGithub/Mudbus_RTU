#ifndef _DATA_TRANSFORMATION_H
#define _DATA_TRANSFORMATION_H

#include <stdio.h>
#include <string.h>
#include"stdint.h"

class SystemChange{
public:
	SystemChange();
	~SystemChange();

	int ChangeNum(char* str, int length);

	void nToHexstr(uint8_t n, unsigned char * hexstr, uint8_t strlen);

};

#endif