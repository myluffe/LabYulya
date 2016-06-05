#include "stdafx.h"
#include "MachineWorker.h"
#include "LexemeWorker.h"

MachineWorker::MachineWorker()
{
	_count = 0;

	// Special Words
	sm = new Type1Machine("Special Words", "Special Word", SPECIALWORD);
	char* ss[] = { "//", "/*", "*/", "for ", "if ", "else ", "while ", "do ", "input ", "output ", "max ", "min ", "sin ", "cos " };
	for each (char* var in ss)
	{
		sm->AddWord(var);
	}
	Addmachine(sm);

	// Types
	tm = new Type1Machine("Types", "Type", TYPE);
	char* ts[] = { "class ", "struct ", "int ", "float ", "double ", "char ", "bool ", "string " };
	for each (char* var in ts)
	{
		tm->AddWord(var);
	}
	Addmachine(tm);

	//Bool
	bm = new Type1Machine("Bools", "Bool", BOOL);
	char* bs[] = { "true ", "false " };
	for each (char* var in bs)
	{
		bm->AddWord(var);
	}
	Addmachine(bm);

	// Operations1
	//"?:" как добавить
	om = new Type1Machine("Operations1", "Operation", OPERATION);
	char* os[] = { "++", "--", "==", "!=", "||", "&&", ">=", "<=", "<<", ">>" };
	for each (char* var in os)
	{
		om->AddWord(var);
	}
	Addmachine(om);

	// Numbers
	nm = new Type2Machine("Numbers", "Number", FLOAT);
	char nm_perstart[] = "0123456789.";
	char nm_optional[] = "e";
	nm->AddWord(nm_perstart);
	nm->AddWord(nm_optional);
	nm->SetPerStartWords(nm_perstart);
	Addmachine(nm);

	// Operations2
	om2 = new Type1Machine("Operations2", "Operation", OPERATION);
	char* os2[] = { "+", "-", "=", "/", "*", "|", "&", ">", "<", "%", ":", "!", "?", "," };
	for each (char* var in os2)
	{
		om2->AddWord(var);
	}
	Addmachine(om2);

	// Variables
	vm = new Type2Machine("Variables", "Variable", VARIABLE);
	char vm_perstart[] = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	char vm_optional[] = "1234567890";
	vm->AddWord(vm_perstart);
	vm->AddWord(vm_optional);
	vm->SetPerStartWords(vm_perstart);
	Addmachine(vm);

	// Deviders
	dm = new Type1Machine("Deviders", "Devider", DEVIDER);
	char* ds[] = { ";", "(", ")", "{", "}", "[", "]"};
	for each (char* var in ds)
	{
		dm->AddWord(var);
	}
	Addmachine(dm);

	// String & Symbols
	ssm = new Type3Machine("String & Symbols", "String", STRING);
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
	sm->~Type1Machine();
	om->~Type1Machine();
	om2->~Type1Machine();
	dm->~Type1Machine();
	tm->~Type1Machine();
	bm->~Type1Machine();

	nm->~Type2Machine();
	vm->~Type2Machine();

	ssm->~Type3Machine();
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
	bool SuperFlag = false;

	while (!fr.EndFile())
	{
		if (!_currentaut->IsStart())
			strcpy_s(str, fr.ReadNextLine());
		else SuperFlag = true;
		int lenght = (int)strlen(str);
		for (int s = 0; s < lenght; s++)
		{
			//if (!EnterInComment)
			if (!EnterInComment && (_currentaut->IsStart() || (str[s] != ' ' && str[s] != '\t')) && (str[s] != EOF || _currentaut->IsStart()))
			{
				if (!SuperFlag)
					_currentaut->EnterChar(str[s], s, fr.CurrentLine());
				else
				{
					s = sizeof(strlen(str));
					_currentaut->EnterChar('\0', s, fr.CurrentLine());
					SuperFlag = false;
				}
				if (_currentaut->IsFinished())
				{
					if (_currentaut->CheckError())
					{
						fr.~mFileReader();
						return -3;
					}
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
						{
							fr.~mFileReader();
							return -2;
						}
					}
					if (strcmp(_currentaut->MachineName(), "Deviders") == 0)
					{
						Hooker(_currentaut->Buffer());
						if (_error)
						{
							fr.~mFileReader();
							return -5;
						}
						GetOperationPriority(_currentaut);
					}
					if (strcmp(_currentaut->CurrentLexName(), "Operation") == 0)
					{
						lexeme* tlex = (lexeme*)lexes->get(lexes->count() - 1);
						if (tlex->Type() == VARIABLE || tlex->Type() == NUMBER || strcmp(tlex->Data(), ")") == 0)
							_currentaut->ChangeType(BYNARYOPERATION);
						else _currentaut->ChangeType(UNARYOPERATION);
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
							if (str[s] == EOF)
								ErrorReporter().FReport(logfile, "Unprocessed word!", fr.CurrentLine(), _currentaut->CurrentLexPos());
							UpdateMachines();
							fr.~mFileReader();
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
					ErrorReporter().FReport(logfile, "Maybe you forget to close the comment", commentline, commentpos);
					//_error = false;
					fr.~mFileReader();
					return -1;
				}
				if (fr.EndFile())
				{
					if (!HooksCheck(&fr))
					{
						//_error = true;
						fr.~mFileReader();
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
	{
		fr.~mFileReader();
		return -6;
	}
	fr.~mFileReader();
	return 1;
}

int MachineWorker::Count()
{
	return _count;
}

void MachineWorker::GetOperationPriority(mStateMachine * machine)
{
	if (strcmp("(", machine->Buffer()) == 0 || strcmp(")", machine->Buffer()) == 0)
		machine->Priority = 0;
	if (strcmp("--", machine->Buffer()) == 0 || strcmp("++", machine->Buffer()) == 0)
		machine->Priority = 2;
	if (strcmp("!", machine->Buffer()) == 0)
		machine->Priority = 3;
	if (strcmp("/", machine->Buffer()) == 0 || strcmp("%", machine->Buffer()) == 0)
		machine->Priority = 5;
	if (strcmp("<<", machine->Buffer()) == 0 || strcmp(">>", machine->Buffer()) == 0)
		machine->Priority = 7;
	if (strcmp("<", machine->Buffer()) == 0 || strcmp(">", machine->Buffer()) == 0 ||
		strcmp("<=", machine->Buffer()) == 0 || strcmp(">=", machine->Buffer()) == 0)
		machine->Priority = 8;
	if (strcmp("==", machine->Buffer()) == 0 || strcmp("!=", machine->Buffer()) == 0)
		machine->Priority = 9;
	if (strcmp("|", machine->Buffer()) == 0)
		machine->Priority = 12;
	if (strcmp("&&", machine->Buffer()) == 0)
		machine->Priority = 13;
	if (strcmp("||", machine->Buffer()) == 0)
		machine->Priority = 14;
	//if (strcmp("?:", machine->Buffer()) == 0)
	//	machine->Priority = 15;
	if (strcmp(":", machine->Buffer()) == 0 || strcmp("?", machine->Buffer()))
	{
		machine->Priority = 15;
		machine->ChangeType(TERNARYOPERATION);
	}
	if (strcmp("=", machine->Buffer()) == 0)
		machine->Priority = 16;
	if (strcmp(",", machine->Buffer()) == 0)
		machine->Priority = 17;

	if (strcmp("+", machine->Buffer()) == 0 || strcmp("-", machine->Buffer()) == 0)
	{
		if (machine->CurrentLexType() == BYNARYOPERATION)
			machine->Priority = 6;
		if (machine->CurrentLexType() == UNARYOPERATION)
			machine->Priority = 3;
	}

	if (strcmp("*", machine->Buffer()) == 0)
	{
		if (machine->CurrentLexType() == BYNARYOPERATION)
			machine->Priority = 10;
		if (machine->CurrentLexType() == UNARYOPERATION)
			machine->Priority = 3;
	}

	if (strcmp("&", machine->Buffer()) == 0)
	{
		if (machine->CurrentLexType() == BYNARYOPERATION)
			machine->Priority = 5;
		if (machine->CurrentLexType() == UNARYOPERATION)
			machine->Priority = 3;
	}


	if (strcmp(machine->CurrentLexName(), "Operation") == 0)
		machine->Priority = 50;
	machine->Priority = 100;
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
	bool flagE = false;

	while (i < (int)(strlen(number)))
	{
		if (number[i] != '.')
		{
			if (number[i] == 'e')
			{
				if (flagE)
				{
					ErrorReporter().FReport(logfile, "Uncorrect number. \"e\" - уже встретилось.", line, _currentaut->CurrentLexPos() + i);
					_error = true;
					heap.free_mem(first);
					heap.free_mem(second);
					heap.free_mem(number);
					return;
				}
				else flagE = true;
			}
			first[i] = number[i];
			i++;
		}
		else
		{
			if (flagE)
			{
				ErrorReporter().FReport(logfile, "Uncorrect number. \"e\" - если присутствует в числителе, знаменателя не может быть.", line, _currentaut->CurrentLexPos() + i);
				_error = true;
				heap.free_mem(first);
				heap.free_mem(second);
				heap.free_mem(number);
				return;
			}
			break;
		}
	}

	first[i] = '\0';

	int j = 0;
	int e_count = 0;
	i++;

	while (i < (int)(strlen(number)))
	{
		if (number[i] == '.')
		{
			ErrorReporter().FReport(logfile, "Uncorrect number. \".\" - не может быть несколько.", line, _currentaut->CurrentLexPos() + i);
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
				ErrorReporter().FReport(logfile, "Uncorrect number. \"e\" - не может быть несколько.", line, _currentaut->CurrentLexPos() + i);
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
		if (strlen(first) > 9)
			_currentaut->ChangeType(DOUBLE);
		else
			_currentaut->ChangeType(INT);
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
			ErrorReporter().FReport(logfile, "You have lost \"(\"", _currentaut->CurrentLexLine(), _currentaut->CurrentLexPos());
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
			ErrorReporter().FReport(logfile, "You have lost \"[\"", _currentaut->CurrentLexLine(), _currentaut->CurrentLexPos());
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
			ErrorReporter().FReport(logfile, "You have lost \"{\"", _currentaut->CurrentLexLine(), _currentaut->CurrentLexPos());
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
		ErrorReporter().FReport(logfile, temp, f->CurrentLine(), 0);
		return false;
	}
	if (_circlehooks > 0)
	{
		sprintf_s(temp, "You need to close %d circle hook(s)", _circlehooks);
		ErrorReporter().FReport(logfile, temp, f->CurrentLine(), 0);
		return false;
	}
	if (_squarehooks > 0)
	{
		sprintf_s(temp, "You need to close %d square hook(s)", _squarehooks);
		ErrorReporter().FReport(logfile, temp, f->CurrentLine(), 0);
		return false;
	}
	return true;
}
