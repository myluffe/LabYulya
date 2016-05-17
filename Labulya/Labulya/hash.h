#include "Heap.h"
#include "List.h"

#define MAXRANGE 20

class Hash
{
public:
	Hash(int _n1, int _n2, int _n3, int _n4, int _n5)
        { n1 = abs(_n1) % MAXRANGE + 1;
          n2 = abs(_n2) % MAXRANGE + 1;
          n3 = abs(_n3) % MAXRANGE + 1;
          n4 = abs(_n4) % MAXRANGE + 1;
          n5 = abs(_n5) % MAXRANGE + 1;
          table=(List**)heap.get_mem(n1*n2*n3*n4*n5*sizeof(List*);
        };
	~Hash(); 

	List* find_list(char* key_word)
          { return table[combine_keys(key_word)]; };

	virtual int key1(char* key_word) { return 0; };
	virtual int key2(char* key_word) { return 0; };
	virtual int key3(char* key_word) { return 0; };
	virtual int key4(char* key_word) { return 0; };
	virtual int key5(char* key_word) { return 0; };

protected:
    int combine_keys(char* key_word)
		{  
			return 	
			    abs(key5(key_word))%n5 + 
			    abs(key4(key_word))%n4 *n4 +
			    abs(key3(key_word))%n3 *n3*n4 +
			    abs(key2(key_word))%n2 *n2*n3*n4 +
			    abs(key1(key_word))%n1 *n1*n2*n3*n4 +
             );	
        };

private:
	List** table;
	int    n1,n2,n3,n4,n5;
};

struct Article
{
	char* word;
	char* description;
};

class Diction_list : public List
{
	Diction_list():List(sizeof(Article));
	~Diction_list(); // purge all Article data

	void     put(Article* article); // allocate memory for word & description
	Article* find(char* word); 
	void     del(char* word); // free memory from word & decription
	void     del(Article* article) { del(article->word) };
};

class Diction : public Hash
{
public: 
	Diction() { Hash::Hash(33,33,0,0,0); }
	~Diction(); 
	
	int key1(char* key_word) { int f = key_word[0]-'A';
				   if (f<33 && f>0) return f;
			           else             return 0; }
	int key2(char* key_word) { return key_word[1] % 33; }

	Article* find (char* word)
    { Diction_list* list =(Diction_list*)find_list(word);
      return list ? list->find(*word) : 0; 
    };
	Article* auto_create(char* word); // create Article (and List) if not exist
private:
	
};
