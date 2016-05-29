#include "stdafx.h"
#include "heap.h"
#include "list.h"
#include "hash.h"
#include "hash_lec.h"



int Diction_lexem::key1(char* key_word)
{
	int f = key_word[0] - 'a';
	if (f<33 && f>0) return f;
	else             return 0;
}
int Diction_lexem::key2(char* key_word)
{
	int f = key_word[1] - 'a';
	if (f<33 && f>0) return f;
	if (f >= -49 && f <= -40) return f + 75;
	return 0;
}
int Diction_lexem::key3(char* key_word)
{
	int f = key_word[2] - 'a';
	if (f<33 && f>0) return f;
	if (f >= -49 && f <= -40) return f + 75;
	return 0;
}

Diction_lexem::Diction_lexem(int _n1, int _n2, int _n3, int _n4, int _n5) 
{
	n1 = abs(_n1) % MAXRANGE + 1;
	n2 = abs(_n2) % MAXRANGE + 1;
	n3 = abs(_n3) % MAXRANGE + 1;
	n4 = abs(_n4) % MAXRANGE + 1;
	n5 = abs(_n5) % MAXRANGE + 1;
	table = new List(sizeof(Lexeme_list));
	int k = n1*n2*n3*n4*n5;
	printf("+");
	for (int i = 0; i < k; i++)
	{
		table->add(new Lexeme_list());
	}
};

Diction_lexem::~Diction_lexem()
{
	List* list;
	for (int i = 0; i < table->count(); i++)
	{
		list = (List*)table->get(i);
		if (list->count() > 0)
		{
			for (int j = 0; j < list->count(); j++)
			{
				lexeme* a = (lexeme*)list->get(j);
				((Lexeme_list*)list)->del(a);
			}
		}
	}
};

Lexeme_list::Lexeme_list() :List(sizeof(lexeme))
{
	;
}

Lexeme_list::~Lexeme_list()
{
	List::~List();
}

lexeme* Lexeme_list::find(char*word)
{
	lexeme * now = new lexeme("","","",0,0,0); // конструктор!

	for (int i = 0; i < count(); i++)
	{
		now = (lexeme*)get(i);
		if (!strcmp(now->Name(), word))
		{
			return now;
		}
	}
	return nullptr;
}

int Lexeme_list::findpos(char* word)
{
	lexeme * now = new lexeme("","","",0,0,0);

	for (int i = 0; i < count(); i++)
	{
		now = (lexeme*)get(i);
		if (!strcmp(now->Name(), word))
		{
			return i;
		}

	}
	return -1;
}

void Lexeme_list::put(lexeme* article)
{
	if (article != nullptr){

		lexeme* art = new lexeme(article->Name(), article->Type(), article->Data(), article->Line(), article->Start_Position(), article->Priority());
		add(art);
		heap.free_mem(art);
	}
}

void Lexeme_list::del(char* word)
{
	int a = findpos(word);
	if (a != -1)
	{
		heap.free_mem((lexeme*)get(a));
		
		remove(a);
	}
}
void Lexeme_list::del(lexeme* article)
{
	del(article->Name());
}

void Lexeme_list::print_list()
{
	for (int j = 0; j < count(); j++)
	{
		lexeme* a = (lexeme*)get(j);
		printf("Place= %d ||", j);
		a->Print();
		
	}
	if (count() == 0)
		printf("Empty list\n");
}

lexeme* Diction_lexem::find(char* word)
{
	Lexeme_list* list = (Lexeme_list*)find_list(word);
	return list->find(word);

}

lexeme* Diction_lexem::auto_create(char*word)
{
	lexeme* fin = find(word);
	if (fin != nullptr)
		return fin;
	else
	{   
		fin = new lexeme(word, "", "", 0, 0, 0);
		Lexeme_list* list = (Lexeme_list*)find_list(word);
		if (!list)
		{
			list = (Lexeme_list*)heap.get_mem(sizeof(Lexeme_list));
		}
		list->put(fin);
	}
	heap.free_mem(fin);
    return find(word);
}

int Diction_lexem::auto_create(lexeme* lex)
{

	lexeme* fin = find(lex->Name());
	if (fin != nullptr)
		return combine_keys(lex->Name());
	else
	{
		fin = new lexeme(lex->Name(), lex->Type(), lex->Data(), lex->Line(), lex->Start_Position(), lex->Priority());
		Lexeme_list* list = (Lexeme_list*)find_list(lex->Name());
		if (!list)
		{
			list = (Lexeme_list*)heap.get_mem(sizeof(Lexeme_list));
		}
		list->put(fin);
	}
	//heap.free_mem(fin);
	return combine_keys(lex->Name());
}

void Diction_lexem::print_lexems()
{

	List* list;
	for (int i = 0; i < table->count(); i++)
	{
		list = (List*)table->get(i);
		if (list->count() > 0)
		{   
			printf("Hash= %d ", i);
			((Lexeme_list*)list)->print_list();
		}
	}
}

void Diction_lexem::del(char*word) 
{
	Lexeme_list* list = (Lexeme_list*)find_list(word);
	if (list)
	{
		list->del(word);
	}
}

Lexeme_list* Diction_lexem::find(int hash)
{
	return (Lexeme_list*)(table->get(hash));
}