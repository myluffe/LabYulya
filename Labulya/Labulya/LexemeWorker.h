#pragma once

#include "list.h"
#include "hash_lec.h"
#include "list_adv.h"

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
	bool IsCharExpression(List * expression);

private:
	bool WhateverCheck(char ** perone, int c1, char ** types, int c2, List * expression);
	lexeme* ExePression(lexeme expression[3]);
};

static LexemeWorker LWorker = *new LexemeWorker();

