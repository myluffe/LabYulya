#pragma once

#include "string.h"

static class ErrorReporter
{
public:
	ErrorReporter();
	void FReport(FILE*, char* Errortext, int line, int position); //Функция вывода ошибки в файл: текст ошибки; строка, в которой произошла ошибка; позиция,  в которой произошла ошибка;
	void WarningReport(FILE*, char* Errortext, int line, int position); //Функция вывода предупреждения в файл: текст ошибки; строка, в которой произошла ошибка; позиция,  в которой произошла ошибка;
	~ErrorReporter();
}errorReporter = *new ErrorReporter();

static FILE* logfile = (FILE*)stdout; //Стандартный файл для вывода ошибок или предупреждений. По умолчанию - консоль.

