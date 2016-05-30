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
	char* number = (char*)heap.get_mem(sizeof(char) * strlen(str) + 1);
	strcpy_s(number, strlen(str) + 1, str);
	//...
	return -1;
}

double Parser::ToDouble(char * str)
{
	char* number = (char*)heap.get_mem(sizeof(char) * strlen(str) + 1);
	strcpy_s(number, strlen(str) + 1, str);
	//...
	return -1.0;
}

float Parser::ToFloat(char * str)
{
	char* number = (char*)heap.get_mem(sizeof(char) * strlen(str) + 1);
	strcpy_s(number, strlen(str) + 1, str);
	//...
	return -1.0f;
}

char * Parser::IntToString(int value)
{
	//...

	//int count; //длина числа
	//char* str = (char*)heap.get_mem(sizeof(char) * (count) + 1 ); //её возвращать
	return nullptr;
}

char * Parser::DoubleToString(int value)
{
	//...

	//int count; //длина числа
	//char* str = (char*)heap.get_mem(sizeof(char) * (count) + 1 ); //её возвращать
	return nullptr;
}

char * Parser::FloatToString(int value)
{
	//...

	//int count; //длина числа
	//char* str = (char*)heap.get_mem(sizeof(char) * (count) + 1 ); //её возвращать
	return nullptr;
}
