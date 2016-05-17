#pragma once
#include "string.h"
#include "stdafx.h"
#include "list.h"

static class lexeme
{
private:
	struct lex
	{
		char _name[50];
		char _type[20];
		char _data[256];
		int _line;
		int _startposition;
		int _priority;
	public: lex(char* name, char* type, char* data, int line, int startposition, int priority)
		{
			strcpy_s(_name, name);
			strcpy_s(_type, type);
			strcpy_s(_data, data);
			_line = line;
			_startposition = startposition;
			_priority = priority;
		}
	};
	List lexes = List(sizeof(lex));
public:
	void Addlexeme(char* name, char* type, char* data, int line, int startposition);
	~lexeme();
};

