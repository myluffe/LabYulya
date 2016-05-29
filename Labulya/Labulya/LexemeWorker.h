#pragma once

#include "list.h"
#include "hash_lec.h"
#include "list_adv.h"

static class LexemeWorker
{
public:
	bool Processing(List* lexes);
	LexemeWorker();
	~LexemeWorker();
};

static LexemeWorker LWorker = *new LexemeWorker();

