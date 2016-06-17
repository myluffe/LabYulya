#include "stdafx.h"
#include "lec.h"

lexeme::lexeme(char * name, char * type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap.get_mem(sizeof(char) * strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = GetType(type);
	_data = (char*)heap.get_mem(sizeof(char) * strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

lexeme::lexeme(char * name, int type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap.get_mem(sizeof(char) * strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = type;
	_data = (char*)heap.get_mem(sizeof(char) * strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

lexeme* lexeme::mass(lexeme* name, int type, List* val, int rank)
{
	lexeme* lex = new lexeme(name->Data(), type, nullptr, name->Line(), name->Start_Position(), name->Priority());
	lex->_rank = rank;
	lex->values = val;
}

lexeme::~lexeme()
{
	heap.free_mem(_name);
	heap.free_mem(_data);
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
	heap.free_mem(_data);
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
	heap.free_mem(_data);
	_data = (char*)heap.get_mem(sizeof(char) * strlen(newdata) + 1);
	strcpy_s(_data, strlen(newdata) + 1, newdata);
}

void lexeme::Print()
{   
	printf("\"%s\" Name: \"%s\" Type: \"%d\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"\n", _data, _name, _type, _line, _startposition, _priority);
}
