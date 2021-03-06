#include "stdafx.h"
#include "LexemeWorker.h"

bool LexemeWorker::Processing(List* lexes)
{
	//���������
	Lexeme_list* dob = new Lexeme_list(); //���� ��� ������� ���������
	for (int i = 0; i < lexes->count(); i++)
	{
		lexeme* temp_lexeme = (lexeme*)lexes->get(i);
		if (strcmp(temp_lexeme->Data(), "{") == 0)
		{
			dob->add(temp_lexeme);
			continue;
		}
		if (strcmp(temp_lexeme->Data(), "[") == 0) //��������� ��������, �� ���������� � ���-�������
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
						errorReporter.FReport(logfile, "��������� \"]\"!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					if (strcmp(tempdevider->Data(), "[") == 0)
						open = true;
					if (k >= lexes->count())
					{
						errorReporter.FReport(logfile, "������������� ���������!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					tempdevider = (lexeme*)lexes->get(i + k);
					if (tempdevider->Type() == VARIABLE)
						if (!GetVariableValue(dob, tempdevider))
						{
							errorReporter.FReport(logfile, "��������� ��������� ��� ���������� ���� \"int\"!", tempdevider->Line(), tempdevider->Start_Position());
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
						errorReporter.FReport(logfile, "��������� �������� ���� int!", tempdevider->Line(), tempdevider->Start_Position());
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
						errorReporter.FReport(logfile, "��� ���������� ��� ������������!", tempdevider->Line(), tempdevider->Start_Position());
						return false;
					}
					break;
				}
				errorReporter.FReport(logfile, "��������� \"[\" ��� \"]\"!", tempdevider->Line(), tempdevider->Start_Position());
				return false;
			}
			if (k % 3 != 0)
			{
				errorReporter.FReport(logfile, "� ����� ��������� \"]\"!", tempdevider->Line(), tempdevider->Start_Position());
				return false;
			}
			int rank = k / 3;
			k++;
			tempdevider = (lexeme*)lexes->get(i + k);
			if (tempdevider == nullptr || strcmp(tempdevider->Data(), "=") != 0)
			{
				errorReporter.FReport(logfile, "��������� \"=\"!", tempdevider->Line(), tempdevider->Start_Position());
				return false;
			}
			k++;
			int posend = k + i;
			//...�-��� ������� ��������� � ������ �� � ������, ��������� ������ �� ������ �������, � posend ������� ������� ";" (����� ���������).
			List* tempvalues = GetMassValues(lexes, k + i, ttype->Data(), &posend, dob, rank, sizes);
			if (posend > k + 1)
			{
				lexeme* newlex = (lexeme*)lexes->get(namepos);
				newlex->ToMass(name, MASSIVE, tempvalues, rank, sizes);
				//� ��� ��� ������� ���������
				dob->add(newlex);
				//dob->print_list();
				//���������� � ���-�������
				LexemeTable.auto_create(newlex);
				//...������������ ������� ������� �� ������� namepos
				//�������� ������ ������
				for (int g = posend; g >= i - 1; g--)
					lexes->remove(g);
				//�������� �� ������ ������� � �����
				i-=2;
			}
			else
				return false;
			continue;
		}
		if (temp_lexeme->Type() == VARIABLE) //��������� ����������, �� ���������� � ���-������� 
		{
			if (dob->find(temp_lexeme->Data()) == nullptr) //���� ��� � ����� ��� ������� ���������
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
						if (tdevider == nullptr || strcmp(tdevider->Data(), "=") != 0)
						{
							errorReporter.FReport(logfile, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + (int)strlen(temp_lexeme->Data()));
							return false;
						}
						lexeme* tdata = (lexeme*)lexes->get(i + 2);
						if (!(tdata->Type() == INT || tdata->Type() == DOUBLE || tdata->Type() == FLOAT || tdata->Type() == CHAR || tdata->Type() == STRING || tdata->Type() == BOOL) || tdata == nullptr)
						{
							if (tdata->Type() == VARIABLE)
							{
								if (!GetVariableValue(dob, tdata))
									return false;
							}
							else
							{
								errorReporter.FReport(logfile, "��������� ���������� ��� ��������� ���� INT!", tdevider->Line(), tdevider->Start_Position());
								return false;
							}
						}
						lexeme* tdevider2 = (lexeme*)lexes->get(i + 3);
						if (tdevider2 != nullptr && strcmp(tdevider2->Data(), ";") != 0)
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
						//� ��� ��� ������� ���������
						dob->add(newlex);
						//���������� � ���-�������
						LexemeTable.auto_create(newlex);
						//�������� �������� � ������ � �������� :
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
						//�������� �� ������
						//dob->print_list();
						int pos = dob->findpos(temp_lexeme->Data());
						lexeme* ttttttt = (lexeme*)dob->get(pos);
						if (pos != -1)
							lexes->change(i, ttttttt);
						lexeme* sty = (lexeme*)lexes->get(i);
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
			//��������� ��� �����
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
			if (_error) return false;
		}
	}

	//�������
	printf_s("\n|---------------|\nVariable Table:\n");
	LexemeTable.print_lexems();
	printf_s("\n|---------------|\n");
	printf_s("Tree View:\n");
	storage->print();

	for (int h = 0; h < expression->count(); h++)
	{
		printf("%d. ", h);
		(*(lexeme**)expression->get(h))->Print();
	}

	storage->exec();
	//
	if(dob != nullptr)
		dob->~Lexeme_list();
	if(storage != nullptr)
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
	int start = pos;
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
		TNode* tn = treeWorker.GetTNode(tlist, 0, tlist->count() - 2);
		if (tn == nullptr)
		{
			tlist->~List();
			_error = true;
			return pos;
		}	
		storage->addNode(tn);
		tlist->~List();
		return i;
	}

	tlist->~List();
	_error = true;
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
			if (param1 == nullptr)
				return i;
		}
		else
		{
			lexeme* t = *(lexeme**) expression->get(i);
			shrtiflist->~List();
			ErrorReporter().FReport(logfile, "���������� ���������� ������� \"?\"!", t->Line(), t->Start_Position());
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
			if (param1 == nullptr)
				return i;
		}
		else
		{
			lexeme* t = *(lexeme**)expression->get(i);
			ErrorReporter().FReport(logfile, "���������� ���������� ������� \"?\"!", t->Line(), t->Start_Position());
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
		ErrorReporter().FReport(logfile, "��������� \":\" ��� \"?\"!", t->Line(), t->Start_Position());
		return i;
	}
	if (IsBoolExpression(body1, true) || IsNumberExpressionWithBoolOperations(body1) || IsStringExpressionWithBoolOperations(body1) || IsNumberExpression(body1, true) || IsStringExpression(body1, true))
	{
		param2 = treeWorker.GetTNode(body1, 0, body1->count() - 1);
		body1->~List();
		if (param2 == nullptr)
			return i;
	}
	else
	{
		ErrorReporter().FReport(logfile, "��������� ����� \"?\" �� �������!", (*(lexeme**)expression->get(i))->Line(), i);
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
		ErrorReporter().FReport(logfile, "������������� ���������� ����� \"?\"!", t->Line(), t->Start_Position());
		return i;
	}
	if (IsBoolExpression(body2, true) || IsNumberExpressionWithBoolOperations(body2) || IsStringExpressionWithBoolOperations(body2) || IsNumberExpression(body2, true) || IsStringExpression(body2, true))
	{
		param3 = treeWorker.GetTNode(body2, 0, body2->count() - 1);
		if (param3 == nullptr)
		{
			body2->~List();
			return i;
		}
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
		errorReporter.FReport(logfile, "�� ����������� ���������!", spec->Line(), spec->Start_Position());
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
	if (strcmp(spec->Data(), "output ") == 0)
	{
		List* param = new List(sizeof(lexeme*));
		int	res = FuncWithStringParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			TNode* t = treeWorker.GetTNode(expression, pos + 2, res - 1);
			if (t == nullptr)
				return pos;
			TOutPut* ot = new TOutPut(t);
			storage->addNode((TNode*)ot);
			param->~List();
			return res;
		}
		res = FuncWithNumberParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			TNode* t = treeWorker.GetTNode(expression, pos + 2, res - 1);
			if (t == nullptr)
				return pos;
			TOutPut* ot = new TOutPut(t);
			storage->addNode((TNode*)ot);
			param->~List();
			return res;
		}
		res = FuncWithBoolParam(expression, pos, spec, true, param);
		if (res > pos)
		{
			TNode* t = treeWorker.GetTNode(expression, pos + 2, res - 1);
			if (t == nullptr)
				return pos;
			TOutPut* ot = new TOutPut(t);
			storage->addNode((TNode*)ot);
			param->~List();
			return res;
		}
		return pos;
	}
	if (strcmp(spec->Data(), "input ") == 0)
	{
		lexeme* tdevider = *(lexeme**)expression->get(pos + 1);
		if (tdevider == nullptr || strcmp(tdevider->Data(), "(") != 0)
		{
			errorReporter.FReport(logfile, "��������� \"(\"!", tdevider->Line(), tdevider->Start_Position());
			return pos;
		}
		tdevider = *(lexeme**)expression->get(pos + 2);
		if (tdevider == nullptr || strcmp(tdevider->Data(), ")") != 0)
		{
			errorReporter.FReport(logfile, "��������� \")\"!", tdevider->Line(), tdevider->Start_Position());
			return pos;
		}
		TInPut* in = new TInPut(spec);
		storage->addNode((TNode*)in);
		return pos + 2;
	}
	if (strcmp(spec->Data(), "min ") == 0 || strcmp(spec->Data(), "max ") == 0)
	{
		List* param1 = new List(sizeof(lexeme*));
		List* param2 = new List(sizeof(lexeme*));
		int res = FuncWithTwoNumberParams(expression, pos, spec, true, param1, param2);
		if (res != pos)
		{
			bool max = (strcmp(spec->Data(), "max ") == 0);
			TNode* tparam1 = treeWorker.GetTNode(param1, 0, param1->count() - 1);
			if (tparam1 == nullptr)
			{
				param1->~List();
				param2->~List();
				return pos;
			}
			TNode* tparam2 = treeWorker.GetTNode(param2, 0, param2->count() - 1);
			if (tparam2 == nullptr)
			{
				param1->~List();
				param2->~List();
				return pos;
			}
			TMinMax* tmm = new TMinMax(spec, tparam1, tparam2, max);
			storage->addNode((TNode*)tmm);
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
			bool sin = (strcmp(spec->Data(), "sin ") == 0);
			TNode* tparam = treeWorker.GetTNode(param, 0, param->count() - 1);
			if (tparam == nullptr)
			{
				param->~List();
				return pos;
			}
			TSinCos* tsc = new TSinCos(spec, tparam, sin);
			storage->addNode((TNode*)tsc);
			param->~List();
			return res;
		}
		param->~List();
		return pos;
	}
	ErrorReporter().FReport(logfile, "����������� ��� ����������� �������������� ����.�����!", spec->Line(), spec->Start_Position());
	return pos;
}

