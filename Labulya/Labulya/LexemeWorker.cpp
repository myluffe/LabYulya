#include "stdafx.h"
#include "LexemeWorker.h"

bool LexemeWorker::Processing(List* lexes)
{
	//изменение
	Lexeme_list* dob = new Lexeme_list(); //стэк для области видимости
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), "{") == 0)
		{
			dob->add(temp_lexeme);
			continue;
		}
		if (strcmp(temp_lexeme->Data(), "[") == 0) //обработка массивов, из размещение в хэш-таблицу
		{
			lexeme* ttype = nullptr;
			lexeme* name = nullptr;
			List* sizes = new List(sizeof(int));
			int namepos = 0;
			ttype = (lexeme*)lexes->get(i - 1);
			if (ttype == nullptr || ttype->Type() != TYPE)
			{
				//...
				continue;
			}
			int k = 0;
			bool open = false;
			lexeme* tempdevider;
			while (true)
			{
				tempdevider = (lexeme*)lexes->get(i + k);
				if (tempdevider != nullptr && (strcmp(tempdevider->Data(), "[") == 0 || strcmp(tempdevider->Data(), "]") == 0))
				{
					k++;
					if (strcmp(tempdevider->Data(), "]") == 0)
					{
						open = false; 
						continue;
					}
					if (open)
					{
						errorReporter.FReport(logfile, "Ожидается \"]\"!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					if (strcmp(tempdevider->Data(), "[") == 0)
						open = true;
					if (k >= lexes->count())
					{
						errorReporter.FReport(logfile, "#909 Незаконченное выражение!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					tempdevider = (lexeme*)lexes->get(i + k);
					if (tempdevider->Type() == VARIABLE || tempdevider->Type() == MASSIVE)
						if (!GetValue(dob, tempdevider, lexes, i + k))
						{
							errorReporter.FReport(logfile, "Неопределенная переменная!", tempdevider->Line(), tempdevider->Start_Position());
							return false;
						}
					if (tempdevider->Type() == INT)
					{
						int ghp = (int)parser.ToInt(tempdevider->Data());
						sizes->add(&ghp);
						k++;
					}	
					else
					{
						errorReporter.FReport(logfile, "Ожидается значение типа int!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					continue;
				}
				if (tempdevider != nullptr && tempdevider->Type() == VARIABLE)
				{
					namepos = i + k;
					name = tempdevider;
					if (dob->findpos(name->Data()) != -1)
					{
						errorReporter.FReport(logfile, "Имя переменной уже используется!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					break;
				}
				errorReporter.FReport(logfile, "Ожидается \"[\" или \"]\"!", tempdevider->Line(), tempdevider->Start_Position());
				return false;
			}
			if (k % 3 != 0)
			{
				errorReporter.FReport(logfile, "В конце ожидается \"]\"!", tempdevider->Line(), tempdevider->Start_Position());
				return false;
			}
			int rank = k / 3;
			k++;
			tempdevider = (lexeme*)lexes->get(i + k);
			if (tempdevider == nullptr || strcmp(tempdevider->Data(), "=") != 0)
			{
				errorReporter.FReport(logfile, "Ожидается \"=\"!", tempdevider->Line(), tempdevider->Start_Position());
				return false;
			}
			k++;
			int posend = k + i;
			//...ф-ция разбора элементов и записи их в память, возвратит ссылку на первый элемент, в posend запишет позицию ";" (конца выражения).
			lexeme* tempvalues = GetMassValues(lexes, k + i, ttype->Data(), &posend, dob, rank, sizes);
			if (posend > k + 1)
			{
				lexeme* newlex = (lexeme*)lexes->get(namepos);
				newlex->ToMass(name, MASSIVE, tempvalues, rank);
				//в дов уже готовую добавляем
				dob->add(newlex);
				//dob->print_list();
				//добавление в хэш-таблицу
				LexemeTable.auto_create(newlex);
				//...формирование лексемы массива на позиции namepos
				//удаление лишних лексем
				for (int g = posend; g >= i - 1; g--)
					lexes->remove(g);
				//смещение на нужную позицию в цикле
				i-=2;
			}
			else
				return false;
			continue;
		}
		if (temp_lexeme->Type() == VARIABLE) //обработка переменных, из размещение в хэш-таблицу 
		{
			if (dob->find(temp_lexeme->Data()) == nullptr) //если нет в стеке для области видимости
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
							errorReporter.FReport(logfile, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + (int)strlen(temp_lexeme->Data()));
							return false;
						}
						lexeme* tdata = (lexeme*)lexes->get(i + 2);
						if (!(tdata->Type() == INT || tdata->Type() == DOUBLE || tdata->Type() == FLOAT || tdata->Type() == CHAR || tdata->Type() == STRING || tdata->Type() == BOOL))
						{
							if (tdata->Type() == VARIABLE || tdata->Type() == MASSIVE)
							{
								if (!GetValue(dob, tdata, lexes, i + 2))
									return false;
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
						lexes->remove(i + 3);
						lexes->remove(i + 2);
						lexes->remove(i + 1);
						lexes->remove(i);
						lexes->remove(i - 1);
						i = ti;
					}
					else
					{
						errorReporter.FReport(logfile, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + (int)strlen(temp_lexeme->Data()));
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
						errorReporter.FReport(logfile, "This variable is alredy defined!", temp_lexeme->Line(), temp_lexeme->Start_Position());
						return false;
					}
					else
					{
						//заменяем на ссылку
						//dob->print_list();
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
					errorReporter.FReport(logfile, "No type of undefined variable!", temp_lexeme->Line(), temp_lexeme->Start_Position());
					return false;
				}
			}
			continue;
		}
		if (temp_lexeme->Type() == NUMBER)
		{
			//обработка для чисел
			continue;
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
			continue;
		}
	}

	TList* storage = new TList();
	List* expression = new List(sizeof(lexeme*));
	for (int km = 0; km < lexes->count(); km++)
	{
		lexeme* tl = (lexeme*)lexes->get(km);
		expression->add(&tl);
	}

	for (int i = 0; i < expression->count(); i++)
	{
		lexeme* temp_lexeme = *(lexeme**)expression->get(i);
		if (strcmp(temp_lexeme->Data(), "{") == 0 || strcmp(temp_lexeme->Data(), "}") == 0)
		{
			expression->remove(i);
			i--;
			continue;
		}
		if (temp_lexeme->Type() == SPECIALWORD)
		{
			i = CorrectSpecial(temp_lexeme, i, expression, storage);
			if (_error) return false;
		}
		else
		{
			i = CorrectExpression(expression, i, storage);
		}
	}

	//отладка
	printf_s("\n|---------------|\nVariable Table:\n");
	LexemeTable.print_lexems();
	printf_s("\n|---------------|\n");
	printf_s("Tree View:\n");
	storage->print();
	storage->exec();
	printf_s("\n|---------------|\nVariable Table:\n");
	LexemeTable.print_lexems();
	printf_s("|---------------|\n");
	//
	dob->~Lexeme_list();
	storage->~TList();
	return true;
}

LexemeWorker::LexemeWorker()
{
}


LexemeWorker::~LexemeWorker()
{
}

int LexemeWorker::CorrectExpression(List* expression, int pos, TList* storage)
{
	int start = pos; //fix
	int i;
	List* tlist = new List(sizeof(sizeof(lexeme*)));

	for (i = pos; i < expression->count(); i++)
	{
		lexeme* temp_lexeme = *(lexeme**)expression->get(i);
		tlist->add(&temp_lexeme);
		if (strcmp(temp_lexeme->Data(), "?") == 0)
		{
			i = CorrectShortIfOperation(expression, i, storage);
		}
		if (strcmp(temp_lexeme->Data(), ";") == 0)
		{
			start = i + 1;
			break;
		}
	}
	if (IsNumberExpression(tlist, true) || IsBoolExpression(tlist, true) || IsStringExpression(tlist, true) || IsNumberExpressionWithBoolOperations(tlist) || IsStringExpressionWithBoolOperations(tlist))
	{
		storage->addNode(treeWorker.GetTNode(tlist, 0, tlist->count() - 2));
		tlist->~List();
		return i;
	}

	tlist->~List();
	return pos;
}

int LexemeWorker::CorrectShortIfOperation(List * expression, int i, TList* storage)
{	
	List* shrtiflist = new List(sizeof(sizeof(lexeme*)));
	TNode* param1;
	TNode* param2;
	TNode* param3;
	int hookcount;
	bool flag = false;
	int j;
	for (j = i - 1; j >= 0; j--)
	{
		lexeme* tlex = (lexeme*)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			hookcount++;
		if (strcmp(tlex->Data(), "(") == 0)
			hookcount--;
		if (hookcount < 0)
			break;
	}
	if (j < 0)
	{
		lexeme* tlex = *(lexeme**)expression->get(i);
		for (int k = 0; k < i; k++)
			shrtiflist->add(&tlex);
		if (IsBoolExpression(shrtiflist, false) || IsNumberExpressionWithBoolOperations(shrtiflist) || IsStringExpressionWithBoolOperations(shrtiflist))
		{
			param1 = treeWorker.GetTNode(shrtiflist, 0, i - 1);
			shrtiflist->~List();
		}
		else
		{
			lexeme* t = *(lexeme**) expression->get(i);
			shrtiflist->~List();
			ErrorReporter().FReport(logfile, "Невозможно определить условие \"?\"!", t->Line(), t->Start_Position());
			return i;
		}
	}
	else
	{
		flag = true;
		for (int k = j + 1; k < i; k++)
		{
			lexeme* tlex = *(lexeme**)expression->get(i);
			shrtiflist->add(&tlex);
		}
		if (IsBoolExpression(shrtiflist, false) || IsNumberExpressionWithBoolOperations(shrtiflist) || IsStringExpressionWithBoolOperations(shrtiflist))
		{
			param1 = treeWorker.GetTNode(shrtiflist, 0, i - 1);
			shrtiflist->~List();
		}
		else
		{
			lexeme* t = *(lexeme**)expression->get(i);
			ErrorReporter().FReport(logfile, "Невозможно определить условие \"?\"!", t->Line(), t->Start_Position());
			shrtiflist->~List();
			return i;
		}
	}

	List* body1 = new List(sizeof(sizeof(lexeme*)));
	int h = i + 1;
	for (h; h < expression->count(); h++)
	{
		lexeme* tl = *(lexeme**)expression->get(h);
		if (strcmp(tl->Data(), ":") == 0)
			break;
		body1->add(&tl);
	}
	if (h = expression->count())
	{
		lexeme* t = *(lexeme**)expression->get(i);
		ErrorReporter().FReport(logfile, "Ожидается \":\" для \"?\"!", t->Line(), t->Start_Position());
		return i;
	}
	if (IsBoolExpression(body1, true) || IsNumberExpressionWithBoolOperations(body1) || IsStringExpressionWithBoolOperations(body1) || IsNumberExpression(body1, true) || IsStringExpression(body1, true))
	{
		param2 = treeWorker.GetTNode(body1, 0, body1->count() - 1);
		body1->~List();
	}
	else
	{
		ErrorReporter().FReport(logfile, "Выражение после \"?\" не найдено!", (*(lexeme**)expression->get(i))->Line(), i);
		body1->~List();
		return i;
	}

	List* body2 = new List(sizeof(sizeof(lexeme*)));
	int y = h + 1;
	for (y; y < expression->count(); y++)
	{
		lexeme* tl = *(lexeme**)expression->get(y);
		if (flag && strcmp(tl->Data(), ")") == 0)
			break;
		if (strcmp(tl->Data(), ";") == 0)
			break;
		body2->add(&tl);
	}
	if (y = expression->count())
	{
		lexeme* t = *(lexeme**)expression->get(i);
		ErrorReporter().FReport(logfile, "Невозможность определить конец \"?\"!", t->Line(), t->Start_Position());
		return i;
	}
	if (IsBoolExpression(body2, true) || IsNumberExpressionWithBoolOperations(body2) || IsStringExpressionWithBoolOperations(body2) || IsNumberExpression(body2, true) || IsStringExpression(body2, true))
	{
		param3 = treeWorker.GetTNode(body2, 0, body2->count() - 1);
		storage->addNode((TNode*)new TTernaryOperator(param1, param2, param3));
		body2->~List();
	}
	else
	{
		body2->~List();
		return i;
	}
	return i;
}

int LexemeWorker::CorrectSpecial(lexeme* spec, int pos, List* expression, TList* storage)
{
	if (pos == expression->count())
	{
		errorReporter.FReport(logfile, "Не законченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		return 0;
	}
	if (strcmp(spec->Data(), "for ") == 0)
	{
		return CorrectFor(expression, pos, spec, storage);
	}
	if (strcmp(spec->Data(), "if ") == 0)
	{
		return CorrectIf(expression, pos, spec, storage);
	}
	if (strcmp(spec->Data(), "while ") == 0)
	{
		return CorrectWhile(expression, pos, spec, storage);
	}
	if (strcmp(spec->Data(), "do ") == 0)
	{
		return CorrectDo(expression, pos, spec, storage);
	}
	if (strcmp(spec->Data(), "input ") == 0 || strcmp(spec->Data(), "output ") == 0)
	{
		List* param = new List(sizeof(lexeme*));
		int	res = FuncWithStringParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			//input or output
			//storage->addNode();
			param->~List();
			return res;
		}
		res = FuncWithNumberParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			//input or output
			//storage->addNode();
			param->~List();
			return res;
		}
		res = FuncWithBoolParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			//input or output
			//storage->addNode();
			param->~List();
			return res;
		}
		return pos;
	}
	if (strcmp(spec->Data(), "min ") == 0 || strcmp(spec->Data(), "max ") == 0)
	{
		List* param1 = new List(sizeof(lexeme*));
		List* param2 = new List(sizeof(lexeme*));
		int res = FuncWithTwoNumberParams(expression, pos, spec, true, param1, param2);
		if (res != pos)
		{
			//min or max node
			//storage->addNode();
		}
		param1->~List();
		param2->~List();
		return res;
	}
	if (strcmp(spec->Data(), "sin ") == 0 || strcmp(spec->Data(), "cos ") == 0)
	{
		List* param = new List(sizeof(lexeme*));
		int res = FuncWithNumberParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			//sin or cos
			//storage->addNode();
			param->~List();
			return res;
		}
		param->~List();
		return pos;
	}
	ErrorReporter().FReport(logfile, "Неизвестное или неправильно использованное спец.слово!", spec->Line(), spec->Start_Position());
	return pos;
}

int LexemeWorker::FuncWithTwoNumberParams(List * expression, int pos, lexeme * spec, bool finaldevider, List* param1, List* param2)
{
	lexeme* devider1 = *(lexeme**)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		param1->~List();
		param2->~List();
		_error = true;
		return pos;
	}
	lexeme* tlex = nullptr;
	lexeme* devider2 = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = *(lexeme**)expression->get(i);
		if (strcmp(tlex->Data(), ",") == 0)
		{
			devider2 = *(lexeme**)expression->get(i);
			break;
		}
		param1->add(&tlex);
	}
	if (!IsNumberExpression(param1, false))
	{
		errorReporter.FReport(logfile, "Ожидается числовое выражение!", devider1->Line(), devider1->Start_Position());
		param1->~List();
		param2->~List();
		return pos;
	}

	if (strcmp(tlex->Data(), ",") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \",\"", devider2->Line(), devider2->Start_Position());
		param1->~List();
		param2->~List();
		return pos;
	}
	
	lexeme* devider3 = nullptr;
	int j = 0;
	for (j = i + 1; j < expression->count(); j++)
	{
		tlex = *(lexeme**)expression->get(j);
		if (strcmp(tlex->Data(), ")") == 0)
		{
			devider3 = *(lexeme**)expression->get(i);
			break;
		}
		param2->add(&tlex);
	}
	if (!IsNumberExpression(param2, false))
	{
		errorReporter.FReport(logfile, "Ожидается числовое выражение!", devider1->Line(), devider1->Start_Position());
		param1->~List();
		param2->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		ErrorReporter().FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		param1->~List();
		param2->~List();
		_error = true;
		return pos;
	}
	if (!finaldevider)
	{
		return j;
	}
	tlex = *(lexeme**)expression->get(j + 1);
	if ((j + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		param1->~List();
		param2->~List();
		_error = true;
		return pos;
	}
	return j + 1;
}

int LexemeWorker::FuncWithStringParam(List * expression, int pos, lexeme * spec, bool finaldevider, List* param)
{
	lexeme* devider1 = *(lexeme**)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return pos;
	}
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = *(lexeme**)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		param->add(&tlex);
	}
	if (!IsStringExpression(param, false))
	{
		errorReporter.FReport(logfile, "Ожидается строковое выражение!", devider1->Line(), devider1->Start_Position());
		param->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		param->~List();
		return pos;
	}
	if (!finaldevider)
	{
		return i;
	}
	tlex = *(lexeme**)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		param->~List();
		return pos;
	}
	return i + 1;
}

