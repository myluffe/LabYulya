// heap.h - TODO: Description

#pragma once

#include <stdio.h>
#include "windows.h"
#include "ErrorReporter.h"

#define SEGMENTSIZE 65539
#define SEGMENTCOUNT 1024
#define MAXSIZE 8388608

static class Heap
{
public:
	Heap(int _segment_size);
	~Heap();
	void*      get_mem(int size); //���������� ��������� �� ���������� ������. �� ���� ��������� ���-�� ���� ��� ���������.
	void       free_mem(void*); //����������� ���������� get_mem ������ �� ���������. ����� ������� �������.
private:
	void print(); //���������� �-��� ������.
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
	int		count_segments;
	int       make_segment();
	void      delete_segments();

	int       segment_size;

	Segment*  current;

}heap_my = *new Heap(SEGMENTSIZE);