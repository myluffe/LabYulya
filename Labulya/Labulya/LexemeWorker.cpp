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
					if (i + 1 != lexes->count() && i + 2 != lexes->count())
					{
						lexeme* tdevider = (lexeme*)lexes->get(i + 1);
						if (strcmp(tdevider->Data(), "=") != 0)
						{
							ErrorReporter().FReport(stdout, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + strlen(temp_lexeme->Data()));
							return false;
						}
						lexeme* tdata = (lexeme*)lexes->get(i + 2);
						if (strcmp(tdata->Name(), "Number") != 0 && strcmp(tdata->Name(), "String") != 0 && strcmp(tdata->Name(), "Char") != 0)
						{
							ErrorReporter().FReport(stdout, "There should be value!", tdevider->Line(), tdevider->Start_Position());
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
						lexes->remove(i + 2);
						lexes->remove(i + 1);
						//lexes[i] = newlex;
						lexes->set(i, newlex);
						lexes->remove(i - 1);
						i = ti;
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
	printf_s("\n|---------------|\nVariable Table:");
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
