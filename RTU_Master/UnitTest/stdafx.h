#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#include "targetver.h"

// ���� CppUnitTest ��ͷ�ļ�
#include "CppUnitTest.h"

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include"Windows.h"
#include"string.h"
#include "..\\RTU_Master\\include\\Data_Transformation.h"
#include "..\\RTU_Master\\include\\main.h"
#include "..\\RTU_Master\\include\\Serial_port_API.h"
#include "..\\RTU_Master\\include\\stdint.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

void RTUMaster_Test();
void DataHandle(char *section, unsigned char *sendbuffer, unsigned char *Recevebuffer);