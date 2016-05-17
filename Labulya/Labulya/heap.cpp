#include "stdafx.h"
#include "heap.h"

Heap::Heap(int _segment_size)
{
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
    if(size <= SEGMENTSIZE)
	{Segment*  now_current = current;
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
						now_current->descriptor[i+1] = my_Chunk;
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

		return current->descriptor[0].offset;
	}
	}
	else
        //error "a lot of memory"
        ;

	return 0;
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
					for (int i2 = i; i2 < now_current->descriptor_count-count; i2++)
					{
						now_current->descriptor[i].size += now_current->descriptor[i + count].size;
					}
					now_current->descriptor_count -= count;
				}
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
		VirtualFree(now_current->data, segment_size, MEM_RELEASE);
		now_current = now_current->prev;
	}
}

int Heap::make_segment()
{
	Segment *my_Segment = (Segment*)malloc(sizeof(Segment));
	my_Segment->data =  malloc(segment_size);
	if (my_Segment->data != NULL)
	{
		my_Segment->prev = current;
		my_Segment->descriptor_count = 1;
        current = my_Segment;
		Chunk *my_Chunk = (Chunk*)malloc(sizeof(Chunk));
		my_Chunk->offset = my_Segment->data;
		my_Chunk->size = segment_size;
		my_Chunk->used = false;
		my_Segment->descriptor[0] = *my_Chunk;
		return 0;
	}
	return -1;
}

void Heap::print()
{
    Segment*  now_current = current;
    while (now_current != 0)
	{
	    printf("new Seg:\n%d\t%d\n",now_current->descriptor_count, now_current->data);
		for (int i = 0; i < now_current->descriptor_count; i++)
		{
		    printf("%d\t%d\t%d\n", now_current->descriptor[i].size, now_current->descriptor[i].offset, now_current->descriptor[i].used?1:0);
		}
		now_current = now_current->prev;
	}
}
