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
	int GetLexemePositionWithMinimalPriority(List* expression);

	lexeme* NExpressionSolver(List* expression);

	bool IsNumberExpression(List* expression);
	bool IsBoolExpression(List* expression);
	bool IsStringExpression(List * expression);

	bool _error = false;

private:
	bool WhateverCheck(char ** perone, int c1, char ** types, int c2, List * expression);
	lexeme* Exe3Pression(List* expression);
	//lexeme* Exe2Pression(lexeme* temp1, lexeme* temp2, bool after);
};

static LexemeWorker LWorker = *new LexemeWorker();

