#pragma once
#include "list.h"
#include "mStateMachine.h"
#include <typeinfo>
#include "FileReader.h"
#include "lec.h"
#include "ErrorReporter.h"

#define MACHINES_COUNT 7

static class MachineWorker
{
public:
	MachineWorker();
	~MachineWorker();
	int Work(char* filename, List* lexes);
	int Count();
private:
	Type1Machine* sm;
	Type1Machine* om;
	Type1Machine* om2;
	Type1Machine* dm;
	Type1Machine* tm;

	Type2Machine* nm;
	Type2Machine* vm;

	ErrorReporter Error = ErrorReporter();
	bool _error = false;

	void UpdateMachines();
	void _addmachine(mStateMachine* machine);

	int NumberCheck(mStateMachine* curr, int line);
	mStateMachine* _machines[MACHINES_COUNT];
	int _count;
	int _curmachine;

	mStateMachine* _currentaut;
};

