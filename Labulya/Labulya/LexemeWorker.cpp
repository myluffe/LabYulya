#include "stdafx.h"
#include "LexemeWorker.h"
#include "ErrorReporter.h"

bool LexemeWorker::Processing(List* lexes)
{
	//изменение
	//LexemeStack* temp_stack = new LexemeStack();
	Lexeme_list* dob = new Lexeme_list(); //стэк дл€ области видимости
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), "{") == 0)
		{
			//temp_stack->push(temp_lexeme);
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
						if (!(tdata->Type() == INT || tdata->Type() == DOUBLE || tdata->Type() == FLOAT || tdata->Type() == CHAR || tdata->Type() == STRING || tdata->Type() == BOOL))
						{
							if (tdata->Type() == VARIABLE)
							{
								int pos = dob->findpos(tdata->Data());
								if (pos != -1)
									*tdata = *(lexeme*)dob->get(pos);
								else
								{
									ErrorReporter().FReport(stdout, "Ќельз€ непоределенное присвоить значение!", tdata->Line(), tdata->Start_Position());
									return false;
								}
							}
							else
							{
								ErrorReporter().FReport(stdout, "There should be value!", tdevider->Line(), tdevider->Start_Position());
								return false;
							}
						}
						lexeme* tdevider2 = (lexeme*)lexes->get(i + 3);
						if (strcmp(tdevider2->Data(), ";") != 0)
						{
							ErrorReporter().FReport(stdout, "There should be \";\" after value!", tdata->Line(), tdata->Start_Position());
							return false;
						}
						lexeme* newlex = new lexeme(temp_lexeme->Data(), ttype->Data(), tdata->Data(), temp_lexeme->Line(), temp_lexeme->Start_Position(), temp_lexeme->Priority());
						if (tdata->Type() != GetType(ttype->Data()))
						{
							ErrorReporter().FReport(stdout, "Diffrent variable and value types!", temp_lexeme->Line(), temp_lexeme->Start_Position());
							return false;
						}
						//в дов уже готовую добавл€ем
						dob->add(newlex);
						//добавление в хэш-таблицу
						LexemeTable.auto_create(newlex);
						//удаление ненужных и мухлеж с щетчиком :
						int ti = i - 2;
						lexes->remove(i + 3); //удал€ть ли точку с зап€той после переменной?
						lexes->remove(i + 2);
						lexes->remove(i + 1);
						lexes->remove(i); //удал€ть все?
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
					if (ttype->Type() == TYPE)
					{
						dob->print_list();
						ErrorReporter().FReport(stdout, "This variable is alredy defined!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						return false;
					}
					else
					{
						//замен€ем на ссылку
						List* tlist = LexemeTable.find_list(temp_lexeme->Data());
						for (int k = 0; k < tlist->count(); k++)
						{
							if (strcmp(temp_lexeme->Data(), ((lexeme*)tlist->get(k))->Name()) == 0)
							{
								*temp_lexeme = *(lexeme*)tlist->get(k);
								break;
							}
						}
					}
				}
				else
				{
					//
					ErrorReporter().FReport(stdout, "No type of undefined variable!", temp_lexeme->Line(), temp_lexeme->Start_Position());
					return false;
				}
			}
		}
		if (temp_lexeme->Type() == NUMBER)
		{
			//обработка дл€ чисел
		}
		if (temp_lexeme->Type() == SPECIALWORD)
		{

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

bool LexemeWorker::CorrenctSpecial(lexeme* spec, int pos, List* expression)
{
	if (pos == expression->count())
	{
		ErrorReporter().FReport(stdout, "Ќе законченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		return false;
	}
	if (strcmp(spec->Data(), "for ") == 0)
	{

	}
	if (strcmp(spec->Data(), "if ") == 0)
	{

	}
	if (strcmp(spec->Data(), "else ") == 0)
	{

	}
	if (strcmp(spec->Data(), "while ") == 0)
	{

	}
	if (strcmp(spec->Data(), "do ") == 0)
	{

	}
	if (strcmp(spec->Data(), "input ") == 0)
	{

	}
	if (strcmp(spec->Data(), "output ") == 0)
	{

	}
	if (strcmp(spec->Data(), "min ") == 0)
	{

	}
	if (strcmp(spec->Data(), "max ") == 0)
	{

	}
	if (strcmp(spec->Data(), "sin ") == 0)
	{
		if (!StandartFuncWithNumberExpression)
			return false;
		else return true;
	}
	if (strcmp(spec->Data(), "cos ") == 0)
	{
		if (!StandartFuncWithNumberExpression)
			return false;
		else return true;
	}
	return false;
}

/*
lexeme * LexemeWorker::NExpressionSolver(List * expression)
{
	if (!IsNumberExpression(expression) && !IsBoolExpression(expression) && !IsStringExpression(expression))
		return nullptr;
	int pos = -1;
	lexeme* solution = nullptr;
	while (expression->count() != 1)
	{
		pos = GetLexemePositionWithMinimalPriority(expression);
		lexeme* temp = (lexeme*)expression->get(pos);
		//TODO: Full Hoock checker!
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
				if (pos + count == expression->count() && strcmp(temp->Data(), ")") != 0)
				{
					ErrorReporter().FReport(stdout, "You should close the hoock!", temp->Line(), pos + count);
					return nullptr;
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
						ErrorReporter().FReport(stdout, "Operation on wrrong place!", temp->Line(), pos);
						return nullptr;
					}
					else
					{
						//error!? не хватает операций между переменными/числами
						ErrorReporter().FReport(stdout, "You  miss the Operation!", temp->Line(), pos);
						return nullptr;
					}
				}
			}
			else
			{
				switch (expression->count())
				{
				case 1:
					return (lexeme*)expression->get(0);
				break;
				//case 2:
					//if (strcmp(((lexeme*)expression->get(0))->Type(), "Operation") == 0)
						//return Exe2Pression((lexeme*)expression->get(0), (lexeme*)expression->get(1), false);
					//return Exe2Pression((lexeme*)expression->get(0), (lexeme*)expression->get(1), true);
				//break;
				case 3:
					return Exe3Pression(expression);
					break;
				default:
					return nullptr;
					break;
				}
			}
		}
	}
	return solution;
}
*/

bool LexemeWorker::StandartFuncWithNumberExpression(List * expression, int pos, lexeme * spec)
{
	lexeme* devider1 = (lexeme*)expression->get(pos + 1);
	if (strcmp(devider1->Data, "(") != 0)
	{
		ErrorReporter().FReport(stdout, "ќжидаетс€ \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return false;
	}
	List* ltemp = new List(sizeof(lexeme));
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 1; i < expression->count(); i++)
	{
		tlex = (lexeme*)expression->get(i);
		ltemp->add(tlex);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
	}
	if (!IsNumberExpression(ltemp))
	{
		ErrorReporter().FReport(stdout, "ќжидаетс€ числовое выражение!", devider1->Line(), devider1->Start_Position());
		_error = true;
		return false;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		ErrorReporter().FReport(stdout, "ќжидаетс€ \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		return false;
	}
	tlex = (lexeme*)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		ErrorReporter().FReport(stdout, "ќжидаетс€ \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		return false;
	}
	
	ltemp->~List();
	return true;
}

bool LexemeWorker::WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression)
{
	int ttype;
	bool flag = false;
	int hook_count = 0;

	for (int k = 0; k < expression->count(); k++)
	{
		flag = false;
		lexeme* tlex = (lexeme*)expression->get(k);
		if (tlex->Type == DEVIDER)
		{
			if (strcmp(tlex->Data, ")") == 0)
			{
				hook_count--;
				if (hook_count < 0)
				{
					ErrorReporter().FReport(stdout, "¬ы забыли открыть скобку!", tlex->Line(), tlex->Start_Position());
					_error == true;
					return false;
				}
			}
			if (strcmp(tlex->Data, "(") == 0)
				hook_count++;
			flag = true;
		}
		else if (tlex->Type() == OPERATION)
		{
			for (int s = 0; s < c1; s++)
			{
				if (strcmp(perone[s], tlex->Data()) == 0)
				{
					flag = true;
					break;
				}
			}
		}
		else
		{
			for (int f = 0; f < c2; f++)
			{
				if (tlex->Type() == types[f])
				{
					flag = true;
					break;
				}
			}
		}
		if (!flag)
			return false;
	}
	if (hook_count > 0)
	{
		lexeme* tlex = (lexeme*)expression->get(expression->count - 1);
		char str[50];
		str[0] = '\0';
		sprintf_s(str, "¬ы забыли закрыть %d скобку(ок)!", hook_count);
		ErrorReporter().FReport(stdout, str, tlex->Line(), tlex->Start_Position());
		_error == true;
		return false;
	}
	return flag;
}

/*
lexeme * LexemeWorker::Exe3Pression(List* expression)
{
	if (expression->count() != 3)
		return nullptr;
	lexeme* temp1 = (lexeme*)expression->get(0);
	lexeme* temp2 = (lexeme*)expression->get(1);
	lexeme* temp3 = (lexeme*)expression->get(2);

	if (temp2->Type != OPERATION)
	{
		_error = true;
		return nullptr;
	}

	if (!(temp1->Type() == INT || temp1->Type() == DOUBLE || temp1->Type() == FLOAT || temp1->Type() == BOOL || temp1->Type() == CHAR || temp1->Type() == STRING))
	{
		_error = true;
		return nullptr;
	}

	if (!(temp3->Type() == INT || temp3->Type() == DOUBLE || temp3->Type() == FLOAT || temp3->Type() == BOOL || temp3->Type() == CHAR || temp3->Type() == STRING))
	{
		_error = true;
		return nullptr;
	}

	if (temp1->Type() == INT)
	{
		// "+", "-", "/", "*", "%", "==", ">=", "<=", "!=", ">", "<"
		if (temp1->Type() != temp3->Type())
			if (IsCastable(temp3->Type(), temp1->Type()))
			{
				int t1 = Parser().ToInt(temp1->Data());
				int t2 = Parser().ToInt(temp3->Data());

				if (strcmp(temp2->Data(), "+") == 0)
				{
					if (t1 + t2 > INT_MAX)
					{
						ErrorReporter().FReport(stdout, "Out of value range!", temp2->Line(), temp2->Start_Position());
						_error = true;
						return nullptr;
					}
					int result = t1 + t2;
					return new lexeme("Number", "int ", Parser().IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
				}
				if (strcmp(temp2->Data(), "-") == 0)
				{
					if (t1 - t2 > INT_MIN)
					{
						ErrorReporter().FReport(stdout, "Out of value range!", temp2->Line(), temp2->Start_Position());
						_error = true;
						return nullptr;
					}
					int result = t1 - t2;
					return new lexeme("Number", "int ", Parser().IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
				}
				if (strcmp(temp2->Data(), "/") == 0)
				{
					if (t1 % t2 == 0)
					{
						int result = t1 / t2;
						return new lexeme("Number", "int ", Parser().IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
					}
					else
					{
						double result = t1 / t2;
						return new lexeme("Number", "double ", Parser().DoubleToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
					}
				}
				if (strcmp(temp2->Data(), "*") == 0)
				{
					if (t1 * t2 > INT_MAX)
					{
						double result = t1 * t2;
						return new lexeme("Number", "double ", Parser().DoubleToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
					}
					else
					{
						int result = t1 * t2;
						return new lexeme("Number", "int ", Parser().IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
					}
				}
				if (strcmp(temp2->Data(), "%") == 0)
				{
					int result = t1 % t2;
					return new lexeme("Number", "int ", Parser().IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
				}
				// "==", ">=", "<=", "!=", ">", "<"
				bool result = false;
				if (strcmp(temp2->Data(), "==") == 0)
					result = (temp1 == temp2);
				if (strcmp(temp2->Data(), ">=") == 0)
					result = (temp1 >= temp2);
				if (strcmp(temp2->Data(), "<=") == 0)
					result = (temp1 <= temp2);
				if (strcmp(temp2->Data(), "!=") == 0)
					result = (temp1 != temp2);
				if (strcmp(temp2->Data(), ">") == 0)
					result = (temp1 > temp2);
				if (strcmp(temp2->Data(), "<") == 0)
					result = (temp1 < temp2);
				return new lexeme("Bool", "bool ", Parser().BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
			}
			else
			{
				if (IsCastable(temp3->Type(), temp1->Type()))
				{

				}
				else
				{
					_error = true;
					ErrorReporter().FReport(stdout, "Ќевозможно приведение!", temp2->Line(), temp2->Start_Position());
					return nullptr;
				}
			}
	}
	if (temp1->Type() == DOUBLE)
	{
		// "+", "-", "/", "*", "%" "==", ">=", "<=", "!=", ">", "<"
		double t1 = Parser().ToDouble(temp1->Data());
		double t2 = Parser().ToDouble(temp3->Data());
		double result;
		bool f = false;
		if (strcmp(temp2->Data(), "+") == 0)
		{
			f = true;
			result = t1 + t2;
		}
		if (strcmp(temp2->Data(), "-") == 0)
		{
			f = true;
			result = t1 - t2;
		}
		if (strcmp(temp2->Data(), "/") == 0)
		{
			f = true;
			result = t1 / t2;
		}
		if (strcmp(temp2->Data(), "*") == 0)
		{
			f = true;
			result = t1 * t2;
		}
		if (strcmp(temp2->Data(), "%") == 0)
		{
			ErrorReporter().FReport(stdout, "¬ыражение должно относитьс€ к целочисленному типу или типу перечислени€ без области видимости", temp1->Line(), temp1->Start_Position());
			_error = true;
			return nullptr;
		}
		if (f)
			return new lexeme("Number", "double ", Parser().DoubleToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		// "==", ">=", "<=", "!=", ">", "<"
		bool result2 = false;
		if (strcmp(temp2->Data(), "==") == 0)
			result2 = (temp1 == temp2);
		if (strcmp(temp2->Data(), ">=") == 0)
			result2 = (temp1 >= temp2);
		if (strcmp(temp2->Data(), "<=") == 0)
			result2 = (temp1 <= temp2);
		if (strcmp(temp2->Data(), "!=") == 0)
			result2 = (temp1 != temp2);
		if (strcmp(temp2->Data(), ">") == 0)
			result2 = (temp1 > temp2);
		if (strcmp(temp2->Data(), "<") == 0)
			result2 = (temp1 < temp2);
		return new lexeme("Bool", "bool ", Parser().BoolToString(result2), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (temp1->Type() == FLOAT)
	{
		// "+", "-", "/", "*", "%"
		float t1 = Parser().ToFloat(temp1->Data());
		float t2 = Parser().ToFloat(temp3->Data());
		float result;
		bool f = false;
		if (strcmp(temp2->Data(), "+") == 0)
		{
			f = true;
			result = t1 + t2;
		}
		if (strcmp(temp2->Data(), "-") == 0)
		{
			f = true;
			result = t1 - t2;
		}
		if (strcmp(temp2->Data(), "/") == 0)
		{
			f = true;
			result = t1 / t2;
		}
		if (strcmp(temp2->Data(), "*") == 0)
		{
			f = true;
			result = t1 * t2;
		}
		if (strcmp(temp2->Data(), "%") == 0)
		{
			ErrorReporter().FReport(stdout, "¬ыражение должно относитьс€ к целочисленному типу или типу перечислени€ без области видимости", temp1->Line(), temp1->Start_Position());
			_error = true;
			return nullptr;
		}
		if (f)
			return new lexeme("Number", "float ", Parser().FloatToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		// "==", ">=", "<=", "!=", ">", "<"
		bool result2 = false;
		if (strcmp(temp2->Data(), "==") == 0)
			result2 = (temp1 == temp2);
		if (strcmp(temp2->Data(), ">=") == 0)
			result2 = (temp1 >= temp2);
		if (strcmp(temp2->Data(), "<=") == 0)
			result2 = (temp1 <= temp2);
		if (strcmp(temp2->Data(), "!=") == 0)
			result2 = (temp1 != temp2);
		if (strcmp(temp2->Data(), ">") == 0)
			result2 = (temp1 > temp2);
		if (strcmp(temp2->Data(), "<") == 0)
			result2 = (temp1 < temp2);
		return new lexeme("Bool", "bool ", Parser().BoolToString(result2), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (temp1->Type() == BOOL)
	{
		//"==", "||", "&&", "!="
		bool t1 = Parser().ToBool(temp1->Data());
		bool t2 = Parser().ToBool(temp3->Data());
		bool result;
		if (strcmp(temp2->Data(), "==") == 0)
			result = (t1 == t2);
		if (strcmp(temp2->Data(), "||") == 0)
			result = (t1 || t2);
		if (strcmp(temp2->Data(), "&&") == 0)
			result = (t1 && t2);
		if (strcmp(temp2->Data(), "!=") == 0)
			result = (t1 != t2);
		return new lexeme("Bool", "bool ", Parser().BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (temp1->Type() == STRING || temp1->Type() == CHAR)
	{
		//"==", "+", "!="
		char* t1 = temp1->Data();
		char* t2 = temp3->Data();
		if (strcmp(temp2->Data(), "==") == 0)
		{
			bool result = false;
			if (strcmp(t1, t2) == 0)
				result = true;
			return new lexeme("Bool", "bool ", Parser().BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
		if (strcmp(temp2->Data(), "!=") == 0)
		{
			bool result = false;
			if (strcmp(t1, t2) != 0)
				result = true;
			return new lexeme("Bool", "bool ", Parser().BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
		if (strcmp(temp2->Data(), "+") == 0)
		{
			char* result = (char*)heap.get_mem(sizeof(char) * (strlen(t1) + strlen(t2) + 1));
			sprintf_s(result, (strlen(t1) + strlen(t2) + 1), "%s%s", t1, t2);
			return new lexeme("String", "string ", result, temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
	}
	return nullptr;
}
*/

bool LexemeWorker::IsNumberExpression(List * expression)
{
	//char* perone[] = { "++", "--", "+", "-", "(", "/", "*", ")", "%", "==", ">=", "<=", "!=", ">", "<" };
	char* perone[] = { "++", "--", "<<", ">>", "+", "-", "/", "*", "%", "==", ">=", "<=", "!=", ">", "<" };
	int types[] = { INT, FLOAT, DOUBLE };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression);
}

bool LexemeWorker::IsBoolExpression(List * expression)
{
	char* perone[] = { "==", "||", "&&", "!=" };
	int types[] = { BOOL };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression);
}

bool LexemeWorker::IsStringExpression(List * expression)
{
	char* perone[] = { "==", "+", "!="};
	int types[] = { STRING, CHAR };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(int)), types, (sizeof(types) / sizeof(char)), expression);
}
