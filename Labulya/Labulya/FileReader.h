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
	bool EndFile();
	mFileReader(char* filename);
	int CurrentLine();
	int CurrentPosition();
	//char ReadChar(int position); // _currentposition не меняется
	char* ReadNextLine();
	~mFileReader();
};
