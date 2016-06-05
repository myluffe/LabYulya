#pragma once
#include "string.h"
#include "tree.h"
#include "LexemeWorker.h"
#include "ErrorReporter.h"

static class TreeWorker
{
public:
	static bool DoTree(List* lexes);
	static TNode* GetTNode(List* lexes, int start, int finish);
}treeWorker = *new TreeWorker();