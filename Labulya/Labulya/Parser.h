#pragma once

#include <iostream>
#include "stdio.h"

static class Parser
{
public:
	Parser();
	~Parser();

	static int ToInt(char* str);
	double ToDouble(char* str);
	float ToFloat(char* str);

	char* IntToString(int value);
	char * DoubleToString(double value);
	char * FloatToString(float value);

	bool ToBool(char* str);
	char* BoolToString(bool value);
} parser = *new Parser();
