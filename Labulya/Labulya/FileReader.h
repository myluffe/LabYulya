#pragma once

#include "string.h"

#define String 100
#define FileName 50

using namespace std; 

class mFileReader
{
private:
	char _lastreadstring[String];
	char _filename[FileName];
	int _currentline;
	int _currentposiotion;
	FILE *file;
	bool _end = false;
public:
	bool EndFile(); //���������� true, ���� ���� ����������
	mFileReader(char* filename); //����������� ��������� ��� ����� ��� ������
	int CurrentLine(); //���������� ����� ��������� ����������� ������
	int CurrentPosition(); //���������� ����� �������, �� ������� ������������ ������, �� ������� �� ������
	char* ReadNextLine(); //������ ��������� ������ �� �����, ���������� ��������� �� ��
	~mFileReader();
};