#include "stdafx.h"
#include "MachineWorker.h"

MachineWorker::MachineWorker()
{
	_count = 0;

	// Special Words
	sm = new Type1Machine("Special Words", "Special Word", "Special Word");
	char* ss[] = { "//", "/*", "*/", "return ", "for ", "if ", "else ", "while ", "do ", "input ", "output ", "max ", "min " };
	for each (char* var in ss)
	{
		sm->AddWord(var);
	}
	_addmachine(sm);

	// Types
	tm = new Type1Machine("Types", "Type", "Type");
	char* ts[] = { "class ", "struct ", "int ", "float ", "double ", "char ", "bool " };
	for each (char* var in ts)
	{
		tm->AddWord(var);
	}
	_addmachine(tm);

	// Operations1
	om = new Type1Machine("Operations1", "Operation", "Operation");
	char* os[] = { "++", "--", "==", "||", "&&", ">=", "<=" };
	for each (char* var in os)
	{
		om->AddWord(var);
	}
	_addmachine(om);

	// Operations2
	om2 = new Type1Machine("Operations2", "Operation", "Operation");
	char* os2[] = { "+", "-", "=", "/", "*", "|", "&", ">", "<", "%" };
	for each (char* var in os2)
	{
		om2->AddWord(var);
	}
	_addmachine(om2);

	// Numbers
	nm = new Type2Machine("Numbers", "Number", "float");
	char nm_perstart[] = "0123456789.";
	char nm_optional[] = "e";
	nm->AddWord(nm_perstart);
	nm->AddWord(nm_optional);
	nm->SetPerStartWords(nm_perstart);
	_addmachine(nm);

	// Variables
	vm = new Type2Machine("Variables", "Variable", "Variable");
	char vm_perstart[] = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	char vm_optional[] = "1234567890";
	vm->AddWord(vm_perstart);
	vm->AddWord(vm_optional);
	vm->SetPerStartWords(vm_perstart);
	_addmachine(vm);

	// Deviders
	dm = new Type1Machine("Deviders", "Devider", "Devider");
	char* ds[] = { ";", "(", ")", "{", "}", "[", "]"};
	for each (char* var in ds)
	{
		dm->AddWord(var);
	}
	_addmachine(dm);

	// String & Symbols
	ssm = new Type3Machine("String & Symbols", "String", "char*");
	char ssm_perstart[] = "\"\'";
	char ssm_optional[] = "\\|:,<.>/?;\*]}[{-=!@#$%^&*()+`~";
	ssm->AddWord(ssm_perstart);
	ssm->AddWord(ssm_optional);
	ssm->AddWord(vm_perstart);
	ssm->AddWord(vm_optional);
	ssm->SetPerStartWords(ssm_perstart);
	_addmachine(ssm);
	
	for (int klick = 0; klick < _count; klick++)
	{
		_machines[klick]->PrintMachine();
		printf("\n-------------\n");
	}
	
	_currentaut = _machines[0];
	_curmachine = 0;
}


MachineWorker::~MachineWorker()
{
}

