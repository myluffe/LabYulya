#pragma once

#include "stdafx.h"
#include "list.h"
#include "hash_lec.h"

static class LexemeWorker
{
public:
	void Processing(List* lexes);
	LexemeWorker();
	~LexemeWorker();
};

