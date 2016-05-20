#include "stdafx.h"
#include "FileReader.h"

bool mFileReader::EndFile()
{
	return _end;
}

mFileReader::mFileReader(char* filename)
{
	strcpy_s(_filename, filename);
	_lastreadstring[0] = '\0';
	_currentline = 0;
	_currentposiotion = 0;
	if (fopen_s(&file, _filename, "r") != 0)
		perror("The file wasn't opened\n");
}
int mFileReader::CurrentLine()
{
	return _currentline;
}
int mFileReader::CurrentPosition()
{
	return _currentposiotion;
}
/*
char mFileReader::ReadChar(int position) // _currentposition не меняется
{
	fseek(file, position, SEEK_SET);
	return fgetc(file);
}
*/
char* mFileReader::ReadNextLine()
{
	char ch = 0;
	int i = 0;
	fseek(file, _currentposiotion, SEEK_SET);
	while (ch != EOF)
	{
		ch = fgetc(file);
		_currentposiotion++;
		if (ch == '\n')
		{
			_currentposiotion++;
			break;
		}
		_lastreadstring[i++] = ch;
	}
	if (ch == EOF)
		_end = true;
	_lastreadstring[i] = '\0';
	_currentline++;
	return _lastreadstring;
}

mFileReader::~mFileReader()
{
	fclose(file);
}
