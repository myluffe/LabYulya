#include "stdafx.h"
#include "lec.h"

lexeme::lexeme(char * name, char * type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap_my.get_mem(sizeof(char) * (int)strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = GetType(type);
	_data = (char*)heap_my.get_mem(sizeof(char) * (int)strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

lexeme::lexeme(char * name, int type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap_my.get_mem(sizeof(char) * (int)strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = type;
	_data = (char*)heap_my.get_mem(sizeof(char) * (int)strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

void lexeme::ToMass(lexeme* name, int type, lexeme* val, int rank)
{
	heap_my.free_mem(_name);
	_name = (char*)heap_my.get_mem(sizeof(char) * ((int)strlen(name->Data()) + 1));
	strcpy_s(_name, (strlen(name->Data()) + 1), name->Data());
	_type = type;
	heap_my.free_mem(_data);
	_data = "";
	_line = name->Line();
	_startposition = name->Start_Position();
	_priority = name->Priority();
	_rank = rank;
	Values = val;
}

lexeme::~lexeme()
{
	heap_my.free_mem(_name);
	heap_my.free_mem(_data);
	heap_my.free_mem(Values);
}

bool lexeme::GetNumber(double * value)
{
	if (_type == INT || _type == DOUBLE || _type == FLOAT)
	{
		double temp = Parser().ToDouble(_data);
		*value = temp;
		return true;
	}
	return false;
}

bool lexeme::SetNumber(double value)
{
	heap_my.free_mem(_data);
	int temp = 0;
	if (_type == INT)
	{
		temp = (int)value;
		_data = Parser().IntToString(temp);
		return true;
	}
	if (_type == FLOAT || _type == DOUBLE)
	{
		_data = Parser().DoubleToString(value);
		return true;
	}
	return false;
}

void lexeme::DataChange(char* newdata)
{
	heap_my.free_mem(_data);
	_data = (char*)heap_my.get_mem(sizeof(char) * (int)strlen(newdata) + 1);
	strcpy_s(_data, strlen(newdata) + 1, newdata);
}

void lexeme::Print()
{   
	printf("\"%s\" Name: \"%s\" Type: \"%d\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"\n", _data, _name, _type, _line, _startposition, _priority);
}
