#pragma once
#include "string.h"

class Log
{
public:
	Log(char* FileName);
	void Write(char* Out); //������� ������ ������
	~Log();
private:
	char* _logfile; //����������� ���� ��� ������ ������ ��� ��������������. �� ��������� - �������.
};

static Log LogFile = *new Log("log.txt");