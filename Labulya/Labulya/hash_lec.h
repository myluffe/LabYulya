#pragma once

#include "List.h"
#include "lec.h"
#include "hash.h"


class Lexeme_list : public List  //список из лексем
{
public:
	Lexeme_list();  
	~Lexeme_list(); 

	void     put(lexeme* article); //добавить лексему в список
	lexeme* find(char* word);      // найти нужную лексему по имени в списке
	int      findpos(char* word);  //найти позицию лексемы в списке
	void     del(char* word);      // удалить лексему по имени
	void     del(lexeme* article); // удалить лексему с именем лексемы 
	void     print_list();         // вывести на экран весь список лексем
};

class Diction_lexem : public Hash  // хэш таблица для лексем
{
public:
	Diction_lexem(int _n1, int _n2, int _n3, int _n4, int _n5);
	~Diction_lexem();

	int key1(char* key_word); //ключи для хэш таблицы
	int key2(char* key_word); 
	int key3(char* key_word);

	lexeme* find(char* word);  // найти нужную лексему по имени
	void del(char* word);  // Удалить из хэш - таблицы лексему с данным именем
	int auto_create(lexeme* lex);// добавление лексемы в хэштаблицу - возвращает хэш код
	void print_lexems();  // вывести на экран всю хэш - таблицу
	Lexeme_list* find(int hash); //возвращает лист лексем по хэшкоду
protected:
	lexeme* auto_create(char* word);
};

Diction_lexem VariablesTable = *new Diction_lexem(36, 36, 36, 0, 0);


