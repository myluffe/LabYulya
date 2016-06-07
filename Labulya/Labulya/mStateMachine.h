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

	char* Buffer(); //Возвращает _buffer.
	char* MachineName(); //Возвращает имя машины.

	bool IsFinished(); //Возвращает _isFinished.
	bool IsError(); //Возвращает _isError.
	bool IsStart(); //Возвращает _start.

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

	virtual void ClearAdditionals() = 0; //Дополнительная очистка для наследников данного класса. Используется в деструкторах наследников.

	virtual void PrintAdditionals() = 0; //Отладочная печать для наследников данного класса.

	char _machineName[Chunck]; //Имя машины.
	char* _buffer; //Буффер, в который записываются символы при формировании лексемы. В случае успеха содежит кусок текста из обрабатываемого файла, соответсвующий лексеме.

	bool _isFinished; //true, если автомат успешно сформировал лексему.
	bool _isError; //true, если данный автомат не смог сформировать лексему.
	bool _start; //true, если мы начали работу с данным автоматом.

	bool _checkError;
	
	char _currentLexemName[Chunck]; //Имя формируемой лексемы
	int _currentLexemeType; //Текущий тип формируемой лексемы
	int _originaltype; //Тип формируемых данной машиной лексем по умолчанию 

	int _currentLexemePosition; //Стартовая позиция, относительно начала строки, лексемы, которая формируется в данный момент.
	int _curlexline; //Номер строки лексемы, которая формируется в данный момент.
	int _step; //Шаг. Позиция _buffer, в которую будет записываться следующий символ при формировании лексемы.
	
	int _chunckcount; //Количество кусков, которые вмещает в себя _buffer. По умолчанию _chunckcount = 1, а _buffer[chunck]. При переплнении создается новый буффер на один chunk больше предыдущего, в который копируется старый _buffer, _chunckcount увеличивается на еденицу. Проще говоря, это вспомогательный счетчик для динамического выделения памяти по кускам для _buffer.
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