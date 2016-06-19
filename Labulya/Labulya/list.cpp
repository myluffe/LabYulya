#include "stdafx.h"
#include "list.h"

List::List(int element_size)
{
	_element_count = LISTSIZE;
	_element_size = element_size;
	_count = 0;
	first = nullptr;
	last = nullptr;
	_error = false;
}

List::~List()
{
	Segment* s = first;
	while (s != nullptr)
	{
		Segment* snext = s->next;
		delete_segment(s);
		s = snext;
	}
}

void * List::get(int pos)
{
	if (pos < 0 && pos >= _count)
	{
		_error = true;
		return nullptr;
	}
	else
	{
		int n;
		Segment* s = first;
		for(n = 0; n != pos; n++)
		{
			s = s->next;
		}
		return s->data;
	}
}


void List::add(void * data)
{
    new_segment();
	memcpy(last->data, data, _element_size);
}

void List::set(int pos, void * data)
{
	int n;
	Segment* s = first;
	for (n = 0; n != pos; n++)
	{
		s = s->next;
	}
	memcpy(s->data, data, _element_size);
}

void List::take_first(void * store)
{
	memcpy(store, first->data, _element_size);
}

void List::take_last(void * store)
{
	memcpy(store, last->data, _element_size);
}

void List::take(int pos, void * store)
{
	if (pos < 0 && pos >= _count)
	{
		_error = true;
	}
	else
	{
		int n;
		Segment* s = first;
		for(n = 0; n != pos; n++)
		{
			s = s->next;
		}
		memcpy(store, s->data, _element_size);
	}
}

void List::sort(bool dir, int method)
{
    if (method == 1)
    {
        bool flag = 1;
        Segment* tmp;
        Segment* a;
        void* fd;
        while (flag == 1)
        {
            tmp = first;
            a = tmp->next;
            flag = 0;
            while (a)
            {
                if (compare(tmp->data, a->data)) // если tmp > a
                {
                    fd = tmp->data;
                    tmp->data = a->data;
                    a->data = fd;
                    flag = 1;
                }
                tmp = tmp->next;
                a = a->next;
            }
        }
    }
}

void List::remove(int pos)
{
	if(pos >= 0 && pos < _count)
    {
        int i;
        Segment *current = first;
        for(i = 0; i < pos; i++)
            current = current->next;
        delete_segment(current);
    }
    else
         _error = true;
}

int List::count()
{
	return _count;
}

bool List::error()
{
	return _error;
}

void List::new_segment()
{
	Segment *a = (Segment*)heap.get_mem(sizeof(Segment));
	a->data = heap.get_mem(_element_size);
    a->next = nullptr;
	if (first == nullptr)
	{
		first = a;
        a->prev = nullptr;
		last = a;
	}
	else
	{
		a->prev = last;
        last->next = a;
        last = a;
	}
	_count++;
}

void List::delete_segment(Segment * seg)
{
	if (seg->prev == nullptr)
	{
		if (seg->next == nullptr)
		{
			first = nullptr;
			last = nullptr;
		}
		else
		{
			first = seg->next;
			first->prev = nullptr;
		}
	}
	else
	{
		if (seg->next == nullptr)
		{
			last = seg->prev;
			last->next = nullptr;
		}
		else
		{
			seg->next->prev = seg->prev;
			seg->prev->next = seg->next;
		}
	}
	heap.free_mem(seg->data);
	heap.free_mem(seg);
	_count--;
}

