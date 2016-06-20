#include "stdafx.h"
#include "lec.h"

lexeme::lexeme(char * name, char * type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap.get_mem(sizeof(char) * (int)strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = GetType(type);
	_data = (char*)heap.get_mem(sizeof(char) * (int)strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

lexeme::lexeme(char * name, int type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap.get_mem(sizeof(char) * (int)strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = type;
	_data = (char*)heap.get_mem(sizeof(char) * (int)strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

void lexeme::ToMass(lexeme* name, int type, lexeme* val, int rank, List* sizes)
{
	Sizes = sizes;
	heap.free_mem(_name);
	_name = (char*)heap.get_mem(sizeof(char) * ((int)strlen(name->Data()) + 1));
	strcpy_s(_name, (strlen(name->Data()) + 1), name->Data());
	_type = type;
	heap.free_mem(_data);
	_data = "";
	_line = name->Line();
	_startposition = name->Start_Position();
	_priority = name->Priority();
	_rank = rank;
	Values = val;
}

lexeme::~lexeme()
{
	heap.free_mem(_name);
	heap.free_mem(_data);
	heap.free_mem(Values);
	if(Sizes != nullptr)
		Sizes->~List();
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
	if (Type() == INT)
	{
		int a = parser.ToInt(newdata);
		_data = (char*)heap.get_mem(sizeof(char) * (int)strlen(newdata) + 1);
		strcpy_s(_data, strlen(newdata) + 1, parser.IntToString(a));
		return;
	}
	if (Type() == FLOAT)
	{
		int a = parser.ToFloat(newdata);
		_data = (char*)heap.get_mem(sizeof(char) * (int)strlen(newdata) + 1);
		strcpy_s(_data, strlen(newdata) + 1, parser.FloatToString(a));
		return;
	}
	if (Type() == CHAR)
	{
		_data = (char*)heap.get_mem(sizeof(char));
		char a = (char)newdata;
		sprintf_s(_data, 1, "%d", a);
		return;
	}
	_data = (char*)heap.get_mem(sizeof(char) * (int)strlen(newdata) + 1);
	strcpy_s(_data, strlen(newdata) + 1, newdata);
}

void lexeme::Print()
{   
	if (_rank == 0)
		printf("\"%s\" Name: \"%s\" Type: \"%d\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"\n", _data, _name, _type, _line, _startposition, _priority);
	else
	{
		printf("Array Name: \"%s\" Type: \"%d\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"\n", _data, _name, _type, _line, _startposition, _priority);
		printf("Data: ");
		int q = 1;
		for (int l = 0; l < Sizes->count(); l++)
			q *= *(int*)Sizes->get(l);
		for (int l = 0; l < q; l++)
		{
			lexeme* temp = &Values[l];
			printf("\"%s\" Name: \"%s\" Type: \"%d\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"", _data, _name, _type, _line, _startposition, _priority);
			if (l != q - 1)
				printf(", ");
		}
		printf(";\n");
	}
}