int MachineWorker::Work(char* filename, List* lexes)
{
	mFileReader fr = mFileReader(filename);
	char str[100];

	bool EnterInComment = false;
	int commentline = 0;
	int commentpos = 0;

	while (!fr.EndFile())
	{
		strcpy_s(str, fr.ReadNextLine());
		int lenght = (int)strlen(str);
		for (int s = 0; s < lenght; s++)
		{
			//if (!EnterInComment)
			if (!EnterInComment && (_currentaut->IsStart() || (str[s] != ' ' && str[s] != '\t')))
			{
				_currentaut->EnterChar(str[s], s, fr.CurrentLine());
				if (_currentaut->IsFinished())
				{
					if (_currentaut->CheckError())
						return -3;
					if (strcmp(_currentaut->MachineName(), "Special Words") == 0)
					{
						if (strcmp(_currentaut->Buffer(), "//") == 0)
						{
							strcpy_s(str, fr.ReadNextLine());
							lenght = (int)strlen(str);
							s = -1;
							UpdateMachines();
							continue;
						}
						if (strcmp(_currentaut->Buffer(), "/*") == 0)
						{
							EnterInComment = true;
							commentline = _currentaut->CurrentLexLine();
							commentpos = _currentaut->CurrentLexPos();
							UpdateMachines();
							continue;
						}
					}
					if (strcmp(_currentaut->MachineName(), "Numbers") == 0)
					{
						NumberCheck(_currentaut, fr.CurrentLine());
						if (_error)
							return -2;
					}
					lexes->add(new lexeme(_currentaut->CurrentLexName(),
						_currentaut->CurrentLexType(), _currentaut->Buffer(),
						_currentaut->CurrentLexLine(), _currentaut->CurrentLexPos(), _currentaut->Priority)->Print);
					UpdateMachines();
					s--;		
				}
				if (_currentaut->IsError())
				{
					if (_curmachine >= _count - 1)
					{
						if (str[s] == ' ' || str[s] == '\t')
						{
							UpdateMachines();
							continue;
						}	
						else
						{
							if (str[s] != EOF)
								Error.FReport(stdout, "Unprocessed word!", fr.CurrentLine(), _currentaut->CurrentLexPos());
							UpdateMachines();
							return -100;
						}
					}
					else
					{
						s = _currentaut->CurrentLexPos() - 1; 
						_currentaut->UpdateStatus();
						_curmachine++;
						_currentaut = _machines[_curmachine];
					}
				}
			}
			else
			{
				if (fr.EndFile() && s >= strlen(str) - 1)
				{
					Error.FReport(stdout, "Maybe you forget to close the comment", commentline, commentpos);
					_error = false;
					return -1;
				}
				if (str[s] == '/' && str[s - 1] == '*')
				{
					EnterInComment = false;
					commentline = 0;
					commentpos = 0;
					continue;
				}
			}
		}
	}
	return 1;
}

int MachineWorker::Count()
{
	return _count;
}

void MachineWorker::UpdateMachines()
{
	_currentaut->UpdateStatus();
	_currentaut = _machines[0];
	_curmachine = 0;
}

void MachineWorker::_addmachine(mStateMachine * machine)
{
	_machines[_count++] = machine;
}

void MachineWorker::NumberCheck(mStateMachine* curr, int line)
{
	char first[30];
	first[0] = 0;

	char second[30];
	second[0] = 0;

	char number[60];
	strcpy_s(number, curr->Buffer());

	int count = strlen(number);
	int i = 0;
	while (i < (int)(strlen(number)))
	{
		if (number[i] != '.')
		{
			if (number[i] == 'e')
			{
				Error.FReport(stdout, "Uncorrect number. \"e\" - не может быть в числителе.", line, curr->CurrentLexPos() + i);
				_error = true;
				return;
			}
			first[i] = number[i];
			i++;
		}
		else break;
	}

	first[i] = '\0';

	int j = 0;
	int e_count = 0;
	i++;

	while (i < (int)(strlen(number)))
	{
		if (number[i] == '.')
		{
			Error.FReport(stdout, "Uncorrect number. \".\" - не может быть несколько.", line, curr->CurrentLexPos() + i);
			_error = true;
			return;
		}
		if (number[i] == 'e')
		{
			if (e_count == 0)
			{
				second[j++] = number[i++];
			}
			else
			{
				Error.FReport(stdout, "Uncorrect number. \"e\" - не может быть несколько.", line, curr->CurrentLexPos() + i);
				_error = true;
				return;
			}
		}
		else second[j++] = number[i++];
	}
	second[j] = '\0';
	// a = 123
	// b = e1
	if (strlen(second) == 0)
	{
		if (strlen(first) > 0)
			curr->ChangeType("double");
		else
			curr->ChangeType("int");
	}
	return;
}
