#pragma once
#include "list.h"
#include "ErrorReporter.h"

#define Chunck 50
#define Type23Chunck 60

class mStateMachine
{
public:
	mStateMachine(char* name, char* lexname, char* lextype);
	~mStateMachine();

	char* Buffer();
	char* MachineName();

	bool IsFinished();
	bool IsError();
	bool IsStart();

	bool CheckError();

	char* CurrentLexName();
	char* CurrentLexType();
	void ChangeType(char* str);

	//Test
	void PrintMachine();
	//

	int CurrentLexPos();
	int CurrentLexLine();
	int Priority = 100;
	int ChunkCount();

	void AddWord(void * word);
	virtual void EnterChar(char ch, int pos, int line) = 0;
	virtual void CheckStart(char ch) = 0;
	void UpdateStatus();
protected:
	List* _words = new List(sizeof(char[Chunck]));

	virtual void ClearAdditional() = 0;
	//
	virtual void PrintAdditionals() = 0;
	//

	char _machineName[Chunck]; //name
	char* _buffer; //current lexem

	bool _isFinished;
	bool _isError;
	bool _start; // true if we are expect this type lexeme

	bool _checkError;
	
	char _currentLexemName[Chunck];
	char _currentLexemeType[Chunck];

	int _currentLexemePosition;
	int _curlexline;
	int _step;
	int _chunckcount;
};

class Type1Machine : public mStateMachine
{
public:
	Type1Machine(char* name, char* lexname, char* lextype) : mStateMachine(name, lexname, lextype) 
	{
		_words = new List(sizeof(char[Chunck]));
	};
	~Type1Machine();

	void CheckStart(char ch);
	void EnterChar(char ch, int pos, int line);
	void ClearAdditional();
private:
	List _potentialwords = *new List(sizeof(char[Chunck]));
	void PrintAdditionals();
};

class Type2Machine : public mStateMachine
{
public:
	Type2Machine(char* name, char* lexname, char* lextype) : mStateMachine(name, lexname, lextype)
	{
		_words = new List(sizeof(char[Type23Chunck]));
		_permissiblestart[0] = '\0';
	};
	~Type2Machine();

	void CheckStart(char ch);
	virtual void EnterChar(char ch, int pos, int line);
	void AddPerStartWord(char word);
	void SetPerStartWords(char* words);
	void ClearAdditional();
protected:
	char _permissiblestart[Type23Chunck];
	void PrintAdditionals();
};

class Type3Machine : public Type2Machine
{
public:
	Type3Machine(char* name, char* lexname, char* lextype) : Type2Machine(name, lexname, lextype)
	{ }
	~Type3Machine();
	void EnterChar(char ch, int pos, int line);
private:
	char _permissiblestart[Type23Chunck];
	void CheckType();
};