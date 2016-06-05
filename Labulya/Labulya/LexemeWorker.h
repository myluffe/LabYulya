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
	//lexeme* NExpressionSolver(List* expression);

	bool IsNumberExpression(List* expression, bool equal);
	bool IsBoolExpression(List* expression, bool equal);
	bool IsStringExpression(List * expression, bool equal);

	bool IsNumberExpressionWithBoolOperations(List * expression);
	bool IsStringExpressionWithBoolOperations(List * expression);

	int CorrectWhile(List* expression, int pos, lexeme* spec);
	int CorrectDo(List* expression, int pos, lexeme* spec);
	int CorrectFor(List* expression, int pos, lexeme* spec);
	int CorrectIf(List* expression, int pos, lexeme* spec);

	int CorrectSpecial(lexeme* spec, int pos, List* expression);

	bool _error = false;

private:
	int FuncWithNumberParam(List* expression, int pos, lexeme* spec, bool finaldevider);
	int FuncWithTwoNumberParams(List* expression, int pos, lexeme* spec, bool finaldevider);
	int FuncWithStringParam(List* expression, int pos, lexeme* spec, bool finaldevider);
	int FuncWithBoolParam(List* expression, int pos, lexeme* spec, bool finaldevider);

	bool InnerExpression(List* expression);
	bool WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression, bool equal);
	//lexeme* Exe3Pression(List* expression);
};

static LexemeWorker LWorker = *new LexemeWorker();

