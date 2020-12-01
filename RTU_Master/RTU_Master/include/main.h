#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include "Serial_port_API.h"
#include "stdint.h"
#include<iostream>
#include<string>
#include<vector>
#include <windows.h>
#include <process.h>
#include "Data_Transformation.h"
// portname(串口名): 在Windows下是"COM1""COM2"等，在Linux下是"/dev/ttyS1"等
// baudrate(波特率): 9600、19200、38400、43000、56000、57600、115200 
// parity(校验位): 0为无校验，1为奇校验，2为偶校验，3为标记校验（仅适用于windows)
// databit(数据位): 4-8(windows),5-8(linux)，通常为8位
// stopbit(停止位): 1为1位停止位，2为2位停止位,3为1.5位停止位


using namespace std;

//#define ComPort "COM3"
#define confirm 111




uint16_t crc16table(uint8_t* ptr, uint16_t len);