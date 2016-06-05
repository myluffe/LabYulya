#include "stdafx.h"
#include "LexemeWorker.h"
#include "ErrorReporter.h"

bool LexemeWorker::Processing(List* lexes)
{
	//изменение
	//LexemeStack* temp_stack = new LexemeStack();
	Lexeme_list* dob = new Lexeme_list(); //стэк для области видимости
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), "{") == 0)
		{
			dob->add(temp_lexeme);
			continue;
		}
		if (temp_lexeme->Type() == VARIABLE)
		{
			if (dob->find(temp_lexeme->Data()) == nullptr) //если нет в дов
			{
				if (i > 0)
				{
					lexeme* ttype = (lexeme*)lexes->get(i - 1);
					if (ttype->Type() != TYPE)
					{
						errorReporter.FReport(logfile, "No type of undefined variable!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						return false;
					}
					if (i + 1 != lexes->count() && i + 2 != lexes->count() && i + 3 != lexes->count())
					{
						lexeme* tdevider = (lexeme*)lexes->get(i + 1);
						if (strcmp(tdevider->Data(), "=") != 0)
						{
							errorReporter.FReport(logfile, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + strlen(temp_lexeme->Data()));
							return false;
						}
						lexeme* tdata = (lexeme*)lexes->get(i + 2);
						if (!(tdata->Type() == INT || tdata->Type() == DOUBLE || tdata->Type() == FLOAT || tdata->Type() == CHAR || tdata->Type() == STRING || tdata->Type() == BOOL))
						{
							if (tdata->Type() == VARIABLE)
							{
								int pos = dob->findpos(tdata->Data());
								if (pos != -1)
									*tdata = *(lexeme*)dob->get(pos);
								else
								{
									errorReporter.FReport(logfile, "Нельзя непоределенное присвоить значение!", tdata->Line(), tdata->Start_Position());
									return false;
								}
							}
							else
							{
								errorReporter.FReport(logfile, "There should be value!", tdevider->Line(), tdevider->Start_Position());
								return false;
							}
						}
						lexeme* tdevider2 = (lexeme*)lexes->get(i + 3);
						if (strcmp(tdevider2->Data(), ";") != 0)
						{
							errorReporter.FReport(logfile, "There should be \";\" after value!", tdata->Line(), tdata->Start_Position());
							return false;
						}
						lexeme* newlex = new lexeme(temp_lexeme->Data(), ttype->Data(), tdata->Data(), temp_lexeme->Line(), temp_lexeme->Start_Position(), temp_lexeme->Priority());
						if (tdata->Type() != GetType(ttype->Data()))
						{
							errorReporter.FReport(logfile, "Diffrent variable and value types!", temp_lexeme->Line(), temp_lexeme->Start_Position());
							return false;
						}
						//в дов уже готовую добавляем
						dob->add(newlex);
						//добавление в хэш-таблицу
						LexemeTable.auto_create(newlex);
						//удаление ненужных и мухлеж с щетчиком :
						int ti = i - 2;
						lexes->remove(i + 3); //удалять ли точку с запятой после переменной?
						lexes->remove(i + 2);
						lexes->remove(i + 1);
						lexes->remove(i); //удалять все?
						lexes->remove(i - 1);
						i = ti;
					}
					else
					{
						errorReporter.FReport(logfile, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + strlen(temp_lexeme->Data()));
						return false;
					}
				}
				else
				{
					errorReporter.FReport(logfile, "You miss the type!", temp_lexeme->Line(), temp_lexeme->Start_Position());
					return false;
				}
			}
			else
			{
				if (i > 0)
				{
					lexeme* ttype = (lexeme*)lexes->get(i - 1);
					if (ttype->Type() == TYPE)
					{
						dob->print_list();
						errorReporter.FReport(logfile, "This variable is alredy defined!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						return false;
					}
					else
					{
						//заменяем на ссылку
						int pos = dob->findpos(temp_lexeme->Data());
						if (pos != -1)
							*temp_lexeme = *(lexeme*)dob->get(pos);
						/*
						List* tlist = LexemeTable.find_list(temp_lexeme->Data());
						for (int k = 0; k < tlist->count(); k++)
						{
							if (strcmp(temp_lexeme->Data(), ((lexeme*)tlist->get(k))->Name()) == 0)
							{
								*temp_lexeme = *(lexeme*)tlist->get(k);
								break;
							}
						}
						*/
					}
				}
				else
				{
					//
					errorReporter.FReport(logfile, "No type of undefined variable!", temp_lexeme->Line(), temp_lexeme->Start_Position());
					return false;
				}
			}
		}
		if (temp_lexeme->Type() == NUMBER)
		{
			//обработка для чисел
		}
		if (strcmp(temp_lexeme->Data(), "}") == 0)
		{
			for (int j = dob->count() - 1; j >= 0; j--)
			{
				lexeme* tlex = (lexeme*)dob->get(j);
				if (strcmp(tlex->Data(), "{") != 0)
				{
					dob->remove(j);
				}
				else
				{
					dob->remove(j);
					break;
				}
			}
		}
	}
	dob->~Lexeme_list();

	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (temp_lexeme->Type() == SPECIALWORD)
		{
			i = CorrectSpecial(temp_lexeme, i, lexes);
			if (_error) return false;
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

static int GetLexemePositionWithMinimalPriority(List * expression)
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

int LexemeWorker::CorrectSpecial(lexeme* spec, int pos, List* expression)
{
	if (pos == expression->count())
	{
		errorReporter.FReport(logfile, "Не законченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		return 0;
	}
	if (strcmp(spec->Data(), "for ") == 0)
	{
		return CorrectFor(expression, pos, spec);
	}
	if (strcmp(spec->Data(), "if ") == 0)
	{
		return CorrectIf(expression, pos, spec);
	}
	if (strcmp(spec->Data(), "while ") == 0)
	{
		return CorrectWhile(expression, pos, spec);
	}
	if (strcmp(spec->Data(), "do ") == 0)
	{
		return CorrectDo(expression, pos, spec);
	}
	if (strcmp(spec->Data(), "input ") == 0)
	{
		int res = 0;
		res = FuncWithStringParam(expression, pos, spec, true);
		if (res > 0) return res;
		res = FuncWithNumberParam(expression, pos, spec, true);
		if (res > 0) return res;
		res = FuncWithBoolParam(expression, pos, spec, true);
		if (res > 0) return res;
		return 0;
	}
	if (strcmp(spec->Data(), "output ") == 0)
	{
		int res = 0;
		res = FuncWithStringParam(expression, pos, spec, true);
		if (res > 0) return res;
		res = FuncWithNumberParam(expression, pos, spec, true);
		if (res > 0) return res;
		res = FuncWithBoolParam(expression, pos, spec, true);
		if (res > 0) return res;
		return 0;
	}
	if (strcmp(spec->Data(), "min ") == 0)
	{
		return FuncWithTwoNumberParams(expression, pos, spec, true);
	}
	if (strcmp(spec->Data(), "max ") == 0)
	{
		return FuncWithTwoNumberParams(expression, pos, spec, true);
	}
	if (strcmp(spec->Data(), "sin ") == 0)
	{
		return FuncWithNumberParam(expression, pos, spec, true);
	}
	if (strcmp(spec->Data(), "cos ") == 0)
	{
		return FuncWithNumberParam(expression, pos, spec, true);
	}
	return 0;
}

int LexemeWorker::FuncWithTwoNumberParams(List * expression, int pos, lexeme * spec, bool finaldevider)
{
	lexeme* devider1 = (lexeme*)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return 0;
	}
	List* ltemp1 = new List(sizeof(lexeme));
	lexeme* tlex = nullptr;
	lexeme* devider2 = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = (lexeme*)expression->get(i);
		if (strcmp(tlex->Data(), ",") == 0)
		{
			devider2 = (lexeme*)expression->get(i);
			break;
		}
		ltemp1->add(tlex);
	}
	if (!IsNumberExpression(ltemp1, false))
	{
		errorReporter.FReport(logfile, "Ожидается числовое выражение!", devider1->Line(), devider1->Start_Position());
		ltemp1->~List();
		return 0;
	}
	if (strcmp(tlex->Data(), ",") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \",\"", devider2->Line(), devider2->Start_Position());
		ltemp1->~List();
		return 0;
	}
	List* ltemp2 = new List(sizeof(lexeme));
	lexeme* devider3 = nullptr;
	int j = 0;
	for (j = i + 1; j < expression->count(); j++)
	{
		tlex = (lexeme*)expression->get(j);
		if (strcmp(tlex->Data(), ")") == 0)
		{
			devider3 = (lexeme*)expression->get(i);
			break;
		}
		ltemp2->add(tlex);
	}
	if (!IsNumberExpression(ltemp2, false))
	{
		errorReporter.FReport(logfile, "Ожидается числовое выражение!", devider1->Line(), devider1->Start_Position());
		ltemp1->~List();
		ltemp2->~List();
		return 0;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp1->~List();
		ltemp2->~List();
		return 0;
	}
	if (!finaldevider)
	{
		ltemp1->~List();
		ltemp2->~List();
		return j;
	}
	tlex = (lexeme*)expression->get(j + 1);
	if ((j + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp1->~List();
		ltemp2->~List();
		return 0;
	}

	ltemp1->~List();
	ltemp2->~List();
	return j + 1;
}

int LexemeWorker::FuncWithStringParam(List * expression, int pos, lexeme * spec, bool finaldevider)
{
	lexeme* devider1 = (lexeme*)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return 0;
	}
	List* ltemp = new List(sizeof(lexeme));
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = (lexeme*)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		ltemp->add(tlex);
	}
	if (!IsStringExpression(ltemp, false))
	{
		errorReporter.FReport(logfile, "Ожидается строковое выражение!", devider1->Line(), devider1->Start_Position());
		ltemp->~List();
		return 0;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp->~List();
		return 0;
	}
	if (!finaldevider)
	{
		ltemp->~List();
		return i;
	}
	tlex = (lexeme*)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp->~List();
		return 0;
	}

	ltemp->~List();
	return i + 1;
}

