#include "stdafx.h"
#include "Parser.h"


Parser::Parser()
{
}

Parser::~Parser()
{
}

int Parser::ToInt(char * str)
{
	char* number = (char*)heap.get_mem(sizeof(char) * ((int)strlen(str) + 1));
	strcpy_s(number, strlen(str) + 1, str);
	int res = atoi(number);
	heap.free_mem(number);
	return res;
}

double Parser::ToDouble(char * str)
{
	char* number = (char*)heap.get_mem(sizeof(char) * ((int)strlen(str) + 1));
	strcpy_s(number, strlen(str) + 1, str);
	double res = atof(number);
	heap.free_mem(number);
	return res;
}

float Parser::ToFloat(char * str)
{
	char* number = (char*)heap.get_mem(sizeof(char) * ((int)strlen(str) + 1));
	strcpy_s(number, strlen(str) + 1, str);
	long temp = 0;
	float res = _Stof(number, nullptr, temp);
	heap.free_mem(number);
	return res;
}

char * Parser::IntToString(int value)
{
	int buf = value;
	int count = 0; //длина числа
	while (buf!= 0) 
	{
		value /= 10;
		count++;
	}
	char* str = (char*)heap.get_mem(sizeof(char) * (count + 1) );
	char* res = (char*)_itoa_s(value, str, count, 10);
	heap.free_mem(str);
	return res;
}

char * Parser::DoubleToString(double value)
{
	int decimal, sign;
	char* res = (char*)heap.get_mem(_CVTBUFSIZE);
	int buf = _ecvt_s(res, _CVTBUFSIZE, value, 10, &decimal, &sign);
	if (buf != 0)
		return nullptr;
	return res;
}

char * Parser::FloatToString(float value)
{
	int decimal, sign;
	char* res = (char*)heap.get_mem(_CVTBUFSIZE);
	int buf = _fcvt_s(res, _CVTBUFSIZE, value, 100, &decimal, &sign);
	if (buf != 0)
		return nullptr;
	return res;
}

bool Parser::ToBool(char * str)
{
	if (strcmp(str, "true") == 0)
		return true;
	return false;
}

char * Parser::BoolToString(bool value)
{
	char* temp = (char*)heap.get_mem(sizeof(char) * (6));
	if (value)
		strcpy_s(temp, 6, "true");
	else strcpy_s(temp, 6, "false");
	return temp;
}
