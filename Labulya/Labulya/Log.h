#pragma once
#include "string.h"

static class Log
{
public:
	Log();
	void Write(char* Out); //������� ������ ������
	~Log();
}LogFile = Log();

static char LogFileName[] = "log.txt"; //����������� ���� ��� ������ ������ ��� ��������������. �� ��������� - �������.
