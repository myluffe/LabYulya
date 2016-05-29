#include "stdafx.h"
#include "lec.h"
#include "heap.h"

lexeme::lexeme(char * name, char * type, char * data, int line, int startposition, int priority)
{
	_name = (char*)heap.get_mem(sizeof(char) * strlen(name) + 1);
	strcpy_s(_name, strlen(name) + 1, name);
	_type = (char*)heap.get_mem(sizeof(char) * strlen(type) + 1);
	strcpy_s(_type, strlen(type) + 1, type);
	_data = (char*)heap.get_mem(sizeof(char) * strlen(data) + 1);
	strcpy_s(_data, strlen(data) + 1, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

lexeme::~lexeme()
{
	heap.free_mem(_name);
	heap.free_mem(_type);
	heap.free_mem(_data);
}

void lexeme::Print()
{
	printf("\"%s\" Name: \"%s\" Type: \"%s\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"\n", _data, _name, _type, _line, _startposition, _priority);
}
