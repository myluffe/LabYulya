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
	if (PreprocessingElemMas(lexes, start, finish))
	{
		return GetTNode1(lexes, start, finish);
	}
	ErrorReporter().FReport(logfile, "Не удалось обработать элементы массива!", (*(lexeme**)lexes->get(start))->Line(), (*(lexeme**)lexes->get(start))->Start_Position());
	return nullptr;
}

TNode* TreeWorker::GetTNode1(List* lexes, int start, int finish)
{
	if (PreprocessingBrackets(lexes, start, finish))
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
		int pos_max = 0;
		lexeme* temp = nullptr;
		for (int y = 0; y < nodes->count(); y++)
		{
			DoTNode* current = *(DoTNode**)nodes->get(y);
			if (start == current->start && finish == current->finish)
				return current->node;
			if (start == current->start && finish > current->finish)
			{
				if (current->type == DEVIDER)
				{
					temp = *(lexeme**)lexes->get(current->finish + 1);
					pos_max = current->finish + 1;
				}
			}
			if (start < current->start && finish == current->finish)
			{
				if (current->type == DEVIDER)
				{
					temp = *(lexeme**)lexes->get(current->start - 1);
					pos_max = current->start - 1;
				}
			}
		}
		
		if (temp == nullptr)
		{
			pos_max = GetLexemePositionWithMaximalPriority(lexes, start, finish);
			temp = *(lexeme**)lexes->get(pos_max);
		}
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

bool TreeWorker::PreprocessingBrackets(List* lexes, int start, int finish)
{
	int countBrac = 0;
	for (int i = start; i <= finish; i++)
	{
		lexeme* lexeme1 = *(lexeme**)lexes->get(i);
		if (strcmp(lexeme1->Data(), ")") == 0)
		{
			countBrac++;
			int currBrac = 0;
			bool flag = false;
			for (int j = i - 1; j >= start && !flag; j--)
			{
				lexeme* lexeme2 = *(lexeme**)lexes->get(j);
				if (strcmp(lexeme2->Data(), "(") == 0)
				{
					if ((countBrac - currBrac - 1) == 0)
					{
						flag = true;
						DoTNode* a = (DoTNode*)heap.get_mem(sizeof(DoTNode));
						a->start = j;
						a->finish = i;
						a->type = lexeme2->Type();
						a->node = GetTNode2(lexes, j + 1, i - 1);
						if (a->node == nullptr)
						{
							ErrorReporter().FReport(logfile, "Не удалось получить внутренний операнд!", lexeme1->Line(), lexeme1->Start_Position());
							return false;
						}
						nodes->add(&a);
					}
					currBrac++;
				}
			}
			if (!flag)
			{
				ErrorReporter().FReport(logfile, "Не удалось найти \"(\"!", lexeme1->Line(), lexeme1->Start_Position());
				return false;
			}

		}
	}
	return true;
}

bool TreeWorker::PreprocessingElemMas(List* lexes, int start, int finish)
{
	for (int i = start; i <= finish; i++)
	{
		lexeme* lexeme1 = *(lexeme**)lexes->get(i);
		if (lexeme1->Type() == MASSIVE)
		{
			int	step = i;
			TList* Iters = LWorker.GetMassElemIndexes(lexes, lexeme1, &step);
			if (Iters != nullptr)
			{
				DoTNode* a = (DoTNode*)heap.get_mem(sizeof(DoTNode));
				TElementArray* telem = new TElementArray(lexeme1, Iters);
				a->start = i;
				a->finish = step;
				a->type = lexeme1->Type();
				a->node = (TNode*)telem;
				if (a->node == nullptr)
				{
					ErrorReporter().FReport(logfile, "Не удалось получить внутренний операнд!", lexeme1->Line(), lexeme1->Start_Position());
					return false;
				}
				nodes->add(&a);
				i += step;
			}
		}
	}
	return true;
}