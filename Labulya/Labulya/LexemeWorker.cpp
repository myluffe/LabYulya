#include "stdafx.h"
#include "LexemeWorker.h"
#include "ErrorReporter.h"

bool LexemeWorker::Processing(List* lexes)
{
	//LexemeStack* temp_stack = new LexemeStack();
	Lexeme_list* dob = new Lexeme_list(); //стэк для области видимости
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), "{") == 0)
		{
			//temp_stack->push(temp_lexeme);
			dob->add(temp_lexeme);
			continue;
		}
		if (strcmp(temp_lexeme->Type(), "Variable") == 0)
		{
			if (dob->find(temp_lexeme->Data()) == nullptr) //если нет в дов
			{
				if (i > 0)
				{
					lexeme* ttype = (lexeme*)lexes->get(i - 1);
					if (strcmp(ttype->Type(), "Type") != 0)
					{
						ErrorReporter().FReport(stdout, "No type of undefined variable!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						return false;
					}
					if (i + 1 != lexes->count() && i + 2 != lexes->count() && i + 3 != lexes->count())
					{
						lexeme* tdevider = (lexeme*)lexes->get(i + 1);
						if (strcmp(tdevider->Data(), "=") != 0)
						{
							ErrorReporter().FReport(stdout, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + strlen(temp_lexeme->Data()));
							return false;
						}
						lexeme* tdata = (lexeme*)lexes->get(i + 2);
						if (strcmp(tdata->Name(), "Number") != 0 && strcmp(tdata->Name(), "String") != 0 && strcmp(tdata->Name(), "Char") != 0 && strcmp(tdata->Name(), "Bool") != 0)
						{
							ErrorReporter().FReport(stdout, "There should be value!", tdevider->Line(), tdevider->Start_Position());
							return false;
						}
						lexeme* tdevider2 = (lexeme*)lexes->get(i + 3);
						if (strcmp(tdevider2->Data(), ";") != 0)
						{
							ErrorReporter().FReport(stdout, "There should be \";\" after value!", tdata->Line(), tdata->Start_Position());
							return false;
						}
						lexeme* newlex = new lexeme(temp_lexeme->Data(), ttype->Data(), tdata->Data(), temp_lexeme->Line(), temp_lexeme->Start_Position(), temp_lexeme->Priority());
						if (strcmp(tdata->Type(), ttype->Data()) != 0)
							ErrorReporter().FReport(stdout, "Diffrent variable and value types!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						//в дов уже готовую добавляем
						dob->add(newlex);
						//добавление в хэш-таблицу
						LexemeTable.auto_create(newlex);
						//удаление ненужных и мухлеж с щетчиком :
						int ti = i - 1;
						//lexes->remove(i + 3); //удалять ли точку с запятой после переменной?
						lexes->remove(i + 2);
						lexes->remove(i + 1);
						//lexes[i] = newlex;
						lexes->set(i, newlex);
						lexes->remove(i - 1);
						i = ti;
					}
					else
					{
						ErrorReporter().FReport(stdout, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + strlen(temp_lexeme->Data()));
						return false;
					}
				}
				else
				{
					ErrorReporter().FReport(stdout, "You miss the type!", temp_lexeme->Line(), temp_lexeme->Start_Position());
					return false;
				}
			}
			else
			{
				if (i > 0)
				{
					lexeme* ttype = (lexeme*)lexes->get(i - 1);
					if (strcmp(ttype->Type(), "Type") == 0)
					{
						ErrorReporter().FReport(stdout, "This variable is alredy defined!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						return false;
					}
				}
				else
				{
					//заменяем на ссылку
					List* tlist = LexemeTable.find_list(temp_lexeme->Data());
					for (int k = 0; k < tlist->count(); k++)
					{
						if (strcmp(temp_lexeme->Data(), ((lexeme*)tlist->get(k))->Name()) == 0)
						{
							*temp_lexeme = *(lexeme*)tlist->get(k);
							break;
						}
					}
					//отладка
					printf("1 What's wrong! LexemeWorker.cpp");
					//
				}
			}
		}
		if (strcmp(temp_lexeme->Type(), "Number") == 0)
		{
			//обработка для чисел
		}
		if (strcmp(temp_lexeme->Data(), "}") == 0)
		{
			for (int j = 0; j < dob->count(); j++)
			{
				lexeme* tlex = (lexeme*)dob->get(j);
				if (strcmp(tlex->Data(), "{") != 0)
				{
					dob->remove(j);
					j--;
				}
				else continue;
			}
		}
	}
	//отладка
	printf_s("\n|---------------|\nVariable Table:\n");
	LexemeTable.print_lexems();
	printf_s("\n|---------------|\n");
	//
	return true;
}

LexemeWorker::LexemeWorker()
{
}


LexemeWorker::~LexemeWorker()
{
}

int LexemeWorker::GetLexemePositionWithMinimalPriority(List * expression)
{
	int min = 101;
	int pos = -1;
	for (int s = 0; s < expression->count(); s++)
	{
		lexeme* temp = (lexeme*)expression->get(s);
		if (temp->Priority() < min)
		{
			pos = s;
			min = temp->Priority();
		}
	}
	return pos;
}

lexeme * LexemeWorker::NExpressionSolver(List * expression)
{
	if (!IsNumberExpression(expression))
		return nullptr;
	int pos = -1;
	lexeme* solution = nullptr;
	while (expression->count() != 1)
	{
		pos = GetLexemePositionWithMinimalPriority(expression);
		lexeme* temp = (lexeme*)expression->get(pos);
		if (strcmp(temp->Data(), "(") == 0)
		{
			int count = 1;
			List* temp_lexes = new List(sizeof(lexeme));
			temp_lexes->add(temp);
			do
			{
				temp = (lexeme*)expression->get(pos + count);
				temp_lexes->add(temp);
				count++;
				if (pos == expression->count() && strcmp(temp->Data(), ")") != 0)
				{
					//не хватает закрывающей скобки
					//return nullptr;
				}
			} while (strcmp(temp->Data(), ")") != 0);
			expression->set(pos, NExpressionSolver(temp_lexes));
			while (pos != pos + count)
			{
				expression->remove(pos + count);
				count--;
			}
		}
		else
		{
			if (expression->count() > 3)
			{
				if (strcmp(temp->Name(), "Operation") == 0 && pos > 0 && pos < (expression->count() - 1))
				{
					List* temp_list = new List(sizeof(lexeme));
					temp_list->add((lexeme*)expression->get(pos - 1));
					temp_list->add((lexeme*)expression->get(pos));
					temp_list->add((lexeme*)expression->get(pos + 1));
					expression->set(pos - 1, NExpressionSolver(temp_list));
					expression->remove(pos + 1);
					expression->remove(pos);
				}
				else
				{
					if (temp->Priority() != 100)
					{
						//error!? неверное расположение операции
						//return nullptr;
					}
					else
					{
						//error!? не хватает операций между переменными/числами
						//return nullptr;
					}
				}
			}
			else
			{
				//...

				//
			}
		}
	}
	return solution;
}

bool LexemeWorker::WhateverCheck(char ** perone, int c1, char ** types, int c2, List * expression)
{
	char ttype[30];
	bool type = false;
	for (int k = 0; k < expression->count(); k++)
	{
		lexeme* tlex = (lexeme*)expression->get(k);
		bool flag = false;
		for (int s = 0; s < c1; s++)
		{
			if (strcmp(perone[s], tlex->Data()) == 0)
			{
				flag = true;
				break;
			}
		}
		bool tf1 = false;
		for (int f = 0; f < c2; f++)
		{
			if (strcmp(tlex->Type(), types[f]) == 0)
				tf1 = true;
		}
		if (!flag && tf1)
		{
			if (type)
				if (strcmp(ttype, tlex->Type()) != 0)
					return false;
			if (!type)
			{
				strcpy_s(ttype, tlex->Type());
				type = true;
			}
			flag = true;
		}
		if (!flag)
			return false;
	}
	return true;
}

bool LexemeWorker::IsNumberExpression(List * expression)
{
	char* perone[] = { "++", "--", "+", "-", "(", "/", "*", ")", "%" };
	char* types[] = { "int ", "float ", "double " };
	return WhateverCheck(perone, 9, types, 3, expression);
}

bool LexemeWorker::IsBoolExpression(List * expression)
{
	char* perone[] = { "==", "||", "&&", ">=", "<=", "<<", ">>", "!=" };
	char* types[] = { "bool " };
	return WhateverCheck(perone, 8, types, 1, expression);
}

bool LexemeWorker::IsStringExpression(List * expression)
{
	char* perone[] = { "==", "+", "!=" };
	char* types[] = { "string " };
	return WhateverCheck(perone, 9, types, 3, expression);
}

bool LexemeWorker::IsCharExpression(List * expression)
{
	char* perone[] = { "==", "!=" };
	char* types[] = { "char " };
	return WhateverCheck(perone, 2, types, 1, expression);
}
