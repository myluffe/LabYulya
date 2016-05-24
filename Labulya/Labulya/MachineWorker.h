#pragma once
#include "list.h"
#include "mStateMachine.h"
#include <typeinfo>
#include "FileReader.h"
#include "lec.h"
#include "ErrorReporter.h"

#define MACHINES_COUNT 8

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

	Type3Machine* ssm;

	//ErrorReporter Error = ErrorReporter();
	bool _error = false;

	void UpdateMachines();
	void Addmachine(mStateMachine* machine);
	void NumberCheck(int line);
	void Hooker(char* buffer);
	bool HooksCheck(mFileReader* f);

	mStateMachine* _machines[MACHINES_COUNT];
	int _count;
	int _curmachine;

	//for hooks
	int _hooks = 0;
	int _circlehooks = 0;
	int _squarehooks = 0;
	//

	mStateMachine* _currentaut;
};

