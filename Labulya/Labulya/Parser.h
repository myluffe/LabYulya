#pragma once

#include <iostream>
#include "stdio.h"

static class Parser
{
public:
	Parser();
	~Parser();

	int ToInt(char* str); //Преобразует строку в значение INT
	double ToDouble(char* str); //Преобразует строку в значение DOUBLE
	float ToFloat(char* str); //Преобразует строку в значение FLOAT

	char* IntToString(int value); //Преобразует значение INT в строку
	char * DoubleToString(double value); //Преобразует значение DOUBLE в строку
	char * FloatToString(float value); //Преобразует значение FLOAT в строку

	bool ToBool(char* str); //Преобразует строку в значение BOOL
	char* BoolToString(bool value); //Преобразует значение BOOL в строку
} parser = *new Parser();
