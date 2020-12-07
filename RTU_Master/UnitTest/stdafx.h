#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "targetver.h"

// 用于 CppUnitTest 的头文件
#include "CppUnitTest.h"

// TODO:  在此处引用程序需要的其他头文件
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