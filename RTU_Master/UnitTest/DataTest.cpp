#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"

#define MAX_LINE 1024

#define filename  "..\\RTU_Master\\test\\datatest.ini"
#define request  "RequestMessage"
#define requtBytenum  "RequestByteNum"
#define receive  "ReceiveMessage"
#define receveByte  "ReceiveBytenum"
#define output  "Output"

int SendBytenum = 0;
int ReceveBytenum = 0;

void DataHandle(char *section, unsigned char *sendbuffer, unsigned char *Recevebuffer)
{
	SystemChange data;
	/*请求数据读取处理*/
	char sendbuf[1024] = {};
	char buf[5] = {};
	SendBytenum = GetPrivateProfileIntA(section, requtBytenum, -1, filename);
	GetPrivateProfileStringA(section, request, NULL, sendbuf, 1024, filename);
	char *p = strtok(sendbuf," ");
	memcpy(buf, p, 2);
	int k = data.ChangeNum((unsigned char*)buf);
	sendbuffer[0] = k & 0xff;
	p = NULL;
	memset(buf, 0, 5);
	for (int i = 1; i < SendBytenum; i++)
	{
		p = strtok(NULL," ");
		memcpy(buf, p, 2);
		k = data.ChangeNum((unsigned char*)buf);
		sendbuffer[i] = k & 0xff;
		p = NULL;
		memset(buf, 0, 5);
	}

	/*接收数据处理*/
	char Recevebuf[1024] = {};
	ReceveBytenum = GetPrivateProfileIntA(section, receveByte, -1, filename);
	GetPrivateProfileStringA(section, receive, NULL, Recevebuf, 1024, filename);
	p = strtok(Recevebuf," ");
	memcpy(buf, p, 2);
	k = data.ChangeNum((unsigned char*)buf);
	Recevebuffer[0] = k & 0xff;
	p = NULL;
	memset(buf, 0, 5);
	for (int i = 1; i < ReceveBytenum; i++)
	{
		p = strtok(NULL," ");
		memcpy(buf, p, 2);
		k = data.ChangeNum((unsigned char*)buf);
		Recevebuffer[i] = k & 0xff;
		p = NULL;
		memset(buf, 0, 5);
	}
	return;
}


void RTUMaster_Test()
{
	char section1[MAX_LINE] = { '\0' };//section1--该文件下所有节名保存数组
	int a = 0;
	GetPrivateProfileSectionNamesA(section1, MAX_LINE, filename);
	char section[MAX_LINE] = { '\0' };//保存单个节名数组
	int time = 0;
	while ((section1[a] != '\0') || (section1[a + 1] != '\0'))
	{
		for (int k = a; section1[k] != '\0'; k++)
			section[time++] = section1[k];
		
		unsigned char sendbuffer[1024] = {};
		unsigned char Recevebuffer[1024] = {};

		DataHandle(section, sendbuffer, Recevebuffer);

		SystemChange data;
		WzSerialPort w;
		int TheoryBytenum = data.ReceiveLenth(sendbuffer);//理论接收长度


		if (!LenthJuage(ReceveBytenum, TheoryBytenum))
		{
			unsigned char outbuf[50] = {};
			GetPrivateProfileStringA(section, receive, NULL, (char*)outbuf, 50, filename);
			Assert::AreEqual((unsigned char)0, outbuf[0]);
			Logger::WriteMessage("长度错误");
		}
		else
		{
			unsigned short d = crc16table(Recevebuffer, ReceveBytenum - 2);
			unsigned short t = ((unsigned short)((Recevebuffer[ReceveBytenum - 1] & 0x00ff) << 8) | (unsigned short)(Recevebuffer[ReceveBytenum - 2] & 0x00ff));
			if (d != t)
			{
				Assert::AreNotEqual(d, t);
				Logger::WriteMessage("CRC不一致");
				goto loop;
			}

			if (!data.ErrorcodeJuage(sendbuffer, Recevebuffer, TheoryBytenum, ReceveBytenum))
			{
				unsigned char outbuf[50] = {};
				GetPrivateProfileStringA(section, output, NULL, (char*)outbuf, 50, filename);
				Assert::AreEqual((unsigned char)0, outbuf[0]);
				Logger::WriteMessage("other erroy");
			}
			else
			{
				char outbuf[500] = {};
				unsigned char outbuffer[500] = {};
				char buf[5] = {};
				GetPrivateProfileStringA(section, output, NULL, outbuf, 500, filename);
				char *p = strtok(outbuf," ");
				memcpy(buf, p, 2);
				int k = data.ChangeNum((unsigned char*)buf);
				outbuffer[0] = k & 0xff;
				p = NULL;
				memset(buf, 0, 5);
				int i = 1;
				while (1)
				{
					p = strtok(NULL," ");
					if (p == NULL)
						break;
					memcpy(buf, p, 2);
					k = data.ChangeNum((unsigned char*)buf);
					outbuffer[i++] = k & 0xff;
					p = NULL;
					memset(buf, 0, 5);
				}
				for (int j = 0; j < i; j++)
					Assert::AreEqual(Recevebuffer[j], outbuffer[j]);
			}
		}
loop:	a = a + time + 1;
		time = 0;
		memset(section, 0, sizeof(section));
	}
}