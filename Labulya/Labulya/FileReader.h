#pragma once
#include "string.h"

using namespace std;


class mFileReader
{
private:
	char _lastreadstring[100];
	char _filename[50];
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