int LexemeWorker::FuncWithBoolParam(List * expression, int pos, lexeme * spec, bool finaldevider, List* param)
{
	lexeme* devider1 = *(lexeme**)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return pos;
	}
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = *(lexeme**)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		param->add(&tlex);
	}
	if (!IsBoolExpression(param, false) && !IsNumberExpressionWithBoolOperations(param) && !IsStringExpressionWithBoolOperations(param))
	{
		errorReporter.FReport(logfile, "Ожидается выражение типа bool!", devider1->Line(), devider1->Start_Position());
		param->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		param->~List();
		return pos;
	}
	if (!finaldevider)
	{
		return i;
	}
	tlex = *(lexeme**)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		param->~List();
		return pos;
	}
	return i + 1;
}

int LexemeWorker::FuncWithNumberParam(List * expression, int pos, lexeme * spec, bool finaldevider, List* param)
{
	lexeme* devider1 = *(lexeme**)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"", devider1->Line(), devider1->Start_Position());
		_error = true;
		return pos;
	}
	lexeme* tlex = nullptr;
	int i = 0;
	for (i = pos + 2; i < expression->count(); i++)
	{
		tlex = *(lexeme**)expression->get(i);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		param->add(&tlex);
	}
	if (!IsNumberExpression(param, false))
	{
		errorReporter.FReport(logfile, "Ожидается числовое выражение!", devider1->Line(), devider1->Start_Position());
		param->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		param->~List();
		return pos;
	}
	if (!finaldevider)
	{
		return i;
	}
	tlex = *(lexeme**)expression->get(i + 1);
	if ((i + 1) >= expression->count() || strcmp(tlex->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
		_error = true;
		param->~List();
		return pos;
	}
	return i + 1;
}

