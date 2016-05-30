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
	lexeme* NumberExpressionSolver(List* expression);
	bool IsNumberExpression(List* expression);
	bool IsBoolExpression(List* expression);
};

static LexemeWorker LWorker = *new LexemeWorker();