int LexemeWorker::FuncWithTwoNumberParams(List * expression, int pos, lexeme * spec, bool finaldevider, List* param1, List* param2)
{
	lexeme* devider1 = *(lexeme**)expression->get(pos + 1);
	if (strcmp(devider1->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "��������� \"(\"", devider1->Line(), devider1->Start_Position());
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
		errorReporter.FReport(logfile, "��������� �������� ���������!", devider1->Line(), devider1->Start_Position());
		param1->~List();
		param2->~List();
		return pos;
	}

	if (strcmp(tlex->Data(), ",") != 0)
	{
		errorReporter.FReport(logfile, "��������� \",\"", devider2->Line(), devider2->Start_Position());
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
		errorReporter.FReport(logfile, "��������� �������� ���������!", devider1->Line(), devider1->Start_Position());
		param1->~List();
		param2->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		ErrorReporter().FReport(logfile, "��������� \")\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \";\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \"(\"", devider1->Line(), devider1->Start_Position());
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
		errorReporter.FReport(logfile, "��������� ��������� ���������!", devider1->Line(), devider1->Start_Position());
		param->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "��������� \")\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \";\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \"(\"", devider1->Line(), devider1->Start_Position());
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
		errorReporter.FReport(logfile, "��������� ��������� ���� bool!", devider1->Line(), devider1->Start_Position());
		param->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "��������� \")\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \";\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \"(\"", devider1->Line(), devider1->Start_Position());
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
		errorReporter.FReport(logfile, "��������� �������� ���������!", devider1->Line(), devider1->Start_Position());
		param->~List();
		return pos;
	}
	if (strcmp(tlex->Data(), ")") != 0)
	{
		errorReporter.FReport(logfile, "��������� \")\"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \";\"", tlex->Line(), tlex->Start_Position());
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
				ErrorReporter().FReport(logfile, "��������� \";\"", tlex->Line(), tlex->Start_Position());
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
				TNode* tn = treeWorker.GetTNode(tlist, 0, tlist->count() - 2);
				if (tn == nullptr)
				{
					tlist->~List();
					return false;
				}
				storage->addNode(tn);
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
					errorReporter.FReport(logfile, "�� ������ ������� ������ \"(\"!", tlex->Line(), tlex->Start_Position());
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
						errorReporter.FReport(logfile, "� ��������� �� ����� ���� ��������� ������������!", tlex->Line(), tlex->Start_Position());
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
				errorReporter.FReport(logfile, "��������� ��������!", tlex->Line(), tlex->Start_Position());
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
			if (!correct && tlex->Type() == MASSIVE)
			{
				for (int f = 0; f < c2; f++)
				{
					if ((*(lexeme**)tlex->Values->get(0))->Type() == types[f])
					{
						correct = true;
						varbefore = true;
						break;
					}
				}
				if (correct)
					GetMassElemIndexes(expression, tlex, &k);
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
		sprintf_s(str, "�� ������ ������� %d ������(��)!", hook_count);
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
		ErrorReporter().FReport(logfile, "������������� ���������!", spec->Line(), spec->Start_Position() + (int)strlen(spec->Data()));
		_error = true;
		return pos;
	}
	TNode* h = treeWorker.GetTNode(hl, 0, hl->count() - 1);
	if (h == nullptr)
	{
		_error = true;
		return pos;
	}
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
		errorReporter.FReport(logfile, "������������� ���������!", spec->Line(), spec->Start_Position());
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
						errorReporter.FReport(logfile, "��������� \"while \" ����� ���� ����� \"do \"!", tlex->Line(), tlex->Start_Position());
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
					if (h == nullptr)
					{
						_error = true;
						return origpos;
					}
					storage->addNode((TNode*)new TDoWhile(body, h));
					return pos2;
				}
				else
				{
					ErrorReporter().FReport(logfile, "������������� ���������!", spec->Line(), spec->Start_Position());
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
		errorReporter.FReport(logfile, "��������� ���� ����� \"do \"!", spec->Line(), spec->Start_Position());
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
		errorReporter.FReport(logfile, "��������� ��������� ��� \"for \"!", spec->Line(), spec->Start_Position());
		_error = true;
		return origpos;
	}
	lexeme* tlex = *(lexeme**)expression->get(pos);
	if (strcmp(tlex->Data(), "(") != 0)
	{
		errorReporter.FReport(logfile, "��������� \"(\"!", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "������������� ��������� \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		return origpos;
	}
	if (param1->count() != 1)
	{
		errorReporter.FReport(logfile, "��������� ������������������ ���������� � �������� ������� ���������!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		return origpos;
	}
	lexeme* first = *(lexeme**)param1->get(0);
	if (!(first->Type() == INT || first->Type() == DOUBLE || first->Type() == FLOAT))
	{
		errorReporter.WarningReport(logfile, "�������� �������� ��� ������� ��������� \"for \"", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "������������� ��������� \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		return origpos;
	}
	if (!IsNumberExpressionWithBoolOperations(param2) && !IsBoolExpression(param2, false) && !IsNumberExpressionWithBoolOperations(param2))
	{
		errorReporter.FReport(logfile, "�������� ������ �������� \"for \"!", tlex->Line(), tlex->Start_Position());
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
		errorReporter.FReport(logfile, "��������� \")\"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		param3->~List();
		return origpos;
	}
	if (pos >= expression->count())
	{
		errorReporter.FReport(logfile, "������������� ��������� \"for \"!", tlex->Line(), tlex->Start_Position());
		_error = true;
		param1->~List();
		param2->~List();
		param3->~List();
		return origpos;
	}
	if (!IsNumberExpression(param3, true))
	{
		errorReporter.FReport(logfile, "�������� ������ �������� \"for \"!", tlex->Line(), tlex->Start_Position());
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
			if (tparam2 == nullptr || tparam3 == nullptr)
			{
				param3->~List();
				param2->~List();
				param1->~List();
				_error = true;
				body->~List();
				return origpos;
			}
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
		errorReporter.FReport(logfile, "��������� ���� \"for \"!", tlex->Line(), tlex->Start_Position());
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
	if (h == nullptr)
	{
		_error = true;
		return origpos;
	}
	
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
		errorReporter.FReport(logfile, "������������� ���������!", spec->Line(), spec->Start_Position());
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
		if (InnerExpression(tlist2, body2))
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
		errorReporter.FReport(logfile, "������������� ���������!", spec->Line(), spec->Start_Position());
		_error = true;
		tlist2->~List();
		return origpos;
	}
	return pos;
}

List * LexemeWorker::GetMassValues(List* expression, int start, char* ttype, int* putend, Lexeme_list* dob, int rank, List* sizes)
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
		errorReporter.FReport(logfile, "��������� \"{\"", temp->Line(), temp->Start_Position());
		heap.free_mem(tempsizes);
		return nullptr;
	}
	currentpos++;
	//����� � ������� ���������
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
			errorReporter.FReport(logfile, "��������� \"}\"", temp->Line(), temp->Start_Position());
			heap.free_mem(tempsizes);
			return nullptr;
		}
		currentpos++;
	}
	temp = (lexeme*)expression->get(currentpos);
	if (temp == nullptr || strcmp(temp->Data(), ";") != 0)
	{
		errorReporter.FReport(logfile, "��������� \";\"", temp->Line(), temp->Start_Position());
		heap.free_mem(tempsizes);
		return nullptr;
	}

	//������ ��������� � ������
	List* a = new List(sizeof(lexeme*));
	for (int h = 0; h < poses->count(); h++)
	{
		lexeme* ty = (lexeme*)expression->get(*(int*)poses->get(h));
		a->add(&ty);
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
					errorReporter.FReport(logfile, "��������� \",\"", temp->Line(), temp->Start_Position());
					return false;
				}
				deviderneeded = false;
				continue;
			}
			else
			{
				if (temp->Type() == VARIABLE || temp->Type() == MASSIVE)
					if (!GetVariableValue(dob, temp))
						return false;
				if (temp->Type() != type)
				{
					errorReporter.FReport(logfile, "��� �������� �� ������������ ���� �������!", temp->Line(), temp->Start_Position());
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
			errorReporter.FReport(logfile, "��������� \"}\"!", temp->Line(), temp->Start_Position());
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
				errorReporter.FReport(logfile, "��������� \",\"", temp->Line(), temp->Start_Position());
				return false;
			}
			(*currentpos)++;
		}
	}
	return true;
}

