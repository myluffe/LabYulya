#include "stdafx.h"
#include "TreeWorker.h"

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
	return true;
}

TNode* TreeWorker::GetTNode(List* lexes, int start, int finish)
{
	if (start != finish)
	{
		int min_prior = ((lexeme*)lexes->get(start))->Priority();
		int pos_min = start;
		for (int i = start; i < finish; i++)
		{
			lexeme* temp_lexeme = (lexeme*)lexes->get(i);
			if (temp_lexeme->Priority() < min_prior)
			{
				min_prior = temp_lexeme->Priority();
				pos_min = i;
			}
		}
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
		if (strcmp(((lexeme*)lexes->get(start))->Name(), "Variables") == 0)
			return (TNode*)new TVariable((lexeme*)lexes->get(start));
		if (strcmp(((lexeme*)lexes->get(start))->Name(), "Numbers") == 0)
			return (TNode*)new TConst((lexeme*)lexes->get(start));
		return NULL;
	}
}