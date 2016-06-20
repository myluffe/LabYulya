#include "stdafx.h"
#include "TreeWorker.h"

TreeWorker::TreeWorker()
{
	nodes = new List(sizeof(DoTNode*));
}
TreeWorker::~TreeWorker()
{
	if(nodes != nullptr)
		nodes->~List();
}

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
	if (Preprocessing(lexes, start, finish))
	{
		return GetTNode2(lexes, start, finish);
	}
	ErrorReporter().FReport(logfile, "Не удалось обработать скобки!", (*(lexeme**)lexes->get(start))->Line(), (*(lexeme**)lexes->get(start))->Start_Position());
	return nullptr;
}

TNode* TreeWorker::GetTNode2(List* lexes, int start, int finish)
{
	if (start != finish)
	{
		lexeme* temp = nullptr;
		for (int y = 0; y < nodes->count(); y++)
		{
			if (start == (*(DoTNode**)nodes->get(y))->start && finish == (*(DoTNode**)nodes->get(y))->finish)
				return (*(DoTNode**)nodes->get(y))->node;
			if (start == (*(DoTNode**)nodes->get(y))->start && finish > (*(DoTNode**)nodes->get(y))->finish)
			{
				temp = *(lexeme**)lexes->get((*(DoTNode**)nodes->get(y))->finish + 1);
			}
			if (start < (*(DoTNode**)nodes->get(y))->start && finish == (*(DoTNode**)nodes->get(y))->finish)
			{
				temp = *(lexeme**)lexes->get((*(DoTNode**)nodes->get(y))->start - 1);
			}
		}
		int pos_max = GetLexemePositionWithMaximalPriority(lexes, start, finish);
		if(temp == nullptr)
			temp = *(lexeme**)lexes->get(pos_max);
		if (temp->Type() == BYNARYOPERATION)
			if (pos_max > start && pos_max < finish)
			{
				TNode* oper1 = GetTNode2(lexes, start, pos_max - 1);
				if (oper1 == nullptr)
				{
					ErrorReporter().FReport(logfile, "Не удалось получить первый операнд!", temp->Line(), temp->Start_Position());
					return nullptr;
				}
				TNode* oper2 = GetTNode2(lexes, pos_max + 1, finish);
				if (oper2 == nullptr)
				{
					ErrorReporter().FReport(logfile, "Не удалось получить второй операнд!", temp->Line(), temp->Start_Position());
					return nullptr;
				}
				return (TNode*)new TBinaryOperation(oper1, oper2, temp);
			}
		if (temp->Type() == UNARYOPERATION)
		{
			if (pos_max == finish)
			{
				TNode* oper1 = GetTNode2(lexes, start, pos_max - 1);
				if (oper1 == nullptr)
				{
					ErrorReporter().FReport(logfile, "Не удалось получить предоперанд!", temp->Line(), temp->Start_Position());
					return nullptr;
				}
				return (TNode*)new TUnaryOperation(oper1, temp, false);
			}
			if (pos_max == start)
			{
				TNode* oper2 = GetTNode2(lexes, pos_max + 1, finish);
				if (oper2 == nullptr)
				{
					ErrorReporter().FReport(logfile, "Не удалось получить постоперанд!", temp->Line(), temp->Start_Position());
					return nullptr;
				}
				return (TNode*)new TUnaryOperation(oper2, temp, true);
			}
		}
		if (strcmp(temp->Data(), "(") == 0)
		{
			bool flag = true;
			int i = pos_max;
			while (i < finish && flag)
			{
				lexeme* temp_lexeme = (lexeme*)lexes->get(i);
				flag = !(strcmp(temp_lexeme->Data(), ")") == 0);
				i++;
			}
			if (!flag)
			{
				TNode* include = GetTNode2(lexes, pos_max + 1, i - 1);
				if (include == nullptr)
				{
					ErrorReporter().FReport(logfile, "Не удалось получить операндов!", temp->Line(), temp->Start_Position());
					return nullptr;
				}
			}
			else
			{
				ErrorReporter().FReport(logfile, "Не найдена закрывающая скобка!", temp->Line(), temp->Start_Position());
				return nullptr;
			}

		}
		ErrorReporter().FReport(logfile, "Не удалось получить операнды!", temp->Line(), temp->Start_Position());
		return nullptr;
	}
	else
	{
		lexeme* temp = *(lexeme**)lexes->get(start);
		if (strcmp(temp->Name(), "Number") == 0 || strcmp(temp->Name(), "Bool") == 0)
			return (TNode*)new TConst(temp);
		if (temp->Type() == INT || temp->Type() == DOUBLE || temp->Type() == FLOAT || temp->Type() == BOOL || temp->Type() == STRING || temp->Type() == CHAR)
			return (TNode*)new TVariable(temp);
		ErrorReporter().FReport(logfile, "Ожидается переменная или константа!", temp->Line(), temp->Start_Position());
		return nullptr;
	}
	return nullptr;
}

bool TreeWorker::Preprocessing(List* lexes, int start, int finish)
{
	for (int i = start; i <= finish; i++)
	{
		lexeme* lexeme1 = *(lexeme**)lexes->get(i);
		if (strcmp(lexeme1->Data(), "(") == 0)
		{
			bool flag = false;
			for (int j = i + 1; i <= finish && !flag; i++)
			{
				lexeme* lexeme2 = *(lexeme**)lexes->get(j);
				if (strcmp(lexeme2->Data(), ")") == 0)
				{
					flag = true;
					DoTNode a;
					a.finish = j;
					a.start = i;
					a.node = GetTNode(lexes, i + 1, j + 1);
					if (a.node == nullptr)
					{
						ErrorReporter().FReport(logfile, "Не удалось получить внутренний операнд!", lexeme1->Line(), lexeme1->Start_Position());
						return false;
					}
					nodes->add(&a);
				}
			}
			if (!flag)
			{
				ErrorReporter().FReport(logfile, "Не удалось найти \")\"!", lexeme1->Line(), lexeme1->Start_Position());
				return false;
			}

		}
	}
	return true;
}