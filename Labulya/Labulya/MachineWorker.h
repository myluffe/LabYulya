#pragma once

#include "list.h"
#include "mStateMachine.h"
#include <typeinfo>
#include "FileReader.h"
#include "lec.h"
#include "ErrorReporter.h"

#define MACHINES_COUNT 9

static class MachineWorker
{
public:
	MachineWorker();
	~MachineWorker();
	int Work(char* filename, List* lexes); //Запуск процесса обработки файла (по пути filename), результат записывается в список из лексем (lexeme) lexes.
	int Count(); //Получение списка машин.
private:
	//Машины:
	Type1Machine* sm;
	Type1Machine* om;
	Type1Machine* om2;
	Type1Machine* dm;
	Type1Machine* tm;
	Type1Machine* bm;

	Type2Machine* nm;
	Type2Machine* vm;

	Type3Machine* ssm;

	//ErrorReporter Error = errorReporter;
	bool _error = false; //Внутренний флаг, сигнализирующий об ошибке.

	void GetOperationPriority(mStateMachine* machine); //Функция для смены в автомате machine приоритета. Используется для операций и круглых скобок. Применяется после формирование лексемы, но до добавления в список лексем.

	void UpdateMachines(); //Возвращение текущего автомата к исходному состоянию. Установка первого автомата, как текущего (возвращение к первому автоиату списка). Используется после выделения лексемы и её записи в список.
	void Addmachine(mStateMachine* machine); //Добавление автомата в список (массив указателей _machines) автоматов. Используется в конструкторе класса.
	
	void NumberCheck(int line); //Проверка лексемы-константы (Number) на корректность синтаксиса и определения её типа (INT, FLOAT или DOUBLE).
	void Hooker(char* buffer); //Ф-ция подсчета скобок ("(", ")", "{", "}", "[", "]"). Ругается, если встречает закрывающуюся скобку, которая не открывалась еще. Вызывается при чтении строки. 
	bool HooksCheck(mFileReader* f); //Ф-ция, проверяющая счетчики скобок. Ругается при неверных значениях (незакрытые скобки). Вызывается в конце чтения файла.

	mStateMachine* _machines[MACHINES_COUNT]; //Массив из указателей на машины, для удобного перехода от автомата к автомату при попытке выделить лексему и возвращения к первому в случае успеха.
	int _count; //Кол-во машин.
	int _curmachine; //Индекс текущей машины.

	//Счетчики скобок
	int _hooks = 0; // "{", "}"
	int _circlehooks = 0; // "(", ")"
	int _squarehooks = 0; // "[", "]"

	mStateMachine* _currentaut; //Указатель на текущую машину. Используется для быстрого доступа к текущей машине из _machines.
};