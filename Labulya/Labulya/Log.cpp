#include "stdafx.h"
#include "Log.h"

Log::Log(char* FileName)
{
	_logfile = new char[(int)strlen(FileName) + 1];
	strcpy_s(_logfile, (int)strlen(FileName) + 1, FileName);
}

void Log::Write(char* Out)
{
	if (_logfile != nullptr)
	{
		FILE* f;
		fopen_s(&f, _logfile, "w");
		if (f != nullptr)
			fprintf_s(logfile, "%s\n", Out);
		fclose(f);
	}
}

Log::~Log()
{
}
