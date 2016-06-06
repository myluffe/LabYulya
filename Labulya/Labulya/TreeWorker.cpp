#include "stdafx.h"
#include "TreeWorker.h"

int TreeWorker::GetLexemePositionWithMinimalPriority(List * lexes, int start, int finish)
{
	int min_prior = ((lexeme*)lexes->get(start))->Priority();
	int pos_min = start;
	for (int i = start; i <= finish; i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (temp_lexeme->Priority() < min_prior)
		{
			min_prior = temp_lexeme->Priority();
			pos_min = i;
		}
	}
	return pos_min;
}

bool TreeWorker::DoTree(List* lexes)
{
	int start = 0;
	TList* prog = (TList*)heap.get_mem(sizeof(TList));
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), ";") == 0)
		{
			prog->addNode(GetTNode(lexes, start, i));
			start = i + 1;
		}
	}
	prog->print();
	heap.free_mem(prog);
	return true;
}

TNode* TreeWorker::GetTNode(List* lexes, int start, int finish)
{
	if (start != finish)
	{
		int pos_min = GetLexemePositionWithMinimalPriority(lexes, start, finish);
		if (((lexeme*)lexes->get(pos_min))->Type() == BYNARYOPERATION)
			return (TNode*)new TBinaryOperation(GetTNode(lexes, start, pos_min - 1), GetTNode(lexes, pos_min + 1, finish), (lexeme*)lexes->get(pos_min));
		if (((lexeme*)lexes->get(pos_min))->Type() == UNARYOPERATION)
		{
			if (pos_min == finish)
				return (TNode*)new TUnaryOperation(GetTNode(lexes, start, pos_min - 1), (lexeme*)lexes->get(pos_min), false);
			if (pos_min == start)
				return (TNode*)new TUnaryOperation(GetTNode(lexes, pos_min + 1, finish), (lexeme*)lexes->get(pos_min), true);
		}
		return NULL;
	}
	else
	{
		lexeme* temp = (lexeme*)lexes->get(start);
		if (strcmp(temp->Name(), "Number") == 0)
			return (TNode*)new TConst(temp);
		if (temp->Type() == INT || temp->Type() == DOUBLE || temp->Type() == FLOAT || temp->Type() == BOOL || temp->Type() == STRING || temp->Type() == CHAR)
			return (TNode*)new TVariable(temp);
		return NULL;
	}
}