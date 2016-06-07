#pragma once

#include "list.h"
#include "ErrorReporter.h"

#define Chunck 50
#define Type23Chunck 60

class mStateMachine
{
public:
	mStateMachine(char* name, char* lexname, int lextype);
	~mStateMachine();

	char* Buffer(); //���������� _buffer.
	char* MachineName(); //���������� ��� ������.

	bool IsFinished(); //���������� _isFinished.
	bool IsError(); //���������� _isError.
	bool IsStart(); //���������� _start.

	bool CheckError();

	char* CurrentLexName(); //
	int CurrentLexType();
	void ChangeType(int stype);

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

	virtual void ClearAdditionals() = 0; //�������������� ������� ��� ����������� ������� ������. ������������ � ������������ �����������.

	virtual void PrintAdditionals() = 0; //���������� ������ ��� ����������� ������� ������.

	char _machineName[Chunck]; //��� ������.
	char* _buffer; //������, � ������� ������������ ������� ��� ������������ �������. � ������ ������ ������� ����� ������ �� ��������������� �����, �������������� �������.

	bool _isFinished; //true, ���� ������� ������� ����������� �������.
	bool _isError; //true, ���� ������ ������� �� ���� ������������ �������.
	bool _start; //true, ���� �� ������ ������ � ������ ���������.

	bool _checkError;
	
	char _currentLexemName[Chunck]; //��� ����������� �������
	int _currentLexemeType; //������� ��� ����������� �������
	int _originaltype; //��� ����������� ������ ������� ������ �� ��������� 

	int _currentLexemePosition; //��������� �������, ������������ ������ ������, �������, ������� ����������� � ������ ������.
	int _curlexline; //����� ������ �������, ������� ����������� � ������ ������.
	int _step; //���. ������� _buffer, � ������� ����� ������������ ��������� ������ ��� ������������ �������.
	
	int _chunckcount; //���������� ������, ������� ������� � ���� _buffer. �� ��������� _chunckcount = 1, � _buffer[chunck]. ��� ����������� ��������� ����� ������ �� ���� chunk ������ �����������, � ������� ���������� ������ _buffer, _chunckcount ������������� �� �������. ����� ������, ��� ��������������� ������� ��� ������������� ��������� ������ �� ������ ��� _buffer.
};

class Type1Machine : public mStateMachine
{
public:
	Type1Machine(char* name, char* lexname, int lextype) : mStateMachine(name, lexname, lextype) 
	{
		_words = new List(sizeof(char[Chunck]));
	};
	~Type1Machine();

	void CheckStart(char ch);
	void EnterChar(char ch, int pos, int line);
	void ClearAdditionals();
private:
	List _potentialwords = *new List(sizeof(char[Chunck]));
	void PrintAdditionals();
};

class Type2Machine : public mStateMachine
{
public:
	Type2Machine(char* name, char* lexname, int lextype) : mStateMachine(name, lexname, lextype)
	{
		_words = new List(sizeof(char[Type23Chunck]));
		_permissiblestart[0] = '\0';
	};
	~Type2Machine();

	void CheckStart(char ch);
	virtual void EnterChar(char ch, int pos, int line);
	void AddPerStartWord(char word);
	void SetPerStartWords(char* words);
	void ClearAdditionals();
protected:
	char _permissiblestart[Type23Chunck];
	void PrintAdditionals();
};

class Type3Machine : public Type2Machine
{
public:
	Type3Machine(char* name, char* lexname, int lextype) : Type2Machine(name, lexname, lextype)
	{ }
	~Type3Machine();
	void EnterChar(char ch, int pos, int line);
private:
	char _permissiblestart[Type23Chunck];
	void CheckType();
};