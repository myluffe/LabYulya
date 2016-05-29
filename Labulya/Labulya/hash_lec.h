#pragma once

#include "List.h"
#include "lec.h"
#include "hash.h"


class Lexeme_list : public List  //������ �� ������
{
public:
	Lexeme_list();  
	~Lexeme_list(); 

	void     put(lexeme* article); //�������� ������� � ������
	lexeme* find(char* word);      // ����� ������ ������� �� ����� � ������
	int      findpos(char* word);  //����� ������� ������� � ������
	void     del(char* word);      // ������� ������� �� �����
	void     del(lexeme* article); // ������� ������� � ������ ������� 
	void     print_list();         // ������� �� ����� ���� ������ ������
};

class Diction_lexem : public Hash  // ��� ������� ��� ������
{
public:
	Diction_lexem(int _n1, int _n2, int _n3, int _n4, int _n5);
	~Diction_lexem();

	int key1(char* key_word); //����� ��� ��� �������
	int key2(char* key_word); 
	int key3(char* key_word);

	lexeme* find(char* word);  // ����� ������ ������� �� �����
	void del(char* word);  // ������� �� ��� - ������� ������� � ������ ������
	int auto_create(lexeme* lex);// ���������� ������� � ���������� - ���������� ��� ���
	void print_lexems();  // ������� �� ����� ��� ��� - �������
	Lexeme_list* find(int hash); //���������� ���� ������ �� �������
protected:
	lexeme* auto_create(char* word);
};

Diction_lexem VariablesTable = *new Diction_lexem(36, 36, 36, 0, 0);


