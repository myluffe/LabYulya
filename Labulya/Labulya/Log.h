#pragma once
#include "string.h"

static class Log
{
public:
	Log(char* FileName = "log.txt");
	void Write(char* Out); //������� ������ ������
	~Log();
private:
	char* _logfile; //����������� ���� ��� ������ ������ ��� ��������������. �� ��������� - �������.
}LogFile = *new Log();