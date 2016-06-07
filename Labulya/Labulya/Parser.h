#pragma once

#include <iostream>
#include "stdio.h"

static class Parser
{
public:
	Parser();
	~Parser();

	int ToInt(char* str); //����������� ������ � �������� INT
	double ToDouble(char* str); //����������� ������ � �������� DOUBLE
	float ToFloat(char* str); //����������� ������ � �������� FLOAT

	char* IntToString(int value); //����������� �������� INT � ������
	char * DoubleToString(double value); //����������� �������� DOUBLE � ������
	char * FloatToString(float value); //����������� �������� FLOAT � ������

	bool ToBool(char* str); //����������� ������ � �������� BOOL
	char* BoolToString(bool value); //����������� �������� BOOL � ������
} parser = *new Parser();