int LexemeWorker::FuncWithBoolParam(List * expression, int pos, lexeme * spec, bool finaldevider)
{
	lexeme* devider1 = (lexeme*)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return 0;
	}
	List* ltemp = new List(sizeof(lexeme));
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = (lexeme*)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		ltemp->add(tlex);
	}
	if (!IsBoolExpression(ltemp, false) && !IsNumberExpressionWithBoolOperations(ltemp) && !IsStringExpressionWithBoolOperations(ltemp))
	{
		errorReporter.FReport(logfile, "Ожидается выражение типа bool!", devider1->Line(), devider1->Start_Position());
		ltemp->~List();
		return 0;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp->~List();
		return 0;
	}
	if (!finaldevider)
	{
		ltemp->~List();
		return i;
	}
	tlex = (lexeme*)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp->~List();
		return 0;
	}

	ltemp->~List();
	return i + 1;
}

int LexemeWorker::FuncWithNumberParam(List * expression, int pos, lexeme * spec, bool finaldevider)
{
	lexeme* devider1 = (lexeme*)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return 0;
	}
	List* ltemp = new List(sizeof(lexeme));
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = (lexeme*)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		ltemp->add(tlex);
	}
	if (!IsNumberExpression(ltemp, false))
	{
		errorReporter.FReport(logfile, "Ожидается числовое выражение!", devider1->Line(), devider1->Start_Position());
		ltemp->~List();
		return 0;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp->~List();
		return 0;
	}
	if (!finaldevider)
	{
		ltemp->~List();
		return i;
	}
	tlex = (lexeme*)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		ltemp->~List();
		return 0;
	}
	
	ltemp->~List();
	return i + 1;
}

