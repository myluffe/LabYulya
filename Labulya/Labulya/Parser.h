#pragma once

#include "heap.h" 

static class Parser
{
public:
	Parser();
	~Parser();
protected:
	int ToInt(char* str);
	double ToDouble(char* str);
	float ToFloat(char* str);

	char* IntToString(int value);
	char* DoubleToString(int value);
	char* FloatToString(int value);
};

static Parser = *new Parser();
