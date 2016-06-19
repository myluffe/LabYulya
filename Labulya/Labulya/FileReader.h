#pragma once

#include "string.h"

#define String 100
#define FileName 50

using namespace std; 

class mFileReader
{
private:
	char _lastreadstring[String];
	char _filename[FileName];
	int _currentline;
	int _currentposiotion;
	FILE *file;
	bool _end = false;
public:
	bool EndFile(); //возвращает true, если файл зауончился
	mFileReader(char* filename); //конструктор принимает имя файла для чтения
	int CurrentLine(); //возвращает номер последней прочитанной строки
	int CurrentPosition(); //возвращает номер позиции, на которой остановилось чтение, не зависит от строки
	char* ReadNextLine(); //читает следующую строку из файла, возвращает указатель на неё
	~mFileReader();
};