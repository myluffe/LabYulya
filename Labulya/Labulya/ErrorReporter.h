#pragma once

#include "string.h"

static class ErrorReporter
{
public:
	ErrorReporter();
	void FReport(FILE*, char* Errortext, int line, int position);
	void WarningReport(FILE*, char* Errortext, int line, int position);
	~ErrorReporter();
};

static FILE* logfile = (FILE*)stdout;

