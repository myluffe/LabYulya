#include "stdafx.h"
#include "tree.h"
#include "LexemeWorker.h"
#include "ErrorReporter.h"

static bool DoTree(List* lexes)
{
	int start = 0;
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), ";") == 0)
		{
			TList* prog = (TList*)heap.get_mem(sizeof(TList));
			prog->addNode((TNode*)heap.get_mem(sizeof(TNode));
			DoStr(lexes, start, i, );
			start = i + 1;
		}
	}
	return true;
}

static void DoStr(List* lexes, int start, int finish, TNode parent)
{
	int min_prior = ((lexeme*)lexes->get(start))->Priority;
	int pos_min = start;
	for (int i = start; i < finish; i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (temp_lexeme->Priority < min_prior)
		{
			min_prior = temp_lexeme->Priority;
			pos_min = i;
		}
	}

}