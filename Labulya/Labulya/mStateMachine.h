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

	char* CurrentLexName(); //���������� _machineName.
	int CurrentLexType(); //���������� _machineName.
	void ChangeType(int stype); //����� ���� �������, ������� �����������. ������������ � MachineWorker().NumberCheck ��� ����� ���� � DOUBLE �� INT ��� FLOAT.

	//�������� �������
	void PrintMachine();

	int CurrentLexPos(); //���������� _currentLexemePosition.
	int CurrentLexLine(); //���������� _curlexline.
	int Priority = 100; //��������� �������. � �������� �������� � MachineWorker().GetPriority.
	int ChunkCount(); //���������� _chunckcount.

	void AddWord(void * word); //��������� ����� � ������ ����� _words.
	virtual void EnterChar(char ch, int pos, int line) = 0; //������� "������� ������ � ������". � ������� ���������� ������� �� ������ ��� �� ��������� ������������ �������.
	virtual void CheckStart(char ch) = 0; //��� ������ ��������� � ������, �������� �� ������� �������, ����� �� ���� ������ �������� ���������� �������.
	void UpdateStatus(); //����� �������� ��������.
protected:
	List* _words = new List(sizeof(char[Chunck])); //����� ��������. ������������ �� ������� � �����������.

	virtual void ClearAdditionals() = 0; //�������������� ������� ��� ����������� ������� ������. ������������ � ������������ �����������.

	virtual void PrintAdditionals() = 0; //���������� ������ ��� ����������� ������� ������.

	char _machineName[Chunck]; //��� ������.
	char* _buffer; //������, � ������� ������������ ������� ��� ������������ �������. � ������ ������ ������� ����� ������ �� ��������������� �����, �������������� �������.

	bool _isFinished; //true, ���� ������� ������� ����������� �������.
	bool _isError; //true, ���� ������ ������� �� ���� ������������ �������.
	bool _start; //true, ���� �� ������ ������ � ������ ���������.
	
	char _currentLexemName[Chunck]; //��� ����������� �������
	int _currentLexemeType; //������� ��� ����������� �������
	int _originaltype; //��� ����������� ������ ������� ������ �� ��������� 

	int _currentLexemePosition; //��������� �������, ������������ ������ ������, �������, ������� ����������� � ������ ������.
	int _curlexline; //����� ������ �������, ������� ����������� � ������ ������.
	int _step; //���. ������� _buffer, � ������� ����� ������������ ��������� ������ ��� ������������ �������.
	
	int _chunckcount; //���������� ������, ������� ������� � ���� _buffer. �� ��������� _chunckcount = 1, � _buffer[chunck]. ��� ����������� ��������� ����� ������ �� ���� chunk ������ �����������, � ������� ���������� ������ _buffer, _chunckcount ������������� �� �������. ����� ������, ��� ��������������� ������� ��� ������������� ��������� ������ �� ������ ��� _buffer.
};

class Type1Machine : public mStateMachine //������� ���������� ������. ���� ���������� ����� (��������, "int ", "while "...). ������ _words - �������, ������� ����� �����.
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
	List _potentialwords = *new List(sizeof(char[Chunck])); //������������� �������. ��� ������ ������ ����������� �� ������ (�����) ��� �� �� ���� (������).
	void PrintAdditionals(); //���������� ������ ������������� ������ ������� ��������.
};

class Type2Machine : public mStateMachine //������� "������, ���� ������ ���������� �������". ������ _words - ���������� �������, ������� ����� ��������� ������� �������.
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
	void AddPerStartWord(char word); //��������� ����� � _permissiblestart.
	void SetPerStartWords(char* words); //��������� ������ ���� � _permissiblestart.
	void ClearAdditionals(); //������� ����. �������� ��� ������� ���� ����� (_permissiblestart).
protected:
	char _permissiblestart[Type23Chunck]; //������ ����, � ������� ������� ������� ��������� ����������.
	void PrintAdditionals(); //���������� ������ ������������� ������ ������� ��������.
};

class Type3Machine : public Type2Machine //���������� ��������� ������� ��� �������� � �����, ������� ����������� "hello!"/'s' � ���������� � ����� string/char � ���������������� ������� (��������� ������������). ����� ��������, ��� ������� ������� �� Type2Machine.
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