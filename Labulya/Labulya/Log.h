#pragma once
#include "string.h"

static class Log
{
public:
	Log(char* FileName = "log.txt");
	void Write(char* Out); //Функция вывода памяти
	~Log();
private:
	FILE* logfile; //Стандартный файл для вывода ошибок или предупреждений. По умолчанию - консоль.
}LogFile = *new Log();