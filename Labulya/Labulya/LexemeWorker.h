#pragma once

#include "stdafx.h"
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

