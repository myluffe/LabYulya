#pragma once

#include "list.h"
#include "hash_lec.h"
#include "list_adv.h"
#include "Parser.h"
#include "ErrorReporter.h"
#include "TreeWorker.h"

static class LexemeWorker
{
public:
	bool Processing(List* lexes); //���������� �-���� MachineWorker().Work ����� ������������ ������ �� ������ ��� �� ����������� ��������� (������ ���������� � ���-������� � ������ ���� ���������� � ���� �� ������ �� ���� �� ���� ������� ��������� ���� ����������) � ������ � ������.
	LexemeWorker();
	~LexemeWorker();
protected:
	//���� equal � ������ �������� ��� �������� true ��������, ��� � expression ��������� �������� ���������� (=).
	//�-��� ���� Is...Expression ���������� �-��� WhateverCheck ��� ���� ��������, ��������� � ��� ������ ���������.
	bool IsNumberExpression(List* expression, bool equal); //���������, ��� ������ �� ������ expression ���. ���������� � ����������� ���� INT, FLOAT ��� DOUBLE � �������� ������ ��������� ��� ���� ����� �������� (��������� ���� �������� ���� ������ ����� ��� INT, FLOAT ��� DOUBLE).  
	bool IsBoolExpression(List* expression, bool equal); //���������, ��� ������ �� ������ expression ���. ���������� � ����������� ���� BOOL � �������� ������ ��������� ��� ����� ���� �������� (��������� ���� �������� ���� ������ ����� ��� BOOL).  
	bool IsStringExpression(List * expression, bool equal); //���������, ��� ������ �� ������ expression ���. ���������� � ����������� ���� STRING ��� CHAR � �������� ������ ��������� ��� ���� ����� �������� (��������� ���� �������� ���� ������ ����� ��� STRING ��� CHAR).  

	bool IsNumberExpressionWithBoolOperations(List * expression); //���������, ��� ������ �� ������ expression ���. ���������� � ����������� ���� INT, FLOAT ��� DOUBLE � �������� ������ ��������, ��������� ������� ������ ����� ��� BOOL.  
	bool IsStringExpressionWithBoolOperations(List * expression); //���������, ��� ������ �� ������ expression ���. ���������� � ����������� ���� STRING ��� CHAR � �������� ������ ��������, ��������� ������� ������ ����� ��� BOOL.  

	//��������� int pos (������� ����. ����� ��� �������, � ������� ����� �������� ���������). ������, ������������ int, ���������� ������� ���������, ������������ ������, � ������ ������ - pos.
	int CorrectWhile(List* expression, int pos, lexeme* spec, TList* storage); //�� ������ ������ expression ���������� � ��������� �� ������������ ������� (� ������� FuncWithBoolParam) � ���� ����� (� ������� InnerExpression) "while", � �������� ���� "TWhile" � ������ storage.
	int CorrectDo(List* expression, int pos, lexeme* spec, TList* storage); //�� ������ ������ expression ���������� � ��������� �� ������������ ���� ����� (� ������� InnerExpression) � ������� (� ������� FuncWithBoolParam) "do", � �������� ���� "TDoWhile" � ������ storage.
	int CorrectFor(List* expression, int pos, lexeme* spec, TList* storage); //�� ������ ������ expression ���������� � ��������� �� ������������ ��� ��������� for (param1 - ����������, param2 - � ������� IsBoolExpression(... ,false), IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations), � ���� ����� (� ������� InnerExpression) "for", � �������� ���� "TFor" � ������ storage.
	int CorrectIf(List* expression, int pos, lexeme* spec, TList* storage); //�� ������ ������ expression ���������� � ��������� �� ������������ ������� (� ������� FuncWithBoolParam), ���� "if" (� ������� InnerExpression), ���� "else" (���� ���� � ������� InnerExpression) "else", � �������� � ������ ���� "TIf" storage.

	int CorrectShortIfOperation(List* expression, int i, TList* storage); //�� ������ ������ expression ���������� � ��������� �� ������������ ������� (�� "?" � ������� IsBoolExpression(... ,false), IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations), ���� "if" (����� "?", �� ":" � ������� InnerExpression), ���� "else" (����� ":" � ������� InnerExpression) "... ? ... : ...", � �������� � ������ ���� "TTernaryOperator" storage.
	
	int CorrectSpecial(lexeme* spec, int pos, List* expression, TList* storage); //���������, ���������� � ������ storage � �������� ����.����� �� expression �� ������������ � ������� CorrectWhile, CorrectDo, CorrectFor, CorrectIf � FuncWithNumberParam, FuncWithTwoNumberParams, FuncWithStringParam, FuncWithBoolParam.
	int CorrectExpression(List* expression, int pos, TList* storage); //��������� �� ������������ � ���������� � ������ storage ������ � ���������� (��� ���� ����), ��� ������������� ���������� CorrectShortIfOperation

	bool _error = false;

private:
	lexeme* GetMassValues(List* expression, int start, char* type, int* putend, Lexeme_list* dob, int rank, List* sizes); //��������� ��������� �� { �� } ��� ������������� �������� � ���������� �������� ������ (� ������?)
	bool GetValue(Lexeme_list* dob, lexeme* place, List* expression, int pos);
	bool ReInnerFind(List* expression, int* currentpos, int currentstep, int* s, int rank, List* poses, Lexeme_list* dob, int type);

	//pos - ������� ������� �-��� (� ������ output(...) pos - ������� output � expression)
	//���������� ������� ��������� �������, ��� ������ - pos.
	//spec - ��������� �� �������-�-���
	//param ��� param1, param2 - ������(��), � �������(��) ������������(����) ��������(�) �-���
	int FuncWithNumberParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param); //�� ������ expression �������� ��������, ������� ���. �������� ���������� (� ������� IsNumberExpression(... , false)) ������� (�������� �������� ��� ������� output (...)), ���������� ��������� � ������ param.
	int FuncWithTwoNumberParams(List* expression, int pos, lexeme* spec, bool finaldevider, List* param1, List* param2); //�� ������ expression �������� ���������, ������� ���. ��������� ����������� (� ������� IsNumberExpression(... , false)) ������� (�������� �������� ��� ������� max (..., ...)), ���������� ��������� � ������ param1, param2.
	int FuncWithStringParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param); //�� ������ expression �������� ��������, ������� ���. ��������� ���������� (� ������� IsStringExpression(... , false)) ������� (�������� �������� ��� ������� output (...)), ���������� ��������� � ������ param.
	int FuncWithBoolParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param); //�� ������ expression �������� ��������, ������� ���. ������� ���������� (� ������� IsBoolExpression(... ,false), IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations) ������� (�������� �������� ��� ������� if (...)), ���������� ��������� � ������ param.

	//���������� true � ������ ������
	bool InnerExpression(List* expression, TList* storage); //�������, ���������� � ����� ������. ������������ � CorrectWhile, CorrectFor... ������������ ���� � ���� ����� ������, ���������� ����� � ������ storage. ���� ��������� ����. �����, �������� CorrectSpecial (�� ���� ������������ ��������, �������������� �������� (� ������ �� �������������) ��������). ��� ������������� ���������� CorrectShortIfOperation.
	
	bool WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression, bool equal); //���������� �-���, ������� ���������� IsNumberExpression, IsBoolExpression, IsStringExpression, IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations, �������� � �� ���� ���������.
} LWorker = *new LexemeWorker();
