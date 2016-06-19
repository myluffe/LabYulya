#pragma once
#include "string.h"

static class Log
{
public:
	Log();
	void Write(char* Out); //Функция вывода памяти
	~Log();
}LogFile = Log();

static char LogFileName[] = "log.txt"; //Стандартный файл для вывода ошибок или предупреждений. По умолчанию - консоль.
