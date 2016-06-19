#include "stdafx.h"
#include "Log.h"

Log::Log(char* FileName)
{
	fopen_s(&logfile, FileName, "w");
}

void Log::Write(char* Out)
{
	if(logfile != nullptr)
		fprintf_s(logfile, "%s\n", Out);
}

Log::~Log()
{
	fclose(logfile);
}
