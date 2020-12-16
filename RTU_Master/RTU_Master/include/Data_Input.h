#ifndef _DATA_INPUT_H_
#define _DATA_INPUT_H_

#include <stdio.h>
#include <string.h>
#include"stdint.h"
#include "malloc.h"
#include <iostream>
#include "Data_Transformation.h"

#define CoilNum_Max   1999
#define RestNum_Max   124
//#include<sstream>

using namespace std;


class DataInput
{
public:
	DataInput();
	~DataInput();

	int InputJuage(int numMax, int numMin);//瞎输入判断

	void In_SlaveAddress();//起始地址判断

	int FunctionCode();//功能码判断

	void In_01_Data();//功能码1判断

	void In_03_Data();//功能码3判断

	void In_0f_Data(int *lpaddr,int *lpCoilnum);//功能码15判断

	void In_10_Data(int *lprestaddr, int *lpRestnum);//功能码16判断

	void RW_InJauge(int num);

	int *len;//字节数
	uint8_t *buf;//请求报文指针

private:
	char inbuf[10];

};












#endif