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

	int InputJuage(int numMax, int numMin);//Ϲ�����ж�

	void In_SlaveAddress();//��ʼ��ַ�ж�

	int FunctionCode();//�������ж�

	void In_01_Data();//������1�ж�

	void In_03_Data();//������3�ж�

	void In_0f_Data(int *lpaddr,int *lpCoilnum);//������15�ж�

	void In_10_Data(int *lprestaddr, int *lpRestnum);//������16�ж�

	void RW_InJauge(int num);

	int *len;//�ֽ���
	uint8_t *buf;//������ָ��

private:
	char inbuf[10];

};












#endif