bool LexemeWorker::InnerExpression(List * expression, TList* storage)
{
	lexeme* tlex;
	bool startstring = false;
	List* tlist = new List(sizeof(lexeme*));
	for (int i = 0; i < expression->count(); i++)
	{
		tlex = *(lexeme**)expression->get(i);
		if (strcmp(tlex->Data(), "?") == 0)
		{
			i = CorrectShortIfOperation(expression, i, storage);
		}
		if (tlex->Type() == SPECIALWORD)
		{
			if (startstring)
				ErrorReporter().FReport(logfile, "Ожидается \";\"", tlex->Line(), tlex->Start_Position());
			i = CorrectSpecial(tlex, i, expression, storage);
			
		}
		else
		{
			if (strcmp(tlex->Data(), ";") != 0)
			{
				if (!startstring) startstring = true;
				tlist->add(&tlex);
			}
			else
			{
				tlist->add(&tlex);
				if (!(IsNumberExpression(tlist, true) || IsBoolExpression(tlist, true) || IsStringExpression(tlist, true) || IsNumberExpressionWithBoolOperations(tlist) || IsStringExpressionWithBoolOperations(tlist)))
				{
					tlist->~List();
					return false;
				}
				storage->addNode(treeWorker.GetTNode(tlist, 0, tlist->count() - 2));
				//storage->print();
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
		lexeme* tlex = *(lexeme**)expression->get(k);
		if (tlex->Type() == DEVIDER)
		{
			if (strcmp(tlex->Data(), ")") == 0)
			{
				hook_count--;
				if (hook_count < 0)
				{
					errorReporter.FReport(logfile, "Вы забыли открыть скобку \"(\"!", tlex->Line(), tlex->Start_Position());
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
		lexeme* tlex = *(lexeme**)expression->get(expression->count() - 1);
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
	char* perone[] = { "++", "--", "<<", ">>", "+", "-", "/", "*", "%"};
	int types[] = { INT, FLOAT, DOUBLE };
	return WhateverCheck(perone, 9, types, (sizeof(types) / sizeof(int)), expression, equal);
}

bool LexemeWorker::IsNumberExpressionWithBoolOperations(List * expression)
{
	char* perone[] = { "==", ">=", "<=", "!=", ">", "<" };
	int types[] = { INT, FLOAT, DOUBLE };
	return WhateverCheck(perone, 6, types, (sizeof(types) / sizeof(int)), expression, false);
}

bool LexemeWorker::IsStringExpressionWithBoolOperations(List * expression)
{
	char* perone[] = { "==", "!=" };
	int types[] = { STRING, CHAR };
	return WhateverCheck(perone, 2, types, (sizeof(types) / sizeof(int)), expression, false);
}

bool LexemeWorker::IsBoolExpression(List * expression, bool equal)
{
	char* perone[] = { "==", "||", "&&", "!=" };
	int types[] = { BOOL };
	return WhateverCheck(perone, 4, types, (sizeof(types) / sizeof(int)), expression, equal);
}

bool LexemeWorker::IsStringExpression(List * expression, bool equal)
{
	char* perone[] = { "+" };
	int types[] = { STRING, CHAR };
	return WhateverCheck(perone, 1, types, (sizeof(types) / sizeof(int)), expression, equal);
}

int LexemeWorker::CorrectWhile(List * expression, int pos, lexeme * spec, TList* storage)
{
	List* hl = new List(sizeof(lexeme*));
	int pos2 = FuncWithBoolParam(expression, pos, spec, false, hl);
	if (pos2 <= pos)
	{
		ErrorReporter().FReport(logfile, "Незаконченное выражение!", spec->Line(), spec->Start_Position() + (int)strlen(spec->Data()));
		_error = true;
		return pos;
	}
	TNode* h = treeWorker.GetTNode(hl, 0, hl->count() - 1);
	pos2++;
	
	List* tlist = new List(sizeof(lexeme*));
	if (pos2 < expression->count())
	{
		lexeme* tlex = *(lexeme**)expression->get(pos2);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos2; pos2 < expression->count(); pos2++)
			{
				tlex = *(lexeme**)expression->get(pos2);
				if (strcmp(tlex->Data(), ";") != 0)
					tlist->add(&tlex);
				else
				{
					tlist->add(&tlex);
					break;
				}
			}
		}
		else
		{
			pos2++;
			for (pos2; pos2 < expression->count(); pos2++)
			{
				tlex = *(lexeme**)expression->get(pos2);
				if (strcmp(tlex->Data(), "}") != 0)
					tlist->add(tlex);
				else
				{
					break;
				}
			}
		}
		TList* body = new TList();
		if (InnerExpression(tlist, body))
		{
			storage->addNode((TNode*)(new TWhile(h, body)));
			tlist->~List();
			return pos2;
		}
		else
		{
			_error = true;
			tlist->~List();
			return pos;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Незаконченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist->~List();
		return pos;
	}

	tlist->~List();
	return pos;
}

int LexemeWorker::CorrectDo(List * expression, int origpos, lexeme * spec, TList* storage)
{
	List* tlist = new List(sizeof(lexeme*));
	int pos = origpos;
	if (pos < expression->count())
	{
		pos++;
		lexeme* tlex = *(lexeme**)expression->get(pos);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos; pos < expression->count(); pos++)
			{
				tlex = *(lexeme**)expression->get(pos);
				if (strcmp(tlex->Data(), ";") != 0)
					tlist->add(&tlex);
				else
				{
					break;
				}
			}
		}
		else
		{
			pos++;
			for (pos; pos < expression->count(); pos++)
			{
				tlex = *(lexeme**)expression->get(pos);
				if (strcmp(tlex->Data(), "}") != 0)
					tlist->add(&tlex);
				else
				{
					break;
				}
			}
		}
		TList* body = new TList();
		if (InnerExpression(tlist, body))
		{
			tlist->~List();
			{
				pos++;
				if (pos <= expression->count())
				{
					tlex = *(lexeme**)expression->get(pos);
					if (strcmp(tlex->Data(), "while ") != 0)
					{
						errorReporter.FReport(logfile, "Ожидается \"while \" после тела цикла \"do \"!", tlex->Line(), tlex->Start_Position());
						_error = true;
						tlist->~List();
						return origpos;
					}
					List* hl = new List(sizeof(lexeme*));
					int pos2 = FuncWithBoolParam(expression, pos, spec, true, hl);
					if (pos2 <= pos)
					{
						_error = true;
						return origpos;
					}
					TNode* h = treeWorker.GetTNode(hl, 0, hl->count() - 1);
					hl->~List();
					storage->addNode((TNode*)new TDoWhile(body, h));
					return pos2;
				}
				else
				{
					ErrorReporter().FReport(logfile, "Незаконченное выражение!", spec->Line(), spec->Start_Position());
					_error = true;
					tlist->~List();
					return origpos;
				}
			}
		}
		else
		{
			_error = true;
			tlist->~List();
			return origpos;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Ожидается тело цикла \"do \"!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist->~List();
		return origpos;
	}
	return origpos;
}

int LexemeWorker::CorrectFor(List * expression, int origpos, lexeme * spec, TList* storage)
{
	int pos = origpos;
	pos++;
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Ожидаются параметры для \"for \"!", spec->Line(), spec->Start_Position());
		_error = true;
		return origpos;
	}
	lexeme* tlex = *(lexeme**)expression->get(pos);
	if (strcmp(tlex->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"(\"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		return origpos;
	}
	pos++;
	List* param1 = new List(sizeof(lexeme*));
	for (pos; pos < expression->count(); pos++)
	{
		tlex = *(lexeme**)expression->get(pos);
		if (strcmp(tlex->Data(), ";") == 0)
			break;
		param1->add(&tlex);
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Незаконченное выражение \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		return origpos;
	}
	if (param1->count() != 1)
	{
		errorReporter.FReport(logfile, "Ожидается инициализированная переменная в качестве первого параметра!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		return origpos;
	}
	lexeme* first = *(lexeme**)param1->get(0);
	if (!(first->Type() == INT || first->Type() == DOUBLE || first->Type() == FLOAT))
	{
		errorReporter.WarningReport(logfile, "Возможно неверный тип первого пареметра \"for \"", tlex->Line(), tlex->Start_Position());
	}

	List* param2 = new List(sizeof(lexeme*));
	pos++;
	for (pos; pos < expression->count(); pos++)
	{
		tlex = *(lexeme**)expression->get(pos);
		if (strcmp(tlex->Data(), ";") == 0)
			break;
		param2->add(&tlex);
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Незаконченное выражение \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		return origpos;
	}
	if (!IsNumberExpressionWithBoolOperations(param2) && !IsBoolExpression(param2, false) && !IsNumberExpressionWithBoolOperations(param2))
	{
		errorReporter.FReport(logfile, "Неверный второй параметр \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param2->~List();
		param1->~List();
		return origpos;
	}

	List* param3 = new List(sizeof(lexeme*));
	pos++;
	for (pos; pos < expression->count(); pos++)
	{
		tlex = *(lexeme**)expression->get(pos);
		if (strcmp(tlex->Data(), ")") == 0)
			break;
		param3->add(&tlex);
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \")\"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		param3->~List();
		return origpos;
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "Незаконченное выражение \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		param3->~List();
		return origpos;
	}
	if (!IsNumberExpression(param3, true))
	{
		errorReporter.FReport(logfile, "Неверный третий параметр \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param3->~List();
		param2->~List();
		param1->~List();
		return origpos;
	}

	pos++;
	List* body = new List(sizeof(lexeme*));
	if (pos < expression->count())
	{
		tlex = *(lexeme**)expression->get(pos);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos; pos < expression->count(); pos++)
			{
				tlex = *(lexeme**)expression->get(pos);
				if (strcmp(tlex->Data(), ";") != 0)
					body->add(&tlex);
				else
				{
					body->add(&tlex);
					break;
				}
			}
		}
		else
		{
			pos++;
			for (pos; pos < expression->count(); pos++)
			{
				tlex = *(lexeme**)expression->get(pos);
				if (strcmp(tlex->Data(), "}") != 0)
					body->add(&tlex);
				else
					break;
			}
		}
		TList* sbody = new TList();
		if (InnerExpression(body, sbody))
		{
			TVariable* tparam1 = new TVariable(*(lexeme**)param1->get(0));
			TNode* tparam2 = treeWorker.GetTNode(param2, 0,  param2->count() - 1);
			TNode* tparam3 = treeWorker.GetTNode(param3, 0, param3->count() - 1);
			storage->addNode((TNode*)new TFor(tparam1, tparam2, tparam3, sbody));
			//body->~List();
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
			return origpos;
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
		return origpos;
	}

	body->~List();
	param2->~List();
	param2->~List();
	param1->~List();
	return origpos;
}

int LexemeWorker::CorrectIf(List * expression, int origpos, lexeme * spec, TList* storage)
{
	List* hl = new List(sizeof(lexeme*));
	int pos = origpos;
	int pos3 = FuncWithBoolParam(expression, pos, spec, false, hl);
	if (pos3 <= pos)
	{
		_error = true;
		return origpos;
	}
	TNode* h = treeWorker.GetTNode(hl, 0, hl->count() - 1);
	
	hl->~List();
	TList* body;
	pos3++;

	List* tlist1 = new List(sizeof(lexeme*));
	if (pos3 < expression->count())
	{
		lexeme* tlex = *(lexeme**)expression->get(pos3);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos3; pos3 < expression->count(); pos3++)
			{
				tlex = *(lexeme**)expression->get(pos3);
				if (strcmp(tlex->Data(), ";") != 0)
					tlist1->add(&tlex);
				else
				{
					tlist1->add(&tlex);
					break;
				}
			}
		}
		else
		{
			pos3++;
			for (pos3; pos3 < expression->count(); pos3++)
			{
				tlex = *(lexeme**)expression->get(pos3);
				if (strcmp(tlex->Data(), "}") != 0)
					tlist1->add(&tlex);
				else
				{
					break;
				}
			}
		}
		body = new TList();
		if (InnerExpression(tlist1, body))
		{
			pos = pos3;
			tlist1->~List();
		}
		else
		{
			_error = true;
			tlist1->~List();
			return origpos;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Незаконченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist1->~List();
		return origpos;
	}
	
	if (pos >= expression->count())
	{
		storage->addNode((TNode*)new TIf(h, body, nullptr));
		return pos;
	}
	pos++;
	lexeme* tlex = *(lexeme**)expression->get(pos);
	if (strcmp(tlex->Data(), "else ") != 0)
	{
		storage->addNode((TNode*)new TIf(h, body, nullptr));
		
		return pos - 1;
	}
	pos++;
	pos3 = pos;
	List* tlist2 = new List(sizeof(lexeme*));
	if (pos3 < expression->count())
	{
		lexeme* tlex = *(lexeme**)expression->get(pos3);
		if (strcmp(tlex->Data(), "{") != 0)
		{
			for (pos3; pos3 < expression->count(); pos3++)
			{
				tlex = *(lexeme**)expression->get(pos3);
				if (strcmp(tlex->Data(), ";") != 0)
					tlist2->add(&tlex);
				else
				{
					tlist2->add(&tlex);
					break;
				}
			}
		}
		else
		{
			pos3++;
			for (pos3; pos3 < expression->count(); pos3++)
			{
				tlex = *(lexeme**)expression->get(pos3);
				if (strcmp(tlex->Data(), "}") != 0)
					tlist2->add(&tlex);
				else
				{
					break;
				}
			}
		}
		TList* body2 = new TList();
		if (InnerExpression(tlist1, body2))
		{
			storage->addNode((TNode*)new TIf(h, body, body2));
			pos = pos3;
			tlist2->~List();
		}
		else
		{
			_error = true;
			tlist2->~List();
			return origpos;
		}
	}
	else
	{
		errorReporter.FReport(logfile, "Незаконченное выражение!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist2->~List();
		return origpos;
	}
	return pos;
}

lexeme * LexemeWorker::GetMassValues(List* expression, int start, char* ttype, int* putend, Lexeme_list* dob, int rank, List* sizes)
{
	List* poses = new List(sizeof(int));
	int type = GetType(ttype);
	int currentpos = start;

	//int* tempsizes = new int[sizes->count() + 1];
	int* tempsizes = (int*)heap.get_mem(sizeof(int) * (sizes->count() + 1));
	for (int m = 0; m < sizes->count(); m++)
	{
		int h = *(int*)sizes->get(m);
		tempsizes[m] = (*(int*)sizes->get(m));
	}

	lexeme* temp = (lexeme*)expression->get(currentpos);
	if (temp == nullptr || strcmp(temp->Data(), "{") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \"{\"", temp->Line(), temp->Start_Position());
		heap.free_mem(tempsizes);
		return nullptr;
	}
	currentpos++;
	//отбор и подсчет элементов
	if (!ReInnerFind(expression, &currentpos, 0, tempsizes, rank, poses, dob, type))
	{
		heap.free_mem(tempsizes);
		return nullptr;
	}
	int stemp = currentpos;
	for (int s = stemp; s < stemp + rank - 1; s++)
	{
		temp = (lexeme*)expression->get(currentpos);
		if (temp == nullptr || strcmp(temp->Data(), "}") != 0)
		{
			errorReporter.FReport(logfile, "Ожидается \"}\"", temp->Line(), temp->Start_Position());
			heap.free_mem(tempsizes);
			return nullptr;
		}
		currentpos++;
	}
	temp = (lexeme*)expression->get(currentpos);
	if (temp == nullptr || strcmp(temp->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "Ожидается \";\"", temp->Line(), temp->Start_Position());
		heap.free_mem(tempsizes);
		return nullptr;
	}

	//запись элементов в память
	lexeme* a = (lexeme*)heap.get_mem(sizeof(lexeme) * (poses->count() + 1));
	for (int h = 0; h < poses->count(); h++)
	{
		a[h] = *(lexeme*)expression->get(*(int*)poses->get(h));
	}
	heap.free_mem(tempsizes);
	*putend = currentpos;
	return a;
}

bool LexemeWorker::ReInnerFind(List * expression, int * currentpos, int currentstep, int * s, int rank, List* poses, Lexeme_list* dob, int type)
{
	if (currentstep == rank - 1)
	{
		lexeme* temp = nullptr;
		bool deviderneeded = false;
		int y = 0;
		for (y = *currentpos; y < *currentpos + (2 * s[rank - 1] - 1); y++)
		{
			temp = (lexeme*)expression->get(y);
			if (deviderneeded)
			{
				if (strcmp(temp->Data(), ",") != 0)
				{
					errorReporter.FReport(logfile, "Ожидается \",\"", temp->Line(), temp->Start_Position());
					return false;
				}
				deviderneeded = false;
				continue;
			}
			else
			{
				if (temp->Type() == VARIABLE || temp->Type() == MASSIVE)
					if (!GetValue(dob, temp, expression, y))
						return false;
				if (temp->Type() != type)
				{
					errorReporter.FReport(logfile, "Тип элемента не соответсвует типу массива!", temp->Line(), temp->Start_Position());
					return false;
				}
				poses->add(&y);
				deviderneeded = true;
			}
		}
		*currentpos = y;
		temp = (lexeme*)expression->get(*currentpos);
		if (strcmp(temp->Data(), "}") != 0)
		{
			errorReporter.FReport(logfile, "Ожидается \"}\"!", temp->Line(), temp->Start_Position());
			return false;
		}
		(*currentpos)++;
		return true;
	}
	int st = s[currentstep];
	while (st > 0)
	{
		st--;
		lexeme* temp = (lexeme*)expression->get(*currentpos);
		if (strcmp(temp->Data(), "{") != 0)
			return false;
		(*currentpos)++;
		if (!ReInnerFind(expression, currentpos, currentstep + 1, s, rank, poses, dob, type))
			return false;
		if (st > 0)
		{
			temp = (lexeme*)expression->get(*currentpos);
			if (strcmp(temp->Data(), ",") != 0)
			{
				errorReporter.FReport(logfile, "Ожидается \",\"", temp->Line(), temp->Start_Position());
				return false;
			}
			(*currentpos)++;
		}
	}
	return true;
}

bool LexemeWorker::GetValue(Lexeme_list * dob, lexeme * place, List* expression, int pos)
{
	int dpos = dob->findpos(place->Data());
	if (dpos != -1)
	{
		lexeme* cond = (lexeme*)dob->get(dpos);
		if (cond->Rank() > 0)
		{
			//...получение элемента из массива и запись в *place
			int quanity = cond->Rank() + cond->Rank() * 2;
			List* index = new List(sizeof(int));
			int i = 0;
			for (i = pos + 1; i < pos + quanity; i++)
			{
				lexeme* temp = (lexeme*)expression->get(i);
				if (temp == nullptr || strcmp(temp->Data(), "[") != 0)
				{
					errorReporter.FReport(logfile, "Ожидается \"[\"!", temp->Line(), temp->Start_Position());
					return false;
				}
				i++;
				temp = (lexeme*)expression->get(i);
				if (temp == nullptr)
				{
					errorReporter.FReport(logfile, "Ожидается индекс типа int!", temp->Line(), temp->Start_Position());
					return false;
				}
				if (temp->Type() == MASSIVE || temp->Type() == VARIABLE)
					if (!GetValue(dob, temp, expression, i))
						return false;
				if (temp->Type() != INT)
				{
					errorReporter.FReport(logfile, "Ожидается индекс типа int!", temp->Line(), temp->Start_Position());
					return false;
				}
				int ti = parser.ToInt(temp->Data());
				index->add(&ti);
				i++;
				temp = (lexeme*)expression->get(i);
				if (temp == nullptr || strcmp(temp->Data(), "]") != 0)
				{
					errorReporter.FReport(logfile, "Ожидается \"]\"!", temp->Line(), temp->Start_Position());
					return false;
				}
			}
			for (int h = i - 1; h > pos; h--)
				expression->remove(h);
			//Получение значения из массива!
			int realindex = 1;
			for (int f = 0; f < index->count(); f++)
			{
				realindex *= *(int*)index->get(f);
				for (int kf = f + 1; kf < index->count(); kf++)
					realindex *= *(int*)index->get(kf);
			}
			*place = cond->Values[realindex];
		}
		else *place = *cond;
		return true;
	}
	else
	{
		errorReporter.FReport(logfile, "Нельзя непоределенное присвоить значение!", place->Line(), place->Start_Position());
		return false;
	}
}