#include "list.h"
#include "lec.h"

class Stack : List
{
public:
	Stack() : List(sizeof(double))
	{
	}
	~Stack()
	{
	    List::~List();
    }

	void push(double x)
	{
	    add(&x);
    }
	double pop()
	{
	    double res;
	    take_last(&res);
	    remove(count()-1);
	    return res;
    }
};

class LexemeStack : List
{
public:
	LexemeStack() : List(sizeof(lexeme))
	{
	}
	~LexemeStack()
	{
		List::~List();
	}
	void push(lexeme* x)
	{
		add(x);
	}
	void pop(lexeme* store)
	{
		take_last(store);
		remove(count() - 1);
	}
};

class Queue : List
{
public:
	Queue() : List(sizeof(double))
	{
	}
	~Queue()
	{
	    List::~List();
    }

	void   put(double x)
	{
	    add(&x);
    }
	double get()
	{
	    double res;
	    take_first(&res);
		remove(0);
	    return res;
    }
};

class Deque : List
{
    public:
	Deque() : List(sizeof(double))
	{
	}
	~Deque()
	{
	    List::~List();
    }

	void   put(double x)
	{
	    add(&x);
    }
	double first()
	{
	    double res;
	    take_first(&res);
		remove(0);
	    return res;
    }
	double last()
	{
	    double res;
	    take_last(&res);
		remove(count() - 1);
	    return res;
    }
};
