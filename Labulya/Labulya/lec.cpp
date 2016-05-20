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

void lexeme::Print()
{
	printf("\"%s\" Name: \"%s\" Type: \"%s\" Line: \"%d\" Start Position: \"%d\" Priority: \"%d\"\n", _data, _name, _type, _line, _startposition, _priority);
}
