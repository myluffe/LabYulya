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
	_words.~List();
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

int mStateMachine::CurrentLexPos()
{
	return _currentLexemePosition;
}

void mStateMachine::AddWord(void * word)
{
	_words.add(word);
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

void Type2Machine::ClearAdditional()
{
}

Type1Machine::~Type1Machine()
{
	_potentialwords.~List();
}

void Type1Machine::CheckStart(char ch)
{
	int listcount = _words.count();
	char temp[20];
	bool f = false;
	for (int i = 0; i < listcount; i++)
	{
		strcpy_s(temp, (char*)_words.get(i));
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
		}
		else
		{
			_isError = true;
		}
	}
}

Type2Machine::~Type2Machine()
{
	_permissiblestart.~List();
}

void Type2Machine::CheckStart(char ch)
{
	int listcount = _permissiblestart.count();
	for (int i = 0; i < listcount; i++)
	{
		//char ayaya = (char)_permissiblestart.get(i);
		if ((char)_permissiblestart.get(i) == ch)
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
	int listcount = _words.count();
	_isError = true;
	for (int i = 0; i < listcount; i++)
	{
		if ((char)_words.get(i) != ch)
		{
			_isError = false;
			strcat_s(_buffer, &ch);
			_step++;
			break;
		}
	}
}

void Type2Machine::AddPerStartWord(char word)
{
	_permissiblestart.add(&word);
}
