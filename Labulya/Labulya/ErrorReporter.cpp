#include "stdafx.h"
#include "ErrorReporter.h"

ErrorReporter::ErrorReporter()
{
}

void ErrorReporter::FReport(FILE* f, char* Errortext, int line, int position)
{
	fprintf_s(f, "Error: %s; in line: %d; in position: %d;\n", Errortext, line, position);
}

ErrorReporter::~ErrorReporter()
{
}