bool LexemeWorker::InnerExpression(List * expression)
{
	lexeme* tlex;
	bool startstring = false;
	List* tlist = new List(sizeof(lexeme));
	for (int i = 0; i < expression->count(); i++)
	{
		tlex = (lexeme*)expression->get(i);
		if (!startstring && tlex->Type() == SPECIALWORD)
		{
			i = CorrectSpecial(tlex, i, expression);
			startstring = true;
		}
		else
		{
			if (strcmp(tlex->Data(), ";") != 0)
				tlist->add(tlex);
			else
			{
				if (!(IsNumberExpression(tlist, true) || IsBoolExpression(tlist, true) || IsStringExpression(tlist, true)))
				{
					tlist->~List();
					return false;
				}
				tlist->~List();
				tlist = new List(sizeof(lexeme));
				startstring = false;
			}
		}
	}
	return true;
}

bool LexemeWorker::WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression, bool qual)
{
	bool varbefore = false;
	bool correct = false;
	int hook_count = 0;
	int equalitycount = 0;

	for (int k = 0; k < expression->count(); k++)
	{
		correct = false;
		lexeme* tlex = (lexeme*)expression->get(k);
		if (tlex->Type() == DEVIDER)
		{
			if (strcmp(tlex->Data(), ")") == 0)
			{
				hook_count--;
				if (hook_count < 0)
				{
					errorReporter.FReport(logfile, "Вы забыли открыть скобку!", tlex->Line(), tlex->Start_Position());
					_error = true;
					return false;
				}
			}
			if (strcmp(tlex->Data(), "(") == 0)
				hook_count++;
			varbefore = false;
			correct = true;
		}
		else if (tlex->Type() == BYNARYOPERATION || tlex->Type() == UNARYOPERATION || tlex->Type() == TERNARYOPERATION)
		{
			for (int s = 0; s < c1; s++)
			{
				if (strcmp(perone[s], tlex->Data()) == 0)
				{
					correct = true;
					break;
				}
			}
			varbefore = false;
			if (!correct)
				if (strcmp(tlex->Data(), "=") == 0)
				{
					if (equalitycount > 1)
					{
						errorReporter.FReport(logfile, "В выражении не может быть несколько присваиваний!", tlex->Line(), tlex->Start_Position());
						return false;
					}
					equalitycount++;
					correct = true;
				}
		}
		else
		{
			if (varbefore)
			{
				errorReporter.FReport(logfile, "Ожидается операция!", tlex->Line(), tlex->Start_Position());
				//_error == true;
				return false;
			}
			for (int f = 0; f < c2; f++)
			{
				if (tlex->Type() == types[f])
				{
					correct = true;
					varbefore = true;
					break;
				}
			}
		}
		if (!correct)
			return false;
	}
	if (hook_count > 0)
	{
		lexeme* tlex = (lexeme*)expression->get(expression->count() - 1);
		char str[50];
		str[0] = '\0';
		sprintf_s(str, "Вы забыли закрыть %d скобку(ок)!", hook_count);
		errorReporter.FReport(logfile, str, tlex->Line(), tlex->Start_Position());
		_error = true;
		return false;
	}
	return correct;
}

