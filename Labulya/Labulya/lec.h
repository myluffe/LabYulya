#pragma once

#include "string.h"

class lexeme
{
private:
	char* _name;
	char* _data;
	int _type;
	int _line;
	int _startposition;
	int _priority;
public: 
	lexeme(char* name, char* type, char* data, int line, int startposition, int priority);
	lexeme(char* name, int type, char* data, int line, int startposition, int priority);
	~lexeme();

	char* Data() { return _data; }
	char* Name() { return _name; }
	int Type() { return _type; }
	int Line() { return _line; }
	int Start_Position() { return _startposition; }
	int Priority() { return _priority; }

	bool GetNumber(double* value);
	bool SetNumber(double value);

	void DataChange(char* newdata);
	void Print();
};