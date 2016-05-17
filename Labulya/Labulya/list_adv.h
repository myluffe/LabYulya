#include "list.h"

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

	void   push(double x)
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
