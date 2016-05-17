#pragma once

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "heap.h"
#include <stdio.h>

#define LISTSIZE 64

class List
{
public:
	List(int _element_size);
	~List();

	virtual int compare(void* i, void* j) { return 0; }; //сравнивает два элемента
	void*      get(int pos); //возвращает данные i-ого элемента
	void       add(void* data); //добавляет новый элемент
	void       take_first(void* store); //получает данные первого элемента
	void       take_last(void* store); //получает данные последнего элемента
	void       take(int pos, void* store); //получает данные указанного элемента
	void       sort(bool dir = true, int method = 0); //сортировка
	void	   remove(int pos); //удаляет элемент на posиции
	int        count(); //возвращает количество элементов
	bool       error(); // true if error in last operation
private:
	struct Segment
	{
		void*    data;
		Segment* prev;
		Segment* next;
	};
	struct Segment*  first;
	struct Segment*  last;

	int              _element_size;
	int              _element_count;
	int              _count;
	bool             _error;
    Heap*             ListHeap;

	void new_segment(); //создает какой-то элемент в конце списка
	void delete_segment(Segment* seg); //удаляет определенный элемент
};

#endif // LIST_H_INCLUDED