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

	virtual int compare(void* i, void* j) { return 0; }; //���������� ��� ��������
	void*      get(int pos); //���������� ������ i-��� ��������
	void       add(void* data); //��������� ����� �������
	void       take_first(void* store); //�������� ������ ������� ��������
	void       take_last(void* store); //�������� ������ ���������� ��������
	void       take(int pos, void* store); //�������� ������ ���������� ��������
	void       sort(bool dir = true, int method = 0); //����������
	void	   remove(int pos); //������� ������� �� pos����
	int        count(); //���������� ���������� ���������
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

	void new_segment(); //������� �����-�� ������� � ����� ������
	void delete_segment(Segment* seg); //������� ������������ �������
};

#endif // LIST_H_INCLUDED