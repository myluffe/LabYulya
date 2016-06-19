#include "stdafx.h"
#include "TreeWorker.h"

int TreeWorker::GetLexemePositionWithMaximalPriority(List * lexes, int start, int finish)
{
	int max_prior = (*(lexeme**)lexes->get(start))->Priority();
	int pos_max = start;
	for (int i = start; i <= finish; i++)
	{
		lexeme* temp_lexeme = *(lexeme**)lexes->get(i);
		if (temp_lexeme->Priority() > max_prior)
		{
			max_prior = temp_lexeme->Priority();
			pos_max = i;
		}
	}
	return pos_max;
}

int TreeWorker::GetLexemePositionWithMinimalPriority(List * lexes, int start, int finish)
{
	int min_prior = (*(lexeme**)lexes->get(start))->Priority();
	int pos_min = start;
	for (int i = start; i <= finish; i++)
	{
		lexeme* temp_lexeme = *(lexeme**)lexes->get(i);
		if (temp_lexeme->Priority() < min_prior)
		{
			min_prior = temp_lexeme->Priority();
			pos_min = i;
		}
	}
	return pos_min;
}

TNode* TreeWorker::GetTNode(List* lexes, int start, int finish)
{
	if (start != finish)
	{
		int pos_max = GetLexemePositionWithMaximalPriority(lexes, start, finish);
		lexeme* temp = *(lexeme**)lexes->get(pos_max);
		if (temp->Type() == BYNARYOPERATION)
			if (pos_max > start && pos_max < finish)
				return (TNode*)new TBinaryOperation(GetTNode(lexes, start, pos_max - 1), GetTNode(lexes, pos_max + 1, finish), temp);
		if (temp->Type() == UNARYOPERATION)
		{
			if (pos_max == finish)
				return (TNode*)new TUnaryOperation(GetTNode(lexes, start, pos_max - 1), temp, false);
			if (pos_max == start)
				return (TNode*)new TUnaryOperation(GetTNode(lexes, pos_max + 1, finish), temp, true);
		}
		if (strcmp(temp->Data(), "(") == 0)
		{

		}
		ErrorReporter().FReport(logfile, "�� ������� �������� ��������!", temp->Line(), temp->Start_Position());
		return NULL;
	}
	else
	{
		lexeme* temp = *(lexeme**)lexes->get(start);
		if (strcmp(temp->Name(), "Number") == 0 || strcmp(temp->Name(), "Bool") == 0)
			return (TNode*)new TConst(temp);
		if (temp->Type() == INT || temp->Type() == DOUBLE || temp->Type() == FLOAT || temp->Type() == BOOL || temp->Type() == STRING || temp->Type() == CHAR)
			return (TNode*)new TVariable(temp);
		ErrorReporter().FReport(logfile, "��������� ���������� ��� ���������!", temp->Line(), temp->Start_Position());
		return NULL;
	}
}