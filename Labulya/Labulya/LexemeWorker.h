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
	bool Processing(List* lexes); //Вызавается ф-цией MachineWorker().Work после формирования списка из лексем для их последующей обработки (запись переменных в хэш-таблицу и замена этой переменной в коде на ссылку из хэша во всей области видимости этой переменной) и записи в дерево.
	LexemeWorker();
	~LexemeWorker();
protected:
	//Флаг equal в данных функциях при значении true означает, что в expression разрешена операция присвоения (=).
	//Ф-ции типа Is...Expression используют ф-цию WhateverCheck для свои проверок, отправляя в нее нужные параметры.
	bool IsNumberExpression(List* expression, bool equal); //Проверяет, что список из лексем expression явл. выражением с переменными типа INT, FLOAT или DOUBLE и включает только доступные для этих типов операции (результат этих операций тоже должен иметь тип INT, FLOAT или DOUBLE).  
	bool IsBoolExpression(List* expression, bool equal); //Проверяет, что список из лексем expression явл. выражением с переменными типа BOOL и включает только доступные для этого типа операции (результат этих операций тоже должен иметь тип BOOL).  
	bool IsStringExpression(List * expression, bool equal); //Проверяет, что список из лексем expression явл. выражением с переменными типа STRING или CHAR и включает только доступные для этих типов операции (результат этих операций тоже должен иметь тип STRING или CHAR).  

	bool IsNumberExpressionWithBoolOperations(List * expression); //Проверяет, что список из лексем expression явл. выражением с переменными типа INT, FLOAT или DOUBLE и включает только операции, результат которых должен иметь тип BOOL.  
	bool IsStringExpressionWithBoolOperations(List * expression); //Проверяет, что список из лексем expression явл. выражением с переменными типа STRING или CHAR и включает только операции, результат которых должен иметь тип BOOL.  

	//Принимают int pos (Позиция спец. слова или позиция, с которой нужно начинать обработку). Фунции, возвращающие int, возвращают позицию последней, обработанной лекемы, в случае ошибки - pos.
	int CorrectWhile(List* expression, int pos, lexeme* spec, TList* storage); //Из списка лексем expression вычленияет и проверяет на корректность условие (с помощью FuncWithBoolParam) и тело цикла (с помощью InnerExpression) "while", и внедряет лист "TWhile" в дерево storage.
	int CorrectDo(List* expression, int pos, lexeme* spec, TList* storage); //Из списка лексем expression вычленияет и проверяет на корректность тело цикла (с помощью InnerExpression) и условие (с помощью FuncWithBoolParam) "do", и внедряет лист "TDoWhile" в дерево storage.
	int CorrectFor(List* expression, int pos, lexeme* spec, TList* storage); //Из списка лексем expression вычленияет и проверяет на корректность три параметра for (param1 - переменная, param2 - с помощью IsBoolExpression(... ,false), IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations), и тело цикла (с помощью InnerExpression) "for", и внедряет лист "TFor" в дерево storage.
	int CorrectIf(List* expression, int pos, lexeme* spec, TList* storage); //Из списка лексем expression вычленияет и проверяет на корректность условие (с помощью FuncWithBoolParam), тело "if" (с помощью InnerExpression), тело "else" (если есть с помощью InnerExpression) "else", и внедряет в дерево лист "TIf" storage.

	int CorrectShortIfOperation(List* expression, int i, TList* storage); //Из списка лексем expression вычленияет и проверяет на корректность условие (до "?" с помощью IsBoolExpression(... ,false), IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations), тело "if" (после "?", до ":" с помощью InnerExpression), тело "else" (после ":" с помощью InnerExpression) "... ? ... : ...", и внедряет в дерево лист "TTernaryOperator" storage.
	
	int CorrectSpecial(lexeme* spec, int pos, List* expression, TList* storage); //Вычлиняет, засовывает в дерево storage и проверет спец.слово из expression на корректность с помощью CorrectWhile, CorrectDo, CorrectFor, CorrectIf и FuncWithNumberParam, FuncWithTwoNumberParams, FuncWithStringParam, FuncWithBoolParam.
	int CorrectExpression(List* expression, int pos, TList* storage); //Проверяет на корректность и засовывает в дерево storage строку с операциями (без спец слов), при необходимости использует CorrectShortIfOperation

	bool _error = false;

private:
	lexeme* GetMassValues(List* expression, int start, char* type, int* putend, Lexeme_list* dob, int rank, List* sizes); //разбирает выражение от { до } при инициализации массивов и записывате элементы подряд (в память?)
	bool GetValue(Lexeme_list* dob, lexeme* place, List* expression, int pos);
	bool ReInnerFind(List* expression, int* currentpos, int currentstep, int* s, int rank, List* poses, Lexeme_list* dob, int type);

	//pos - позиция лексемы ф-ции (в случае output(...) pos - позиция output в expression)
	//возвращают позицию последней лексемы, при ошибке - pos.
	//spec - указатель на лексему-ф-цию
	//param или param1, param2 - список(ки), в который(ые) записывается(ются) параметр(ы) ф-ции
	int FuncWithNumberParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param); //Из списка expression выделяет параметр, который явл. числовым выражением (с помощью IsNumberExpression(... , false)) функции (например параметр для функции output (...)), записывает результат в список param.
	int FuncWithTwoNumberParams(List* expression, int pos, lexeme* spec, bool finaldevider, List* param1, List* param2); //Из списка expression выделяет параметры, который явл. числовыми выражениями (с помощью IsNumberExpression(... , false)) функции (например параметр для функции max (..., ...)), записывает результат в списки param1, param2.
	int FuncWithStringParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param); //Из списка expression выделяет параметр, который явл. строуовым выражением (с помощью IsStringExpression(... , false)) функции (например параметр для функции output (...)), записывает результат в список param.
	int FuncWithBoolParam(List* expression, int pos, lexeme* spec, bool finaldevider, List* param); //Из списка expression выделяет параметр, который явл. булевым выражением (с помощью IsBoolExpression(... ,false), IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations) функции (например параметр для функции if (...)), записывает результат в список param.

	//Возвращают true в случае успеха
	bool InnerExpression(List* expression, TList* storage); //Функция, работающая с телом циклов. Используется в CorrectWhile, CorrectFor... Представляет тело в виде ветви дерева, записывает ветвь в дерево storage. Если встречает спец. слово, вызывает CorrectSpecial (то есть используется рекурсия, поддерживаются вложеные (в теории до бесконечности) операции). При необходимости использует CorrectShortIfOperation.
	
	bool WhateverCheck(char ** perone, int c1, int * types, int c2, List * expression, bool equal); //Внутренняя ф-ция, которые используют IsNumberExpression, IsBoolExpression, IsStringExpression, IsNumberExpressionWithBoolOperations, IsStringExpressionWithBoolOperations, загружая в неё свои параметры.
} LWorker = *new LexemeWorker();
