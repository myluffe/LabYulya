#pragma once

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>

#define LISTSIZE 64

class List
{
public:
	List(int _element_size);
	~List();

	virtual int compare(void* i, void* j) { return 0; }; //сравнивает два элемента
	void*      get(int pos); //возвращает i-ый элемент списка
	void       add(void* data); //добавляет новый элемент в список
	void	   set(int pos, void* data); //заменяет значение на позиции pos на значение из data
	void       take_first(void* store); //копирует первый элемент в storage
	void       take_last(void* store); //копирует последний элемент в storage
	void       take(int pos, void* store); //копирует элемент на позиции pos в storage
	void       sort(bool dir = true, int method = 0); //сортировка
	void	   remove(int pos); //удаляет элемент на позиции pos
	int        count(); //возвращает количество элементов
	bool       error(); // true if error in last operation

	void change(int pos, void * data); //ЗАМЕНА ССЫЛКИ!
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

	void new_segment(); //выделяет память в конце списка под сегмент и данные
	void delete_segment(Segment* seg); //освобождает определенный элемент
};

#endif // LIST_H_INCLUDED