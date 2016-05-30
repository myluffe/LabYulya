#include "stdafx.h"
#include "MachineWorker.h"
#include "LexemeWorker.h"

MachineWorker::MachineWorker()
{
	_count = 0;

	// Special Words
	sm = new Type1Machine("Special Words", "Special Word", "Special Word");
	char* ss[] = { "//", "/*", "*/", "return ", "for ", "if ", "else ", "while ", "do ", "input ", "output ", "max ", "min ", "sin ", "cos " };
	for each (char* var in ss)
	{
		sm->AddWord(var);
	}
	Addmachine(sm);

	// Types
	tm = new Type1Machine("Types", "Type", "Type");
	char* ts[] = { "class ", "struct ", "int ", "float ", "double ", "char ", "bool ", "string " };
	for each (char* var in ts)
	{
		tm->AddWord(var);
	}
	Addmachine(tm);

	//Bool
	bm = new Type1Machine("Bools", "Bool", "bool ");
	char* bs[] = { "true ", "false " };
	for each (char* var in bs)
	{
		bm->AddWord(var);
	}
	Addmachine(bm);

	// Operations1
	om = new Type1Machine("Operations1", "Operation", "Operation1");
	char* os[] = { "++", "--", "==", "!=", "||", "&&", ">=", "<=", "<<", ">>", "::" };
	for each (char* var in os)
	{
		om->AddWord(var);
	}
	Addmachine(om);

	// Operations2
	om2 = new Type1Machine("Operations2", "Operation", "Operation2");
	char* os2[] = { "+", "-", "=", "/", "*", "|", "&", ">", "<", "%", ":", "!", "?" };
	for each (char* var in os2)
	{
		om2->AddWord(var);
	}
	Addmachine(om2);

	// Numbers
	nm = new Type2Machine("Numbers", "Number", "float ");
	char nm_perstart[] = "0123456789.";
	char nm_optional[] = "e";
	nm->AddWord(nm_perstart);
	nm->AddWord(nm_optional);
	nm->SetPerStartWords(nm_perstart);
	Addmachine(nm);

	// Variables
	vm = new Type2Machine("Variables", "Variable", "Variable");
	char vm_perstart[] = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	char vm_optional[] = "1234567890";
	vm->AddWord(vm_perstart);
	vm->AddWord(vm_optional);
	vm->SetPerStartWords(vm_perstart);
	Addmachine(vm);

	// Deviders
	dm = new Type1Machine("Deviders", "Devider", "Devider");
	char* ds[] = { ";", "(", ")", "{", "}", "[", "]"};
	for each (char* var in ds)
	{
		dm->AddWord(var);
	}
	Addmachine(dm);

	// String & Symbols
	ssm = new Type3Machine("String & Symbols", "String", "string ");
	char ssm_perstart[] = "\"\'";
	char ssm_optional[] = "\\|:,<.>/?;\*]}[{-=!@#$%^&*()+`~";
	ssm->AddWord(ssm_perstart);
	ssm->AddWord(ssm_optional);
	ssm->AddWord(vm_perstart);
	ssm->AddWord(vm_optional);
	ssm->SetPerStartWords(ssm_perstart);
	Addmachine(ssm);
	
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
	mFileReader fr = *new mFileReader(filename);
	char str[String];

	//for comments
	bool EnterInComment = false;
	int commentline = 0;
	int commentpos = 0;
	//

	while (!fr.EndFile())
	{
		strcpy_s(str, fr.ReadNextLine());
		int lenght = (int)strlen(str);
		for (int s = 0; s < lenght; s++)
		{
			//if (!EnterInComment)
			if (!EnterInComment && (_currentaut->IsStart() || (str[s] != ' ' && str[s] != '\t')) && (str[s] != EOF || _currentaut->IsStart()))
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
						NumberCheck(fr.CurrentLine());
						if (_error)
							return -2;
					}
					if (strcmp(_currentaut->MachineName(), "Deviders") == 0)
					{
						Hooker(_currentaut->Buffer());
						if (_error)
							return -5;
						_currentaut->Priority = 0;
					}
					if (strcmp(_currentaut->CurrentLexName(), "Operation") == 0)
					{
						GetOperationPriority(_currentaut);
					}
					lexes->add(new lexeme(_currentaut->CurrentLexName(),
						_currentaut->CurrentLexType(), _currentaut->Buffer(),
						_currentaut->CurrentLexLine(), _currentaut->CurrentLexPos(), _currentaut->Priority));
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
								ErrorReporter().FReport(stdout, "Unprocessed word!", fr.CurrentLine(), _currentaut->CurrentLexPos());
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
				if (fr.EndFile() && s >= strlen(str) - 1 && EnterInComment)
				{
					ErrorReporter().FReport(stdout, "Maybe you forget to close the comment", commentline, commentpos);
					_error = false;
					return -1;
				}
				if (fr.EndFile())
				{
					if (!HooksCheck(&fr))
					{
						_error = true;
						return -386;
					}
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
	if (!LWorker.Processing(lexes))
		return -6;
	return 1;
}

int MachineWorker::Count()
{
	return _count;
}

int MachineWorker::GetOperationPriority(mStateMachine * machine)
{
	if (strcmp("*", _currentaut->Buffer()) == 0)
		return 25;
	if (strcmp(_currentaut->CurrentLexName(), "Operation") != 0)
		return 100;
	return 50;
}

void MachineWorker::UpdateMachines()
{
	_currentaut->UpdateStatus();
	_currentaut = _machines[0];
	_curmachine = 0;
}

void MachineWorker::Addmachine(mStateMachine * machine)
{
	_machines[_count++] = machine;
}

void MachineWorker::NumberCheck(int line)
{
	char* first = (char*)heap.get_mem(sizeof(char) * Chunck * _currentaut->ChunkCount());
	first[0] = '\0';

	char* second = (char*)heap.get_mem(sizeof(char) * Chunck * _currentaut->ChunkCount());
	second[0] = 0;

	char* number = (char*)heap.get_mem(sizeof(char) * Chunck * _currentaut->ChunkCount());
	number[0] = '\0';
	strcpy_s(number, Chunck * _currentaut->ChunkCount(), _currentaut->Buffer());

	int count = strlen(number);
	int i = 0;
	while (i < (int)(strlen(number)))
	{
		if (number[i] != '.')
		{
			if (number[i] == 'e')
			{
				ErrorReporter().FReport(stdout, "Uncorrect number. \"e\" - не может быть в числителе.", line, _currentaut->CurrentLexPos() + i);
				_error = true;
				heap.free_mem(first);
				heap.free_mem(second);
				heap.free_mem(number);
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
			ErrorReporter().FReport(stdout, "Uncorrect number. \".\" - не может быть несколько.", line, _currentaut->CurrentLexPos() + i);
			_error = true;
			heap.free_mem(first);
			heap.free_mem(second);
			heap.free_mem(number);
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
				ErrorReporter().FReport(stdout, "Uncorrect number. \"e\" - не может быть несколько.", line, _currentaut->CurrentLexPos() + i);
				_error = true;
				heap.free_mem(first);
				heap.free_mem(second);
				heap.free_mem(number);
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
			_currentaut->ChangeType("double ");
		else
			_currentaut->ChangeType("int ");
	}
	heap.free_mem(first);
	heap.free_mem(second);
	heap.free_mem(number);
	return;
}

void MachineWorker::Hooker(char * buffer)
{
	if (strcmp(buffer, "(") == 0)
	{
		_circlehooks++;
		return;
	}
	if (strcmp(buffer, ")") == 0)
	{
		_circlehooks--;
		if (_circlehooks < 0)
		{
			ErrorReporter().FReport(stdout, "You have lost \"(\"", _currentaut->CurrentLexLine(), _currentaut->CurrentLexPos());
			_error = true;
		}
		return;
	}
	if (strcmp(buffer, "[") == 0)
	{
		_squarehooks++;
		return;
	}
	if (strcmp(buffer, "]") == 0)
	{
		_squarehooks--;
		if (_squarehooks < 0)
		{
			ErrorReporter().FReport(stdout, "You have lost \"[\"", _currentaut->CurrentLexLine(), _currentaut->CurrentLexPos());
			_error = true;
		}
		return;
	}
	if (strcmp(buffer, "{") == 0)
	{
		_hooks++;
		return;
	}
	if (strcmp(buffer, "}") == 0)
	{
		_hooks--;
		if (_hooks < 0)
		{
			ErrorReporter().FReport(stdout, "You have lost \"{\"", _currentaut->CurrentLexLine(), _currentaut->CurrentLexPos());
			_error = true;
		}
		return;
	}
}

bool MachineWorker::HooksCheck(mFileReader* f)
{
	char temp[Chunck];
	if (_hooks > 0)
	{
		sprintf_s(temp, "You need to close %d hook(s)", _hooks);
		ErrorReporter().FReport(stdout, temp, f->CurrentLine(), 0);
		return false;
	}
	if (_circlehooks > 0)
	{
		sprintf_s(temp, "You need to close %d circle hook(s)", _circlehooks);
		ErrorReporter().FReport(stdout, temp, f->CurrentLine(), 0);
		return false;
	}
	if (_squarehooks > 0)
	{
		sprintf_s(temp, "You need to close %d square hook(s)", _squarehooks);
		ErrorReporter().FReport(stdout, temp, f->CurrentLine(), 0);
		return false;
	}
	return true;
}
