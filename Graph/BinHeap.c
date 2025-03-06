#include "BinHeap.h"

BinHeap* Heap_create(int size)
{
    BinHeap* heap = calloc(1, sizeof(BinHeap));
    heap->capacity = 0;
    heap->array = calloc(size, sizeof(pair));
    return heap;
}



void Heap_insert(BinHeap* heap, int u, float distance)
{
    int i = heap->capacity;
    heap->capacity++;
    while (i > 0 && heap->array[(i - 1) / 2].dist > distance)
    {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i].node = u;
    heap->array[i].dist = distance;
}



int Heap_pop(BinHeap* heap)
{
    int min = heap->array[0].node;
    heap->capacity--;
    pair last = heap->array[heap->capacity];
    int i = 0;
    while (2 * i + 1 < heap->capacity)
    {
        int child = 2 * i + 1;
        if (child + 1 < heap->capacity && heap->array[child + 1].dist < heap->array[child].dist)
        {
            child++;
        }
        if (last.dist <= heap->array[child].dist)
        {
            break;
        }
        heap->array[i] = heap->array[child];
        i = child;
    }
    heap->array[i] = last;
    return min;
}


void Heap_destroy(BinHeap* heap)
{
    free(heap->array);
    free(heap);
}