#pragma once
#include "string.h"

static class Log
{
public:
	Log(char* FileName);
	void Write(char* Out); //������� ������ ������
	~Log();
private:
	char* _logfile; //����������� ���� ��� ������ ������ ��� ��������������. �� ��������� - �������.
};

Log LogFile = *new Log("log.txt");