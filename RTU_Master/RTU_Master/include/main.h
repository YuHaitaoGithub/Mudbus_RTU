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
// portname(������): ��Windows����"COM1""COM2"�ȣ���Linux����"/dev/ttyS1"��
// baudrate(������): 9600��19200��38400��43000��56000��57600��115200 
// parity(У��λ): 0Ϊ��У�飬1Ϊ��У�飬2ΪżУ�飬3Ϊ���У�飨��������windows)
// databit(����λ): 4-8(windows),5-8(linux)��ͨ��Ϊ8λ
// stopbit(ֹͣλ): 1Ϊ1λֹͣλ��2Ϊ2λֹͣλ,3Ϊ1.5λֹͣλ


using namespace std;

//#define ComPort "COM3"
#define confirm 111




uint16_t crc16table(uint8_t* ptr, uint16_t len);