#include "Heap.h"
#include "List.h"

#define MAXRANGE 20

//Heap heap;
//static Heap heap;

class Hash
{
public:
	Hash(int _n1, int _n2, int _n3, int _n4, int _n5);
   ~Hash(); 

	List* find_list(char* key_word);

	virtual int key1(char* key_word){return 0;}
	virtual int key2(char* key_word){ return 0; }
	virtual int key3(char* key_word){ return 0; }
	virtual int key4(char* key_word){ return 0; }
	virtual int key5(char* key_word) { return 0; }
	
	void print_hash();
protected:
    int combine_keys(char* key_word)
		{  
			return
				abs(key1(key_word)) % n1 +
				abs(key2(key_word)) % n2 *n2 +
				abs(key3(key_word)) % n3 *n3*n2 +
				abs(key4(key_word)) % n4 *n4*n3*n2 +
				abs(key5(key_word)) % n5 *n5*n4*n3*n2;
             
        };

	List* table;
	int    n1,n2,n3,n4,n5;
};

struct Article
{
	char* word;
	char* description;
};

class Diction_list : public List
{ 
public:
	Diction_list();  //:List(sizeof(Article));
	~Diction_list(); // purge all Article data

	void     put(Article* article); // allocate memory for word & description
	Article* find(char* word); 
	int      findpos(char* word);
	void     del(char* word); // free memory from word & decription
	void     del(Article* article);// { del(article->word) };
	void     print_list();
};

class Diction : public Hash
{
public:
	Diction(int _n1, int _n2, int _n3, int _n4, int _n5) : Hash(_n1, _n2, _n3, _n4, _n5) { ; };
	~Diction(); 
	
	int key1(char* key_word);
	int key2(char* key_word);

	Article* find(char* word);

	Article* auto_create(char* word); // create Article (and List) if not exist
	Article* auto_create(char* word,char* description);
private:
	
};
