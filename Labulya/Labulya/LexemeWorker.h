#pragma once

#include "list.h"
#include "hash_lec.h"
#include "list_adv.h"
#include "Parser.h"
#include "ErrorReporter.h"
#include "TreeWorker.h"

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

	int CorrectWhile(List* expression, int pos, lexeme* spec, TList* storage);
	int CorrectDo(List* expression, int pos, lexeme* spec, TList* storage);
	int CorrectFor(List* expression, int pos, lexeme* spec, TList* storage);
	int CorrectIf(List* expression, int pos, lexeme* spec, TList* storage);

	int CorrectShortIfOperation(List* expression, int i, TList* storage);
	int CorrectSpecial(lexeme* spec, int pos, List* expression, TList* storage);
	int CorrectExpression(List* expression, int pos, TList* storage);

	bool _error = false;

private:
	int FuncWithNumberParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param);
	int FuncWithTwoNumberParams(List* expression, int pos, lexeme* spec, bool finaldevider, List* param1, List* param2);
	int FuncWithStringParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param);
	int FuncWithBoolParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param);

	bool InnerExpression(List* expression, TList* storage);
	bool WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression, bool equal);
	//lexeme* Exe3Pression(List* expression);
} LWorker = *new LexemeWorker();

static List Todel = *new List(sizeof(List*));
