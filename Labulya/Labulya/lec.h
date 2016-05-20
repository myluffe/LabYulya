#pragma once
#include "string.h"
#include "stdafx.h"
#include "list.h"

class lexeme
{
private:
	char _name[50];
	char _type[20];
	char _data[256];
	int _line;
	int _startposition;
	int _priority;
public: 
	lexeme(char* name, char* type, char* data, int line, int startposition, int priority);
	void print();
	char* Name() { return _name; }
	char* Type() { return _type; }
	char* Data() { return _data; }
	int Line() { return _line; }
	int Start_Position() { return _startposition; }
	int Priority() { return _priority; }
};