bool LexemeWorker::IsNumberExpression(List * expression, bool equal)
{
	//char* perone[] = { "++", "--", "+", "-", "(", "/", "*", ")", "%", "==", ">=", "<=", "!=", ">", "<" };
	char* perone[] = { "++", "--", "<<", ">>", "+", "-", "/", "*", "%", "==", ">=", "<=", "!=", ">", "<" };
	int types[] = { INT, FLOAT, DOUBLE };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression, equal);
}

bool LexemeWorker::IsNumberExpressionWithBoolOperations(List * expression)
{
	//char* perone[] = { "++", "--", "+", "-", "(", "/", "*", ")", "%", "==", ">=", "<=", "!=", ">", "<" };
	char* perone[] = { "==", ">=", "<=", "!=", ">", "<" };
	int types[] = { INT, FLOAT, DOUBLE };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression, false);
}

bool LexemeWorker::IsStringExpressionWithBoolOperations(List * expression)
{
	char* perone[] = { "==", "!=" };
	int types[] = { STRING, CHAR };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression, false);
}

bool LexemeWorker::IsBoolExpression(List * expression, bool equal)
{
	char* perone[] = { "==", "||", "&&", "!=" };
	int types[] = { BOOL };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression, equal);
}

bool LexemeWorker::IsStringExpression(List * expression, bool equal)
{
	char* perone[] = { "==", "+", "!="};
	int types[] = { STRING, CHAR };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression, equal);
}

