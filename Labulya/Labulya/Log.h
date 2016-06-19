#pragma once
#include "string.h"

class Log
{
public:
	Log(char* FileName);
	void Write(char* Out); //Функция вывода памяти
	~Log();
private:
	char* _logfile; //Стандартный файл для вывода ошибок или предупреждений. По умолчанию - консоль.
};

static Log LogFile = *new Log("log.txt");