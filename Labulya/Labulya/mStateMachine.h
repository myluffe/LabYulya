#pragma once
#include "list.h"

class mStateMachine
{
public:
	mStateMachine(char* name, char* lexname, char* lextype);
	~mStateMachine();

	char* Buffer();
	char* MachineName();

	bool IsFinished();
	bool IsError();

	char* CurrentLexName();
	char* CurrentLexType();
	void ChangeType(char* str);

	//Test
	void PrintMachine();
	//

	int CurrentLexPos();
	int Priority = 100;

	void AddWord(void * word);
	virtual void EnterChar(char ch, int pos) = 0;
	virtual void CheckStart(char ch) = 0;
	void UpdateStatus();
protected:
	List* _words = new List(sizeof(char[30]));

	virtual void ClearAdditional() = 0;
	//
	virtual void PrintAdditionals() = 0;
	//

	char _machineName[30]; //name
	char _buffer[50]; //current lexem

	bool _isFinished;
	bool _isError;
	bool _start; // true if we are expect this type lexeme
	
	char _currentLexemName[30];
	char _currentLexemeType[30];

	int _currentLexemePosition;
	int _step;
};

class Type1Machine : public mStateMachine
{
public:
	Type1Machine(char* name, char* lexname, char* lextype) : mStateMachine(name, lexname, lextype) 
	{
		_words = new List(sizeof(char[30]));
	};
	~Type1Machine();

	void CheckStart(char ch);
	void EnterChar(char ch, int pos);
	void ClearAdditional();
private:
	List _potentialwords = *new List(sizeof(char[30]));
	void PrintAdditionals();
};

class Type2Machine : public mStateMachine
{
public:
	Type2Machine(char* name, char* lexname, char* lextype) : mStateMachine(name, lexname, lextype)
	{
		_words = new List(sizeof(char[60]));
		_permissiblestart[0] = '\0';
	};
	~Type2Machine();

	void CheckStart(char ch);
	void EnterChar(char ch, int pos);
	void AddPerStartWord(char word);
	void SetPerStartWords(char* words);
	void ClearAdditional();
private:
	char _permissiblestart[60];
	void PrintAdditionals();
};