int LexemeWorker::CorrectWhile(List * expression, int pos, lexeme * spec)
{
	//comment
	int pos2 = FuncWithBoolParam(expression, pos, spec, false);
	if (pos2 == 0)
	{
		_error = true;
		return 0;
	}
	pos2++;
	
	List* tlist = new List(sizeof(lexeme));
	if (pos2 < expression->count())
	{
		lexeme* tlex = (lexeme*)expression->get(pos2);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos2; pos2 < expression->count(); pos2++)
			{
				tlex = (lexeme*)expression->get(pos2);
				if (strcmp(tlex->Data(), ";") != 0)
					tlist->add(tlex);
				else
				{
					tlist->add(tlex);
					break;
				}
			}
		}
		else
		{
			pos2++;
			for (pos2; pos2 < expression->count(); pos2++)
			{
				tlex = (lexeme*)expression->get(pos2);
				if (strcmp(tlex->Data(), "}") != 0)
					tlist->add(tlex);
				else
				{
					break;
				}
			}
		}
		if (InnerExpression(tlist))
		{
			tlist->~List();
			return pos2;
		}
		else
		{
			_error = true;
			tlist->~List();
			return 0;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Незаконченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist->~List();
		return 0;
	}

	tlist->~List();
	return 0;
}

int LexemeWorker::CorrectDo(List * expression, int pos, lexeme * spec)
{
	List* tlist = new List(sizeof(lexeme));
	if (pos < expression->count())
	{
		lexeme* tlex = (lexeme*)expression->get(pos);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos; pos < expression->count(); pos++)
			{
				tlex = (lexeme*)expression->get(pos);
				if (strcmp(tlex->Data(), ";") != 0)
					tlist->add(tlex);
				else
				{
					tlist->add(tlex);
					break;
				}
			}
		}
		else
		{
			pos++;
			for (pos; pos < expression->count(); pos++)
			{
				tlex = (lexeme*)expression->get(pos);
				if (strcmp(tlex->Data(), "}") != 0)
					tlist->add(tlex);
				else
				{
					break;
				}
			}
		}
		if (InnerExpression(tlist))
		{
			tlist->~List();
			{
				pos++;
				if (pos <= expression->count())
				{
					tlex = (lexeme*)expression->get(pos);
					if (strcmp(tlex->Data(), "while ") != 0)
					{
						errorReporter.FReport(logfile, "Ожидается \"while \" после тела цикла \"do \"!", tlex->Line(), tlex->Start_Position());
						_error = true;
						tlist->~List();
						return 0;
					}
					int pos2 = FuncWithBoolParam(expression, pos, spec, true);
					if (pos2 == 0)
					{
						_error = true;
						return 0;
					}
					return pos2;
				}
				else
				{
					_error = true;
					tlist->~List();
					return 0;
				}
			}
		}
		else
		{
			_error = true;
			tlist->~List();
			return 0;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Ожидается тело цикла \"do \"!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist->~List();
		return 0;
	}
	return 0;
}

