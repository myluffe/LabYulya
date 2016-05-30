#pragma once

#include "heap.h" 

static class Parser
{
public:
	Parser();
	~Parser();
	int ToInt(char* str);
	double ToDouble(char* str);
	float ToFloat(char* str);

	char* IntToString(int value);
	char* DoubleToString(double value);
	char* FloatToString(float value);

	bool ToBool(char* str);
	char* BoolToString(bool value);
};

static Parser parser = *new Parser();
