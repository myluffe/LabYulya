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
			if (dob->find(temp_lexeme->Data()) == nullptr)
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
							ErrorReporter().FReport(stdout, "You should define the variable!", temp_lexeme->Line(), temp_lexeme->Start_Position() + strlen(temp_lexeme->Data()) - 1);
							return false;
						}
						lexeme* tdata = (lexeme*)lexes->get(i + 2);
						if (strcmp(temp_lexeme->Name(), "Number") != 0)
						{
							ErrorReporter().FReport(stdout, "There should be value!", tdevider->Line(), tdevider->Start_Position() + 1);
							return false;
						}
						lexeme* newlex = new lexeme(temp_lexeme->Data(), ttype->Type(), tdata->Data(), temp_lexeme->Line(), temp_lexeme->Start_Position(), temp_lexeme->Priority());
						dob->add(newlex);
					}
				}
				else
				{
					ErrorReporter().FReport(stdout, "You miss the type!", temp_lexeme->Line(), temp_lexeme->Start_Position());
					return false;
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
	return true;
}

LexemeWorker::LexemeWorker()
{
}


LexemeWorker::~LexemeWorker()
{
}
