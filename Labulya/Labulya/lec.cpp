#include "stdafx.h"
#include "lec.h"

void lexeme::Addlexeme(char* name, char* type, char* data, int line, int startposition)
{
	lex l = lex(name, type, data, line, startposition);
	lexes.add(&l);
}

lexeme::~lexeme()
{
	lexes.~List();
}
