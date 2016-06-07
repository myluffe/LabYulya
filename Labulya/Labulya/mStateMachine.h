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

	char* CurrentLexName(); //Возвращает _machineName.
	int CurrentLexType(); //Возвращает _machineName.
	void ChangeType(int stype); //Смена типа лексемы, которая формируется. Используется в MachineWorker().NumberCheck для смены типа с DOUBLE на INT или FLOAT.

	//Тестовая отладка
	void PrintMachine();

	int CurrentLexPos(); //Возвращает _currentLexemePosition.
	int CurrentLexLine(); //Возвращает _curlexline.
	int Priority = 100; //Приоритет лексемы. У операций меняется в MachineWorker().GetPriority.
	int ChunkCount(); //Возвращает _chunckcount.

	void AddWord(void * word); //Добавляет слово в список слова _words.
	virtual void EnterChar(char ch, int pos, int line) = 0; //Функция "Послать символ в машину". В автомат посылаются символы до ошибки или до успешного формирования лексемы.
	virtual void CheckStart(char ch) = 0; //При первом вхождении в машину, проверят по первому символу, нужно ли этой машине пытаться распознать лексему.
	void UpdateStatus(); //Сброс настроек автомата.
protected:
	List* _words = new List(sizeof(char[Chunck])); //Слова автомата. Используются по разному в наследниках.

	virtual void ClearAdditionals() = 0; //Дополнительная очистка для наследников данного класса. Используется в деструкторах наследников.

	virtual void PrintAdditionals() = 0; //Отладочная печать для наследников данного класса.

	char _machineName[Chunck]; //Имя машины.
	char* _buffer; //Буффер, в который записываются символы при формировании лексемы. В случае успеха содежит кусок текста из обрабатываемого файла, соответсвующий лексеме.

	bool _isFinished; //true, если автомат успешно сформировал лексему.
	bool _isError; //true, если данный автомат не смог сформировать лексему.
	bool _start; //true, если мы начали работу с данным автоматом.
	
	char _currentLexemName[Chunck]; //Имя формируемой лексемы
	int _currentLexemeType; //Текущий тип формируемой лексемы
	int _originaltype; //Тип формируемых данной машиной лексем по умолчанию 

	int _currentLexemePosition; //Стартовая позиция, относительно начала строки, лексемы, которая формируется в данный момент.
	int _curlexline; //Номер строки лексемы, которая формируется в данный момент.
	int _step; //Шаг. Позиция _buffer, в которую будет записываться следующий символ при формировании лексемы.
	
	int _chunckcount; //Количество кусков, которые вмещает в себя _buffer. По умолчанию _chunckcount = 1, а _buffer[chunck]. При переплнении создается новый буффер на один chunk больше предыдущего, в который копируется старый _buffer, _chunckcount увеличивается на еденицу. Проще говоря, это вспомогательный счетчик для динамического выделения памяти по кускам для _buffer.
};

class Type1Machine : public mStateMachine //Автомат дословного поиска. Ищет конкретные слова (например, "int ", "while "...). Список _words - лексемы, которые нужно найти.
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
	List _potentialwords = *new List(sizeof(char[Chunck])); //Потенциальные лексемы. При работе список уменьшается до одного (успех) или до ну нуля (провал).
	void PrintAdditionals(); //Отладочная печать специфических данных данного автомата.
};

class Type2Machine : public mStateMachine //Автомат "хорошо, пока только допустимые символы". Список _words - допустимые символы, которые может содержать искомая лексема.
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
	void AddPerStartWord(char word); //Добаление слова в _permissiblestart.
	void SetPerStartWords(char* words); //Добаление списка слов в _permissiblestart.
	void ClearAdditionals(); //Очистка спец. контента для данного типа машин (_permissiblestart).
protected:
	char _permissiblestart[Type23Chunck]; //Список слов, с которых искомой лексеме разрешено начинаться.
	void PrintAdditionals(); //Отладочная печать специфических данных данного автомата.
};

class Type3Machine : public Type2Machine //Специально созданный автомат для символов и строк, который преобразует "hello!"/'s' в переменную с типом string/char и соответствующими данными (проверяет корректность). Стоит заметить, что автомат основан на Type2Machine.
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