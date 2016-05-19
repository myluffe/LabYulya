#include "stdafx.h"
#include "mStateMachine.h"

mStateMachine::mStateMachine(char* name, char* lexname, char* lextype)
{
	_isFinished = false;
	_isError = false;
	_start = false;
	_buffer[0] = '\0';
	strcpy_s(_machineName, name);
	strcpy_s(_currentLexemName, lexname);
	strcpy_s(_currentLexemeType, lextype);
	_currentLexemePosition = -100;
	_step = 0;
}

mStateMachine::~mStateMachine()
{
	_words->~List();
}

char * mStateMachine::Buffer()
{
	return _buffer;
}

char * mStateMachine::MachineName()
{
	return _machineName;
}

bool mStateMachine::IsFinished()
{
	return _isFinished;
}

bool mStateMachine::IsError()
{
	return _isError;
}

bool mStateMachine::IsStart()
{
	return _start;
}

char * mStateMachine::CurrentLexName()
{
	return _currentLexemName;
}

char * mStateMachine::CurrentLexType()
{
	return _currentLexemeType;
}

void mStateMachine::ChangeType(char* str)
{
	strcpy_s(_currentLexemeType, str);
}

void mStateMachine::PrintMachine()
{
	printf("Machine: %s\nWords: ", MachineName());
	for (int i = 0; i < _words->count(); i++)
	{
		printf("\"%s\" ", (char*)_words->get(i));
	}
	printf("\n");
	printf("Buffer: \"%s\"\n", _buffer);
	PrintAdditionals();
}

int mStateMachine::CurrentLexPos()
{
	return _currentLexemePosition;
}

void mStateMachine::AddWord(void * word)
{
	_words->add(word);
}

void mStateMachine::UpdateStatus()
{
	_isError = false;
	_isFinished = false;
	_start = false;
	_step = 0;
	strcpy_s(_buffer, "");
	_currentLexemePosition = -100;
	ClearAdditional();
	Priority = 100;
}

void Type1Machine::ClearAdditional()
{
	_potentialwords.~List();
	_potentialwords = List(sizeof(char[30]));
}

void Type1Machine::PrintAdditionals()
{	
	printf("Potential Words: ");
	for (int i = 0; i < _potentialwords.count(); i++)
	{
		printf("\"%s\" ", (char*)_potentialwords.get(i));
	}
	printf("\n");
}

void Type2Machine::ClearAdditional()
{
}

void Type2Machine::PrintAdditionals()
{
	printf("Permissible Start: ");
	int count = strlen(_permissiblestart);
	for (int i = 0; i < count; i++)
	{
		printf("\'%c\' ", _permissiblestart[i]);
	}
	printf("\n");
}

Type1Machine::~Type1Machine()
{
	_potentialwords.~List();
}

void Type1Machine::CheckStart(char ch)
{
	int listcount = _words->count();
	char temp[20];
	bool f = false;
	for (int i = 0; i < listcount; i++)
	{
		strcpy_s(temp, (char*)_words->get(i));
		if (temp[0] == ch)
		{
			if (!f)
			{
				_buffer[_step++] = ch;
				_buffer[_step] = '\0';
				f = true;
			}
			_start = true;
			_potentialwords.add(temp);	
			return;
		}
	}
}

void Type1Machine::EnterChar(char ch, int pos)
{
	if (_currentLexemePosition == -100)
	{
		_currentLexemePosition = pos;
	}
	if (!_start)
	{
		CheckStart(ch);
		if (!_start) _isError = true;
		return;
	}
	int listcount = _potentialwords.count();
	char temp[30];
	bool f = false;
	
	if (listcount == 0)
	{
		_isError = true;
		return;
	}
	if (listcount == 1 && (strlen(_buffer) >= strlen((char*)_potentialwords.get(0))))
	{
		if (strcmp(_buffer, (char*)_potentialwords.get(0)) == 0)
		{
			_isFinished = true;
			_potentialwords.~List();
			_potentialwords = List(sizeof(char[30]));
			return;
		}
		else
		{
			_isError = true;
			return;
		}
	}

	for (int i = 0; i < listcount; i++)
	{
		strcpy_s(temp, (char*)_potentialwords.get(i));
		if (temp[_step] != ch)
		{
			_potentialwords.remove(i);
			listcount--;
			i--;
		}
		else if (!f)
		{
			_buffer[_step++] = ch;
			_buffer[_step] = '\0';
			f = true;
		}
	}
}

Type2Machine::~Type2Machine()
{
	//nothing
}

void Type2Machine::CheckStart(char ch)
{
	int listcount = strlen(_permissiblestart);
	for (int i = 0; i < listcount; i++)
	{
		if (_permissiblestart[i] == ch)
		{
			_start = true;
			strcat_s(_buffer, &ch);
			_step++;
			return;
		}
	}
}

void Type2Machine::EnterChar(char ch, int pos)
{
	if (_currentLexemePosition == -100)
	{
		_currentLexemePosition = pos;
	}
	if (!_start)
	{
		CheckStart(ch);
		if (!_start) _isError = true;
		return;
	}
	int listcount = _words->count();
	_isFinished = true;
	for (int i = 0; i < listcount; i++)
	{
		if (strchr((char*)_words->get(i), ch) != nullptr)
		{
			_isFinished = false;
			strcat_s(_buffer, &ch);
			_step++;
			break;
		}
	}
}

void Type2Machine::AddPerStartWord(char word)
{
	strcat_s(_permissiblestart, &word);
}

void Type2Machine::SetPerStartWords(char * words)
{
	strcpy_s(_permissiblestart, words);
}
