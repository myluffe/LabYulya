#pragma once

#define SPECIALWORD 900
#define TYPE 901
#define OPERATION 902
#define VARIABLE 903
#define DEVIDER 904
#define NUMBER 905

#define BYNARYOPERATION 910
#define UNARYOPERATION 911
#define TERNARYOPERATION 912

#define INT 102
#define DOUBLE 100
#define FLOAT 101
#define BOOL 103

#define CHAR 301
#define STRING 300

static int GetType(char * stype)
{
	if (strcmp("int ", stype) == 0)
		return INT;
	if (strcmp("double ", stype) == 0)
		return DOUBLE;
	if (strcmp("float ", stype) == 0)
		return FLOAT;
	if (strcmp("bool ", stype) == 0)
		return BOOL;

	if (strcmp("char ", stype) == 0)
		return CHAR;
	if (strcmp("string ", stype) == 0)
		return STRING;

	if (strcmp("Special Word", stype) == 0)
		return SPECIALWORD;
	if (strcmp("Type", stype) == 0)
		return TYPE;
	if (strcmp("Operation", stype) == 0)
		return OPERATION;
	if (strcmp("Variable", stype) == 0)
		return VARIABLE;
	if (strcmp("Devider", stype) == 0)
		return DEVIDER;
	if (strcmp("Number", stype) == 0)
		return NUMBER;

	return 0;
}

static bool IsCastable(int type1, int totype2)
{
	if ((type1 == INT || type1 == DOUBLE || type1 == FLOAT || type1 == BOOL) && (totype2 == INT || totype2 == DOUBLE || totype2 == FLOAT || totype2 == BOOL))
	{
		if (totype2 <= type1)
			return true;
	}
	if ((type1 == CHAR || type1 == STRING) && (totype2 == CHAR || totype2 == STRING))
	{
		if (totype2 <= type1)
			return true;
	}
	return false;
}