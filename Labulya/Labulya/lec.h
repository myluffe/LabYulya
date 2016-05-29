#pragma once
#include "stdafx.h"
#include "string.h"

struct lexeme
{
private:
	char* _name;
	char* _type;
	int _line;
	int _startposition;
	int _priority;
public: 
	lexeme(char* name, char* type, char* data, int line, int startposition, int priority);
	~lexeme();
	void Print();
	char* _data;
	char* Name() { return _name; }
	char* Type() { return _type; }
	int Line() { return _line; }
	int Start_Position() { return _startposition; }
	int Priority() { return _priority; }
};

