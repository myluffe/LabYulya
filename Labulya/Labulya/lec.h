#pragma once

#include "string.h"
#include "List.h"

class lexeme
{
private:
	char* _name; //Назвоние лексемы. Или имя переменной
	char* _data; //Значение лексемы, считанные из файла. В случае переменной заменяются на её значение.
	int _type; //Тип лексемы. В случае переменной или константы сначала внутренний (Number или Variable) затем по типу значения.
	int _line; //Номер строки, на которой находится лексема в обрабатываемом файле.
	int _startposition; //Позиция лексемы относительно начала сторки.
	int _priority; //Приоритет. По умолчанию 0, далее, если лексема явл. операцией, в спец. функции определяется её приоритет по таблице: http://cppstudio.com/post/302/
	//Скобки "(" и ")" имеют приоритет 100.
	int _rank = 0; //степень массива (если не массив, то 0).
protected:
	lexeme();
public: 
	lexeme(char* name, char* type, char* data, int line, int startposition, int priority);
	lexeme(char* name, int type, char* data, int line, int startposition, int priority);
	void ToMass(lexeme* name, int type, lexeme* values, int rank, List* sizes);
	~lexeme();

	//Получение полей лексемы:
	char* Data() { return _data; }
	char* Name() { return _name; }
	int Type() { return _type; }
	int Line() { return _line; }
	int Start_Position() { return _startposition; }
	int Priority() { return _priority; }

	bool GetNumber(double* value); //Получает значение лексемы (поле Data). С конвертацией входного параметра в строку с помощью Parser
	bool SetNumber(double value); //Менет значение лексемы (поле Data).

	void DataChange(char* newdata); //Менет значение лексемы (поле Data).
	void Print();

	//поля для массивов
	lexeme* Values = nullptr; //ссылка на первый элемент?
	int Rank() { return _rank; }
	List* Sizes = nullptr;
};