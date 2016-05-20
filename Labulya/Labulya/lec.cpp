#include "stdafx.h"
#include "lec.h"

lexeme::lexeme(char * name, char * type, char * data, int line, int startposition, int priority)
{
	strcpy_s(_name, name);
	strcpy_s(_type, type);
	strcpy_s(_data, data);
	_line = line;
	_startposition = startposition;
	_priority = priority;
}

void lexeme::print()
{
	printf("Name: \"%s\" Type: \"%s\" Data: \"%s\" Line: \"%sd\" Start Position: \"%d\" Priority: \"%d\"\n", _name, _type, _data, _line, _startposition, _priority);
}
