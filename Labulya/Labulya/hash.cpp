#include "stdafx.h"
#include "heap.h"
#include "list.h"
#include "hash.h"




int Diction::key1(char* key_word)
{
	int f = key_word[0] - 'a';
	if (f<33 && f>0) return f;
	else             return 0;
}
int Diction::key2(char* key_word)
{
	int f = key_word[1] - 'a';
	if (f<33 && f>0) return f;
	else             return 0;
}


Hash::Hash(int _n1, int _n2, int _n3, int _n4, int _n5)
{
	n1 = abs(_n1) % MAXRANGE + 1;
	n2 = abs(_n2) % MAXRANGE + 1;
	n3 = abs(_n3) % MAXRANGE + 1;
	n4 = abs(_n4) % MAXRANGE + 1;
	n5 = abs(_n5) % MAXRANGE + 1;
	table = new List(sizeof(Diction_list));
	int k = n1*n2*n3*n4*n5;
	printf("+");
	for (int i = 0; i < k; i++)
	{
		table->add(new Diction_list());
	}
	/*table = (List**)heap.get_mem(n1*n2*n3*n4*n5*sizeof(List*));
	memset(table, 0, n1*n2*n3*n4*n5*sizeof(List*));*/
};


List* Hash::find_list(char* key_word)
{   
	int a = combine_keys(key_word);	
	return (List*)table->get(a);	
	
}

void Hash::print_hash()
{
	
	List* list;
	for (int i = 0; i < table->count(); i++)
	{
		list = (List*)table->get(i);
		if (list->count() > 0)
		{
			for (int j = 0; j < list->count(); j++)
			{ 
				Article* a = (Article*)list->get(j);
				printf("List %d place %d Article: %s  -  %s  \n", i , j , a->word, a-> description);
			}
		}
	}
}


Diction::Diction(int _n1, int _n2, int _n3, int _n4, int _n5) : Hash(_n1, _n2, _n3, _n4, _n5)
{
	;
};
 Hash::~Hash()
{ 
	
	for (int i = 0; i < table->count(); i++)
	{
		((List*)table->get(i))->~List();
	}
	table->~List();
};

Diction::~Diction() 
{
	List* list;
	for (int i = 0; i < table->count(); i++)
	{
		list = (List*)table->get(i);
		if (list->count() > 0)
		{
			for (int j = 0; j < list->count(); j++)
			{
				Article* a = (Article*)list->get(j);
				((Diction_list* )list)->del(a);
			}
		}
	}
};

Diction_list::Diction_list() :List(sizeof(Article))
{
	;
}

Diction_list::~Diction_list()
{
	List::~List();
}

Article* Diction_list::find(char*word)
{
	Article * now= new Article();
	for (int i = 0; i < count(); i++)
	{  
		
		now = (Article*)get(i);
		if (!strcmp(now->word , word))
		{ 
			return now;	
		}
	}
	return nullptr;
}

int Diction_list::findpos(char* word)
{
	Article * now = new Article();

	for (int i = 0; i < count(); i++)
	{
		now = (Article*)get(i);
		if (!strcmp(now->word, word))
		{
			return i;
		}

	}
	return -1;
}
void Diction_list::put(Article* article)
{ 
	if (article != nullptr){
		
		Article* art = (Article*)heap.get_mem(sizeof(Article));

		art->word = (char*)heap.get_mem(strlen(article->word)+1);
		
		art->description = (char*)heap.get_mem(strlen(article->description)+1);
		memcpy(art->word, article->word, strlen(article->word)+1);
		memcpy(art->description, article->description, strlen(article->description)+1);
		add(art);		
		heap.free_mem(art);
	}
	
}

void Diction_list::del(char* word)
{
	int a = findpos(word);
	if (a != -1)
	{
		heap.free_mem(((Article*)get(a))->word);
	    heap.free_mem(((Article*)get(a))->description);
		remove(a);
	}
}
void Diction_list::del(Article* article)
{
	del(article->word);
}

void Diction_list::print_list()
{
	for (int j = 0; j < count(); j++)
	{
		Article* a = (Article*)get(j);
		printf("Place %d Article: %s  -  %s  \n", j, a->word, a->description);
	}
}

Article* Diction::find(char* word)
{
	Diction_list* list = (Diction_list*)find_list(word);
    return list->find(word);
	
}

Article* Diction::auto_create(char*word)
{
	Article art = { word, "" };
	Article* fin = find(word);
	if (fin != nullptr)
		return fin;
	else
	{
		Diction_list* list = (Diction_list*)find_list(word);
		if (!list)
		{
			list = (Diction_list*)heap.get_mem(sizeof(Diction_list));
		}
		list->put(&art);
	}
	return find(word);
}

Article* Diction::auto_create(char*word, char* description)
{  
	
	Article art = { word, description };
	Article* fin = find(word);
	if (fin)

	{
		return fin;
	}
	else
	{
		Diction_list* list = (Diction_list*)find_list(word);
		list->put(&art);
		return find(word);
	}
}