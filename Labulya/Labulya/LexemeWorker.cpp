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
						{
							ErrorReporter().FReport(stdout, "Diffrent variable and value types!", temp_lexeme->Line(), temp_lexeme->Start_Position());
							return false;
						}
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
					/*
					if (strcmp(temp->Data(), "++") == 0 || strcmp(temp->Data(), "--") == 0)
					{
						//проверка на то где операция перед или после переменной
						temp_list->add((lexeme*)expression->get(pos - 1));
						temp_list->add((lexeme*)expression->get(pos));
						expression->set(pos - 1, NExpressionSolver(temp_list));
						expression->remove(pos);
					}
					else
					*/
					{
						temp_list->add((lexeme*)expression->get(pos - 1));
						temp_list->add((lexeme*)expression->get(pos));
						temp_list->add((lexeme*)expression->get(pos + 1));
						expression->set(pos - 1, NExpressionSolver(temp_list));
						expression->remove(pos + 1);
						expression->remove(pos);
					}
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
				/*
				case 2:
					if (strcmp(((lexeme*)expression->get(0))->Type(), "Operation") == 0)
						return Exe2Pression((lexeme*)expression->get(0), (lexeme*)expression->get(1), false);
					return Exe2Pression((lexeme*)expression->get(0), (lexeme*)expression->get(1), true);
					break;
				*/
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

