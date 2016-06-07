// heap.h - TODO: Description

#pragma once

#include <stdio.h>
#include "windows.h"
#define SEGMENTSIZE 65539
#define SEGMENTCOUNT 1024

class Heap
{
public:
	Heap(int _segment_size = SEGMENTSIZE);
	~Heap();
	void*      get_mem(int size); //Возвращает указатель на выделенную память. На вход принимает кол-во байт для выделения.
	void       free_mem(void*); //Освобождает выделенную get_mem память по указателю. Знает сколько удалять.
	void print(); //Отладочная ф-ция печати.
private:
	struct Chunk
	{
		bool      used;
		int       size;
		void*     offset;
	};

	struct Segment
	{
		void*        data;
		Segment*     prev;
		Chunk  descriptor[SEGMENTCOUNT];
		int          descriptor_count;
	};

	int       make_segment();
	void      delete_segments();

	int       segment_size;

	Segment*  current;

};

static Heap heap = *new Heap();
