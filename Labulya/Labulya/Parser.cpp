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
	char* res = (char*)heap.get_mem(sizeof(char) * 20 );
	sprintf_s(res, sizeof(char) * 20, "%d", value);
	return res;
}

char * Parser::DoubleToString(double value)
{
	int decimal, sign;
	char* res = (char*)heap.get_mem(_CVTBUFSIZE);
	sprintf_s(res, _CVTBUFSIZE, "%lf", value);
	return res;
}

char * Parser::FloatToString(float value)
{
	int decimal, sign;
	char* res = (char*)heap.get_mem(_CVTBUFSIZE);
	sprintf_s(res, _CVTBUFSIZE, "%f", value);
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
