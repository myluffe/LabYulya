#pragma once
#include "string.h"

static class Log
{
public:
	Log(char* FileName = "log.txt");
	void Write(char* Out); //������� ������ ������
	~Log();
private:
	FILE* logfile; //����������� ���� ��� ������ ������ ��� ��������������. �� ��������� - �������.
}LogFile = *new Log();