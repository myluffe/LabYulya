#include "stdafx.h"
#include "mStateMachine.h"

mStateMachine::mStateMachine(char* name, char* lexname, int lextype)
{
	_isFinished = false;
	_isError = false;
	_start = false;
	_checkError = false;
	_buffer = (char*)heap.get_mem(sizeof(char) * Chunck);
	_buffer[0] = '\0';
	strcpy_s(_machineName, name);
	strcpy_s(_currentLexemName, lexname);
	_currentLexemeType = lextype;
	_originaltype = lextype;
	_currentLexemePosition = -100;
	_curlexline = -100;
	_step = 0;
	_chunckcount = 1;
}

mStateMachine::~mStateMachine()
{
	heap.free_mem(_buffer);
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

bool mStateMachine::CheckError()
{
	return _checkError;
}

char * mStateMachine::CurrentLexName()
{
	return _currentLexemName;
}

int mStateMachine::CurrentLexType()
{
	return _currentLexemeType;
}

void mStateMachine::ChangeType(int stype)
{
	_currentLexemeType = stype;
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

int mStateMachine::CurrentLexLine()
{
	return _curlexline;
}

int mStateMachine::ChunkCount()
{
	return _chunckcount;
}

void mStateMachine::AddWord(void * word)
{
	_words->add(word);
}

void mStateMachine::UpdateStatus()
{
	_currentLexemeType = _originaltype;
	_isError = false;
	_isFinished = false;
	_start = false;
	_checkError = false;
	_step = 0;
	_chunckcount = 1;
	_buffer = (char*)heap.get_mem(sizeof(char) * Chunck);
	_buffer[0] = '\0';
	_currentLexemePosition = -100;
	_curlexline = -100;
	ClearAdditionals();
	Priority = 100;
}

void Type1Machine::ClearAdditionals()
{
	_potentialwords.~List();
	_potentialwords = List(sizeof(char[Chunck]));
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

void Type2Machine::ClearAdditionals()
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
	char temp[Chunck];
	for (int i = 0; i < listcount; i++)
	{
		strcpy_s(temp, (char*)_words->get(i));
		if (temp[0] == ch)
		{
			if (!_start)
			{
				_buffer[_step++] = ch;
				_buffer[_step] = '\0';
				_start = true;
			}
			_potentialwords.add(temp);
		}
	}
}

void Type1Machine::EnterChar(char ch, int pos, int line)
{
	if (_currentLexemePosition == -100)
	{
		_currentLexemePosition = pos;
		_curlexline = line;
	}
	if (!_start)
	{
		CheckStart(ch);
		if (!_start) _isError = true;
		return;
	}
	int listcount = _potentialwords.count();
	char temp[Chunck];
	bool f = false;
	
	if (listcount == 0)
	{
		_isError = true;
		return;
	}

	if (listcount == 1 && ((strlen(_buffer) >= strlen((char*)_potentialwords.get(0)))))
	{
		if (strcmp(_buffer, (char*)_potentialwords.get(0)) == 0)
		{
			_isFinished = true;
			_potentialwords.~List();
			_potentialwords = List(sizeof(char[Chunck]));
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
		bool ftemp;
		if (!f) ftemp = (temp[_step] != ch);
		else 
			ftemp = (temp[_step - 1] != ch);
		if (ftemp)
		{
			//test for missed space
			char stemp[Chunck];
			strcpy_s(stemp, (char*)_potentialwords.get(i));
			stemp[strlen(stemp) - 1] = '\0';
			if (strcmp(stemp, _buffer) == 0 && temp[strlen(temp) - 1] == ' ')
			{
				char sstemp[Chunck];
				sprintf_s(sstemp, "Maybe you misse space? Maybe you mean \"%s\"?", temp);
				ErrorReporter().WarningReport(stdout, sstemp, _curlexline, _currentLexemePosition + strlen(_buffer));
			}
			//
			_potentialwords.remove(i);
			listcount--;
			i--;
		}
		else if (!f)
		{
			if (_step >= Chunck * _chunckcount) //Chunck * кол-во кусков
			{
				_chunckcount++;
				char* _buffer2 = (char*)heap.get_mem(sizeof(char) * Chunck * _chunckcount);
				strcpy_s(_buffer2, strlen(_buffer), _buffer);
				heap.free_mem(_buffer);
				_buffer = _buffer2;
			}
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
			_buffer[_step++] = ch;
			_buffer[_step] = '\0';
			return;
		}
	}
}

void Type2Machine::EnterChar(char ch, int pos, int line)
{
	if (_currentLexemePosition == -100)
	{
		_currentLexemePosition = pos;
		_curlexline = line;
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
			if (_step >= Chunck * _chunckcount) //50 * кол-во кусков
			{
				_chunckcount++;
				char* _buffer2 = (char*)heap.get_mem(sizeof(char) * Chunck * _chunckcount);
				strcpy_s(_buffer2, strlen(_buffer), _buffer);
				heap.free_mem(_buffer);
				_buffer = _buffer2;
			}
			_buffer[_step++] = ch;
			_buffer[_step] = '\0';
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

Type3Machine::~Type3Machine()
{
}

void Type3Machine::EnterChar(char ch, int pos, int line)
{
	if (_currentLexemePosition == -100)
	{
		_currentLexemePosition = pos;
		_curlexline = line;
	}
	if (!_start)
	{
		CheckStart(ch);
		if (!_start) _isError = true;
		return;
	}

	if (((_step - 1) > 0) && (_step - 1 != 0) && (_buffer[_step - 1] == _buffer[0]) && ch != '\\')
	{
		_isFinished = true;
		CheckType();
		return;
	}

	int listcount = _words->count();
	_isFinished = true;
	for (int i = 0; i < listcount; i++)
	{
		if (strchr((char*)_words->get(i), ch) != nullptr)
		{
			_isFinished = false;
			_buffer[_step++] = ch;
			_buffer[_step] = '\0';
			break;
		}
	}

	if (_isFinished)
	{
		if (_buffer[strlen(_buffer) - 1] != _buffer[0])
		{
			_isFinished = false;
			_isError = true;
		}
	}
}

void Type3Machine::CheckType()
{
	if (_buffer[0] == '\'')
	{
		strcpy_s(_currentLexemName, "Char");
		_currentLexemeType = CHAR;
	}
	char* tbuffer = (char*)heap.get_mem(sizeof(char) * Chunck * _chunckcount);
	for (int k = 1; k < strlen(_buffer) - 1; k++)
	{
		tbuffer[k - 1] = _buffer[k];
	}
	tbuffer[strlen(_buffer) - 2] = '\0';
	strcpy_s(_buffer, Chunck * _chunckcount, tbuffer);
	heap.free_mem(tbuffer);
	if (_currentLexemeType == CHAR)
	{
		int slen = strlen(_buffer);
		if (slen > 1)
		{
			if (slen != 2)
			{
				ErrorReporter().FReport(stdout, "To many symbols for char!", _curlexline, _currentLexemePosition);
				_checkError = true;
				return;
			}
			else
			{
				if (_buffer[0] != '\\')
				{
					ErrorReporter().FReport(stdout, "To many Symbols for char!", _curlexline, _currentLexemePosition);
					_checkError = true;
					return;
				}
			}
		}
	}
}
