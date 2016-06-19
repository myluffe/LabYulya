#include "stdafx.h"
#include "heap.h"

Heap::Heap(int _segment_size)
{
	count_segments = 0;
	segment_size = _segment_size;
	current = 0;
	make_segment();
}

Heap::~Heap()
{
	delete_segments();
}

void* Heap::get_mem(int size)
{
	if (size <= SEGMENTSIZE)
	{
		Segment*  now_current = current;
		while (now_current != 0)
		{
			for (int i = 0; i < now_current->descriptor_count; i++)
			{
				if (size <= now_current->descriptor[i].size && !now_current->descriptor[i].used)
				{
					if (size == now_current->descriptor[i].size)
					{
						now_current->descriptor[i].used = true;
						return now_current->descriptor[i].offset;
					}
					else
					{
						if (now_current->descriptor_count + 1 <= SEGMENTCOUNT)
						{
							Chunk my_Chunk;
							my_Chunk.offset = (char*)(now_current->descriptor[i].offset) + size;
							my_Chunk.size = now_current->descriptor[i].size - size;
							my_Chunk.used = false;

							now_current->descriptor[i].used = true;
							now_current->descriptor[i].size = size;
							now_current->descriptor_count++;

							for (int h = now_current->descriptor_count; h > i; h--)
							{
								now_current->descriptor[h] = now_current->descriptor[h - 1];
							}
							now_current->descriptor[i + 1] = my_Chunk;
							print();
							return now_current->descriptor[i].offset;
						}
					}
				}
			}
			now_current = now_current->prev;
		}

		if (make_segment() == 0)
		{
			Chunk my_Chunk;
			my_Chunk.offset = (char*)(current->data) + size;
			my_Chunk.size = current->descriptor[0].size - size;
			my_Chunk.used = false;

			current->descriptor[0].used = true;
			current->descriptor[0].size = size;
			current->descriptor[1] = my_Chunk;
			current->descriptor_count++;
			print();
			return current->descriptor[0].offset;
		}
		else
		{
			errorReporter.FReport(logfile, "A lot of segment of memory", 0, 0);
		}
	}
	else
		errorReporter.FReport(logfile, "A big size for segment of memory", 0, 0);

	return nullptr;
}

void Heap::free_mem(void* free_m)
{
	Segment*  now_current = current;
	while (now_current != 0)
	{
		for (int i = 0; i < now_current->descriptor_count; i++)
		{
			if (free_m == now_current->descriptor[i].offset)
			{
				now_current->descriptor[i].used = false;
				bool prev = false;
				int count = 0;
				if (i - 1 > 0 && !now_current->descriptor[i - 1].used)
				{
					prev = true;
					count++;
				}
				bool next = false;
				if (i + 1 < now_current->descriptor_count && !now_current->descriptor[i + 1].used)
				{
					next = true;
					count++;
				}
				if (count > 0)
				{
					if (prev)
					{
						i = i - 1;
						if (next)
							now_current->descriptor[i].size += now_current->descriptor[i + 1].size;
					}
					for (int i2 = i; i2 < now_current->descriptor_count - count; i2++)
					{
						now_current->descriptor[i].size += now_current->descriptor[i + count].size;
					}
					now_current->descriptor_count -= count;
				}
				print();
				return;
			}
		}
		now_current = now_current->prev;
	}
}

void Heap::delete_segments()
{
	Segment*  now_current = current;
	while (now_current != 0)
	{
		free(now_current->data);
		now_current = now_current->prev;
	}
}

int Heap::make_segment()
{
	if (count_segments * segment_size < MAXSIZE)
	{
		Segment *my_Segment = (Segment*)malloc(sizeof(Segment));
		my_Segment->data = malloc(segment_size);
		if (my_Segment->data != nullptr)
		{
			count_segments++;
			my_Segment->prev = current;
			my_Segment->descriptor_count = 1;
			current = my_Segment;
			Chunk *my_Chunk = (Chunk*)malloc(sizeof(Chunk));
			my_Chunk->offset = my_Segment->data;
			my_Chunk->size = segment_size;
			my_Chunk->used = false;
			my_Segment->descriptor[0] = *my_Chunk;
			LogFile.Write(("new Seg:\n%d\t%s\n", my_Segment->descriptor_count, (char*)my_Segment->data));
			return 0;
		}
	}
	return -1;
}

void Heap::print()
{
	Segment*  now_current = current;
	while (now_current != 0)
	{
		LogFile.Write(("new Seg:\n%d\t%s\n", now_current->descriptor_count, (char*)now_current->data));
		for (int i = 0; i < now_current->descriptor_count; i++)
		{
			LogFile.Write(("%d\t%s\t%s\n", now_current->descriptor[i].size, (char*)now_current->descriptor[i].offset, (char*)now_current->descriptor[i].used));
		}
		now_current = now_current->prev;
	}
}
Heap::Heap()
{
}

Heap Heap::GetHeap()
{
	if(heap == nullptr)
		heap = new Heap(SEGMENTSIZE);
	return *heap;
}