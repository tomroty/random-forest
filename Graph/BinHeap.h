#pragma once
#include "Graph.h"
#include "ListInt.h"


typedef struct pair
{
    int node;
    float dist;
}pair;

typedef struct BinHeap
{
    int capacity;
    pair* array;
}BinHeap;

BinHeap* Heap_create(int size);

void Heap_insert(BinHeap* heap, int u, float distance);

int Heap_pop(BinHeap* heap);

void Heap_destroy(BinHeap* heap);