#pragma once

#include "list.h"
#include "hash_lec.h"
#include "list_adv.h"
#include "Parser.h"

static class LexemeWorker
{
public:
	bool Processing(List* lexes);
	LexemeWorker();
	~LexemeWorker();
protected:
	lexeme* NExpressionSolver(List* expression);

	bool IsNumberExpression(List* expression);
	bool IsBoolExpression(List* expression);
	bool IsStringExpression(List * expression);

	bool _error = false;

private:
	bool WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression);
	lexeme* Exe3Pression(List* expression);
};

static LexemeWorker LWorker = *new LexemeWorker();

