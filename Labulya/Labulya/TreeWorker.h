#pragma once
#include "string.h"
#include "tree.h"
#include "LexemeWorker.h"
#include "ErrorReporter.h"

static class TreeWorker
{
public:
	TreeWorker();
	~TreeWorker();
	static TNode* GetTNode(List* lexes, int start, int finish);
	static TNode* GetTNode1(List* lexes, int start, int finish);
	static TNode* GetTNode2(List* lexes, int start, int finish);
	static bool PreprocessingBrackets(List* lexes, int start, int finish);
	static bool PreprocessingElemMas(List* lexes, int start, int finish);
	static int GetLexemePositionWithMinimalPriority(List * lexes, int start, int finish);
	static int GetLexemePositionWithMaximalPriority(List * lexes, int start, int finish);
private:
	struct DoTNode
	{
		int start;
		int finish;
		int type;
		TNode* node;
	};
	
}treeWorker = *new TreeWorker();

static List* nodes;