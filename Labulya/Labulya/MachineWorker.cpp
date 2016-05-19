#include "stdafx.h"
#include "MachineWorker.h"

MachineWorker::MachineWorker()
{
	_count = 0;

	//First Machine
	// add class, struct
	sm = new Type1Machine("Special Words", "Special Word", "Special Word");
	char* ss[] = { "//", "/*", "*/", "return ", "for ", "if ", "else ", "while ", "do ", "input ", "output ", "max ", "min " };
	for each (char* var in ss)
	{
		sm->AddWord(var);
	}
	_addmachine(sm);

	//Sixth Machine
	tm = new Type1Machine("Types", "Type", "Type");
	char* ts[] = { "class ", "struct ", "int ", "float ", "double ", "char ", "bool " };
	for each (char* var in ts)
	{
		tm->AddWord(var);
	}
	_addmachine(tm);

	//Second Machine
	om = new Type1Machine("Operations", "Operation", "Operation");
	char* os[] = { "+", "-", "=", "/", "*", "++", "--", "==", "||", "|", "&&", "&", ">", ">=", "<=", "%" };
	for each (char* var in os)
	{
		om->AddWord(var);
	}
	_addmachine(om);

	//Third Machine
	nm = new Type2Machine("Numbers", "Number", "float");
	char nm_perstart[] = "0123456789.";
	char nm_optional[] = "e";
	nm->AddWord(nm_perstart);
	nm->AddWord(nm_optional);
	nm->SetPerStartWords(nm_perstart);
	_addmachine(nm);

	//Fourth Machine
	vm = new Type2Machine("Variables", "Variable", "Variable");
	char vm_perstart[] = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	char vm_optional[] = "1234567890";
	vm->AddWord(vm_perstart);
	vm->AddWord(vm_optional);
	vm->SetPerStartWords(vm_perstart);
	_addmachine(vm);

	//Fifth Machine
	dm = new Type1Machine("Deviders", "Devider", "Devider");
	char* ds[] = { ";", "(", ")", "{", "}", "[", "]" };
	for each (char* var in ds)
	{
		dm->AddWord(var);
	}
	_addmachine(dm);

	
	for (int klick = 0; klick < Count(); klick++)
	{
		_machines[klick]->PrintMachine();
		printf("\n-------------\n");
	}
	printf("\n-------------\n");
	
}


MachineWorker::~MachineWorker()
{
}

int MachineWorker::Work(char* filename)
{
	mStateMachine* currentaut;
	currentaut = _machines[0];
	int machinecount = 0;
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
			if (!EnterInComment && str[s] != ' ' && str[s] != '	')
			{
				currentaut->EnterChar(str[s], s);
				if (currentaut->IsFinished())
				{
					if (strcmp(currentaut->MachineName(), "Special Words") == 0)
					{
						if (strcmp(currentaut->Buffer(), "//") == 0)
						{
							strcpy_s(str, fr.ReadNextLine());
							s = -1;
							continue;
						}
						if (strcmp(currentaut->Buffer(), "/*") == 0)
						{
							EnterInComment = true;
							commentline = fr.CurrentLine();
							commentpos = s - 2;
							continue;
						}
					}
					if (strcmp(currentaut->MachineName(), "Numbers") == 0)
					{
						NumberCheck(currentaut, fr.CurrentLine());
						if (_error)
							return -2;
					}
					lexeme().Addlexeme(currentaut->CurrentLexName(),
						currentaut->CurrentLexType(), currentaut->Buffer(),
						fr.CurrentLine(), currentaut->CurrentLexPos(), currentaut->Priority);
					currentaut->UpdateStatus();
					currentaut = _machines[0];
					s--;
					machinecount = 0;
				}
				if (currentaut->IsError())
				{
					if (machinecount >= _count - 1)
					{
						if (str[s] == ' ' || str[s] == '	')
						{
							currentaut->UpdateStatus();
							currentaut = _machines[0];
							machinecount = 0;
							continue;
						}	
						else
						{
							currentaut->UpdateStatus();
							perror("Global Error! NO right machine!");
							return -100;
						}
					}
					else
					{
						s = currentaut->CurrentLexPos() - 1; 
						currentaut->UpdateStatus();
						machinecount++;
						currentaut = _machines[machinecount];
					}
				}
			}
			else
			{
				if (fr.EndFile())
				{
					Error.FReport(stdout, "Не закрытый комментарий", 0, 0);
					_error = false;
					return -1;
				}
				if (str[s] == '*' && str[s + 1] == '/')
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

void MachineWorker::_addmachine(mStateMachine * machine)
{
	_machines[_count++] = machine;
}

int MachineWorker::NumberCheck(mStateMachine* curr, int line)
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
				Error.FReport(stdout, "Некорректное число. \"e\" - не может быть в числителе.", line, curr->CurrentLexPos() + i);
				_error = true;
				return -1;
			}
			first[i] = number[i];
			i++;
		}
		else break;
	}
	i++;
	first[i] = 0;

	int j = 0;
	int e_count = 0;
	while (i < (int)(strlen(number)))
	{
		if (number[i] == '.')
		{
			Error.FReport(stdout, "Некорректное число. \".\" - не может быть несколько.", line, curr->CurrentLexPos() + i);
			_error = true;
			return -2;
		}
		if (number[i] == 'e')
		{
			if (e_count == 0)
			{
				second[j++] = number[i++];
			}
			else
			{
				Error.FReport(stdout, "Некорректное число. \"e\" - не может быть несколько.", line, curr->CurrentLexPos() + i);
				_error = true;
				return -3;
			}
		}
		second[j++] = number[i++];
	}
	second[j] = 0;
	// a = 123
	// b = e1
	if (strlen(second) == 0)
	{
		if (strlen(first) > 0)
			curr->ChangeType("double");
		else
			curr->ChangeType("int");
	}
	return 1;
}
