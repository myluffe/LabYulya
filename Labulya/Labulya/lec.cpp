#include "stdafx.h"
#include "lec.h"

void lexeme::Addlexeme(char* name, char* type, char* data, int line, int startposition, int priority)
{
	lex* l = new lex(name, type, data, line, startposition, priority);
	lexes->add(l);
}

lexeme::~lexeme()
{
	lexes->~List();
}
