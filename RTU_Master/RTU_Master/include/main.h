#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include "Serial_port_API.h"
#include "stdint.h"
#include<iostream>
#include<sstream>
#include<string>
#include<set>
#include<map>
#include<vector>
#include <windows.h>
#include <process.h>
#include "Data_Transformation.h"
#include "Data_Input.h"

using namespace std;


uint16_t crc16table(uint8_t* ptr, uint16_t len);//CRCУ��