lexeme * LexemeWorker::Exe3Pression(List* expression)
{
	lexeme* temp1 = (lexeme*)expression->get(0);
	lexeme* temp2 = (lexeme*)expression->get(1);
	lexeme* temp3 = (lexeme*)expression->get(2);
	if (strcmp(temp1->Type(), "int ") == 0)
	{
		// "+", "-", "/", "*", "%", "==", ">=", "<=", "!=", ">", "<"
		int t1 = parser.ToInt(temp1->Data());
		int t2 = parser.ToInt(temp3->Data());
		if (strcmp(temp2->Data(), "+") == 0)
		{
			if (t1 + t2 > INT_MAX)
			{
				ErrorReporter().FReport(stdout, "Out of value range!", temp2->Line(), temp2->Start_Position());
				_error = true;
				return nullptr;
			}
			int result = t1 + t2;
			return new lexeme("Number", "int ", parser.IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
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
			return new lexeme("Number", "int ", parser.IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
		if (strcmp(temp2->Data(), "/") == 0)
		{
			if (t1 % t2 == 0)
			{
				int result = t1 / t2;
				return new lexeme("Number", "int ", parser.IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
			}
			else
			{
				double result = t1 / t2;
				return new lexeme("Number", "double ", parser.DoubleToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
			}
		}
		if (strcmp(temp2->Data(), "*") == 0)
		{
			if (t1 * t2 > INT_MAX)
			{
				double result = t1 * t2;
				return new lexeme("Number", "double ", parser.DoubleToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
			}
			else
			{
				int result = t1 * t2;
				return new lexeme("Number", "int ", parser.IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
			}
		}
		if (strcmp(temp2->Data(), "%") == 0)
		{
			int result = t1 % t2;
			return new lexeme("Number", "int ", parser.IntToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
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
		return new lexeme("Bool", "bool ", parser.BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (strcmp(temp1->Type(), "double ") == 0)
	{
		// "+", "-", "/", "*", "%" "==", ">=", "<=", "!=", ">", "<"
		double t1 = parser.ToDouble(temp1->Data());
		double t2 = parser.ToDouble(temp3->Data());
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
			ErrorReporter().FReport(stdout, "Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", temp1->Line(), temp1->Start_Position());
			_error = true;
			return nullptr;
		}
		if (f)
			return new lexeme("Number", "double ", parser.DoubleToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
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
		return new lexeme("Bool", "bool ", parser.BoolToString(result2), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (strcmp(temp1->Type(), "float ") == 0)
	{
		// "+", "-", "/", "*", "%"
		float t1 = parser.ToFloat(temp1->Data());
		float t2 = parser.ToFloat(temp3->Data());
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
			ErrorReporter().FReport(stdout, "Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", temp1->Line(), temp1->Start_Position());
			_error = true;
			return nullptr;
		}
		if (f)
			return new lexeme("Number", "float ", parser.FloatToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
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
		return new lexeme("Bool", "bool ", parser.BoolToString(result2), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (strcmp(temp1->Type(), "bool ") == 0)
	{
		//"==", "||", "&&", "!="
		bool t1 = parser.ToBool(temp1->Data());
		bool t2 = parser.ToBool(temp3->Data());
		bool result;
		if (strcmp(temp2->Data(), "==") == 0)
			result = (t1 == t2);
		if (strcmp(temp2->Data(), "||") == 0)
			result = (t1 || t2);
		if (strcmp(temp2->Data(), "&&") == 0)
			result = (t1 && t2);
		if (strcmp(temp2->Data(), "!=") == 0)
			result = (t1 != t2);
		return new lexeme("Bool", "bool ", parser.BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
	}
	if (strcmp(temp1->Type(), "string ") == 0 || strcmp(temp1->Type(), "char ") == 0)
	{
		//"==", "+", "!="
		char* t1 = temp1->Data();
		char* t2 = temp3->Data();
		if (strcmp(temp2->Data(), "==") == 0)
		{
			bool result = false;
			if (strcmp(t1, t2) == 0)
				result = true;
			return new lexeme("Bool", "bool ", parser.BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
		if (strcmp(temp2->Data(), "!=") == 0)
		{
			bool result = false;
			if (strcmp(t1, t2) != 0)
				result = true;
			return new lexeme("Bool", "bool ", parser.BoolToString(result), temp1->Line(), temp1->Start_Position(), temp1->Priority());
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

/*
lexeme * LexemeWorker::Exe2Pression(lexeme* temp1, lexeme* temp2, bool after)
{
	//{ "++", "--" }
	if (after)
	{
		if (strcmp(temp1->Type(), "int ") == 0)
		{
			int t1 = parser.ToInt(temp1->Data());
			if (strcmp(temp2->Data(), "++") == 0)
				t1++;
			if (strcmp(temp2->Data(), "--") == 0)
				t1--;
			return new lexeme("Number", "int ", parser.IntToString(t1), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
		if (strcmp(temp1->Type(), "double ") == 0)
		{
			double t1 = parser.ToDouble(temp1->Data());
			if (strcmp(temp2->Data(), "++") == 0)
				t1++;
			if (strcmp(temp2->Data(), "--") == 0)
				t1--;
			return new lexeme("Number", "double ", parser.DoubleToString(t1), temp1->Line(), temp1->Start_Position(), temp1->Priority());

		}
		if (strcmp(temp1->Type(), "float ") == 0)
		{
			float t1 = parser.ToFloat(temp1->Data());
			if (strcmp(temp2->Data(), "++") == 0)
				t1++;
			if (strcmp(temp2->Data(), "--") == 0)
				t1--;
			return new lexeme("Number", "float ", parser.FloatToString(t1), temp1->Line(), temp1->Start_Position(), temp1->Priority());
		}
	}
	return nullptr;
}
*/

bool LexemeWorker::IsNumberExpression(List * expression)
{
	//char* perone[] = { "++", "--", "+", "-", "(", "/", "*", ")", "%", "==", ">=", "<=", "!=", ">", "<" };
	char* perone[] = { "+", "-", "(", "/", "*", ")", "%", "==", ">=", "<=", "!=", ">", "<" };
	char* types[] = { "int ", "float ", "double " };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(char)), types, (sizeof(types) / sizeof(char)), expression);
}

bool LexemeWorker::IsBoolExpression(List * expression)
{
	char* perone[] = { "==", "||", "&&", "(", ")", "!=" };
	char* types[] = { "bool " };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(char)), types, (sizeof(types) / sizeof(char)), expression);
}

bool LexemeWorker::IsStringExpression(List * expression)
{
	char* perone[] = { "==", "+", "!=", "(", ")"};
	char* types[] = { "string ", "char " };
	return WhateverCheck(perone, (sizeof(perone) / sizeof(char)), types, (sizeof(types) / sizeof(char)), expression);
}
