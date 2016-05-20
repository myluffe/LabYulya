#pragma once
#include "list.h"
#include "mStateMachine.h"
#include <typeinfo>
#include "FileReader.h"
#include "lec.h"
#include "ErrorReporter.h"

#define MACHINES_COUNT 6

static class MachineWorker
{
public:
	MachineWorker();
	~MachineWorker();
	int Work(char* filename);
	int Count();
private:
	Type1Machine* sm;
	Type1Machine* om;
	Type1Machine* dm;
	Type1Machine* tm;

	Type2Machine* nm;
	Type2Machine* vm;

	ErrorReporter Error = ErrorReporter();
	bool _error = false;

	void UpdateMachines(mStateMachine* currentaut);
	void _addmachine(mStateMachine* machine);

	int NumberCheck(mStateMachine* curr, int line);
	mStateMachine* _machines[MACHINES_COUNT];
	int _count;
	int machinecount = 0;
};