int LexemeWorker::CorrectFor(List * expression, int pos, lexeme * spec)
{
	pos++;
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Ожидаются параметры для \"for \"!", spec->Line(), spec->Start_Position());
		_error = true;
		return 0;
	}
	lexeme* tlex = (lexeme*)expression->get(pos);
	if (strcmp(tlex->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		return 0;
	}
	pos++;
	List* param1 = new List(sizeof(lexeme));
	for (pos; pos < expression->count(); pos++)
	{
		tlex = (lexeme*)expression->get(pos);
		if (strcmp(tlex->Data(), ";") == 0)
			break;
		param1->add(tlex);
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Незаконченное выражение \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		return 0;
	}
	if (param1->count() != 1)
	{
		errorReporter.FReport(logfile, "Ожидается инициализированная переменная в качестве первого параметра!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		return 0;
	}
	lexeme* first = (lexeme*)param1->get(0);
	if (!(first->Type() == INT || first->Type() == DOUBLE || first->Type() == FLOAT))
	{
		errorReporter.WarningReport(logfile, "Возможно неверный тип первого пареметра \"for \"", tlex->Line(), tlex->Start_Position());
	}

	List* param2 = new List(sizeof(lexeme));
	for (pos; pos < expression->count(); pos++)
	{
		tlex = (lexeme*)expression->get(pos);
		if (strcmp(tlex->Data(), ";") == 0)
			break;
		param2->add(tlex);
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Незаконченное выражение \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		return 0;
	}
	if (!IsNumberExpressionWithBoolOperations(param2) && !IsBoolExpression(param2, false) && !IsNumberExpressionWithBoolOperations(param2))
	{
		errorReporter.FReport(logfile, "Неверный второй параметр \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param2->~List();
		param1->~List();
		return 0;
	}

	List* param3 = new List(sizeof(lexeme));
	for (pos; pos < expression->count(); pos++)
	{
		tlex = (lexeme*)expression->get(pos);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		param3->add(tlex);
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		param3->~List();
		return 0;
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Незаконченное выражение \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		param3->~List();
		return 0;
	}
	if (!IsNumberExpression(param3, true))
	{
		errorReporter.FReport(logfile, "Неверный третий параметр \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param3->~List();
		param2->~List();
		param1->~List();
		return 0;
	}

	List* body = new List(sizeof(lexeme));
	if (pos < expression->count())
	{
		tlex = (lexeme*)expression->get(pos);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos; pos < expression->count(); pos++)
			{
				tlex = (lexeme*)expression->get(pos);
				if (strcmp(tlex->Data(), ";") != 0)
					body->add(tlex);
				else
				{
					body->add(tlex);
					break;
				}
			}
		}
		else
		{
			pos++;
			for (pos; pos < expression->count(); pos++)
			{
				tlex = (lexeme*)expression->get(pos);
				if (strcmp(tlex->Data(), "}") != 0)
					body->add(tlex);
				else
				{
					break;
				}
			}
		}
		if (InnerExpression(body))
		{
			body->~List();
			param3->~List();
			param2->~List();
			param1->~List();
			return pos;
		}
		else
		{
			_error = true;
			body->~List();
			param3->~List();
			param2->~List();
			param1->~List();
			return 0;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Ожидается тело \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		body->~List();
		param3->~List();
		param2->~List();
		param1->~List();
		return 0;
	}

	body->~List();
	param2->~List();
	param2->~List();
	param1->~List();
	return 0;
}

int LexemeWorker::CorrectIf(List * expression, int pos, lexeme * spec)
{
	pos = CorrectWhile(expression, pos, spec);
	pos++;
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Ожедается условие \"if \"!", spec->Line(), spec->Start_Position());
		_error = true;
		return 0;
	}
	lexeme* tlex = (lexeme*)expression->get(pos);
	if (strcmp(tlex->Data(), "else ") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"else \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		return 0;
	}
	pos = CorrectWhile(expression, pos, spec);
	if (pos >= expression->count())
	{
		_error = true;
		return 0;
	}
	return pos;
}