bool LexemeWorker::GetVariableValue(Lexeme_list * dob, lexeme * place)
{
	int dpos = dob->findpos(place->Data());
	if (dpos != -1)
	{
		lexeme* cond = (lexeme*)dob->get(dpos);
		*place = *cond;
		return true;
	}
	else
	{
		errorReporter.FReport(logfile, "������ �������������� ��������� ��������!", place->Line(), place->Start_Position());
		return false;
	}
}

TList * LexemeWorker::GetMassElemIndexes(List * expression, lexeme * mass, int * origpos)
{
	//���������� origpos �� ��������� ������
	if (mass->Rank() > 0)
	{
		//...��������� �������� �� ������� (� ������ � *place?)
		int quanity = mass->Rank() + mass->Rank() * 2;
		TList* indexes = new TList();
		int pos = *origpos;
		int indexcount = 0;
		int i = 0;
		for (i = pos + 1; i < expression->count(); i++)
		{
			lexeme* temp = *(lexeme**)expression->get(i);
			if (temp == nullptr || strcmp(temp->Data(), "[") != 0)
			{
				break;
				//errorReporter.FReport(logfile, "��������� \"[\"!", temp->Line(), temp->Start_Position());
				//return false;
			}
			List* templ = new List(sizeof(lexeme*));
			int start = i + 1;
			while (true)
			{
				i++;
				temp = *(lexeme**)expression->get(i);
				if (temp == nullptr)
				{
					errorReporter.FReport(logfile, "��������� ������ ���� int!", temp->Line(), temp->Start_Position());
					return nullptr;
				}
				if (strcmp(temp->Data(), "]") == 0)
				{
					*origpos = i;
					break;
				}
				templ->add(&temp);
			}
			if (!LexemeWorker().IsNumberExpression(templ, false))
				return nullptr;
			TNode* tl = treeWorker.GetTNode(expression, start, i - 1);
			if (tl == nullptr)
				return nullptr;
			indexes->addNode(tl);
			indexcount++;
		}
		//��������
		if (indexcount != mass->Sizes->count())
		{
			lexeme* temp = *(lexeme**)expression->get(pos);
			errorReporter.FReport(logfile, "���-�� �������� �� ������!", temp->Line(), temp->Start_Position());
			return false;
		}
		return indexes;
	}
	return nullptr;
}

/*
lexeme* LexemeWorker::GetMassElem(lexeme* mass, List* indexes)
{
	if (mass->Rank() > 0)
	{
		//��� ��������!
		if (indexes->count() != mass->Sizes->count())
		{
			errorReporter.FReport(logfile, "���������� �������� �� ��������� � ������������ �������!", mass->Line(), mass->Start_Position());
			return nullptr;
		}
		int realindex = 1;
		for (int p = 0; p < indexes->count(); p++)
		{
			int s = *(int*)mass->Sizes->get(p);
			int index = *(int*)indexes->get(p);
			realindex *= index;
			if (index >= s)
			{
				char str[100];
				sprintf_s(str, "����� �� ������� ���������! ������ \"%d\" >= \"%d\"", index, s);
				errorReporter.FReport(logfile, str, mass->Line(), mass->Start_Position());
				return nullptr;
			}
			for (int g = p + 1; g < indexes->count(); g++)
				realindex *= *(int*)indexes->get(g);
		}
		return &mass->Values[realindex];
	}
	return nullptr;
}
*/