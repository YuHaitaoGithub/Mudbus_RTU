#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include "Serial_port_API.h"
#include "stdint.h"
#include<iostream>
#include<string>
#include<vector>
#include <windows.h>


using namespace std;

#define ComPort "\\\\.\\COM12"
#define confirm 111




uint16_t CRC16(uint8_t *buf, int length);