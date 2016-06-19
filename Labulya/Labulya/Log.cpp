#include "stdafx.h"
#include "Log.h"

Log::Log()
{
}

void Log::Write(char* Out)
{
	if (LogFileName != nullptr)
	{
		FILE* f;
		if (fopen_s(&f, LogFileName, "w+") == 0)
			fprintf_s(f, "%s\n", Out);
		fclose(f);
	}
}

Log::~Log()
{
}
