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

#define CHAR 300
#define STRING 301

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