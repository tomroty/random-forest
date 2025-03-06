#include "ShortestPath.h"

Path *Graph_shortestPath(Graph *graph, int start, int end)
{
    int size = Graph_size(graph);

    assert(0 <= start && start < size);
    assert(end < size);

    int *predecessors = (int *)calloc(size, sizeof(int));
    AssertNew(predecessors);

    float *distances = (float *)calloc(size, sizeof(float));
    AssertNew(distances);

    Graph_dijkstra(graph, start, end, predecessors, distances);

    Path *path = Graph_dijkstraGetPath(predecessors, distances, end);

    free(predecessors);
    predecessors = NULL;
    free(distances);
    distances = NULL;

    return path;
}

void Graph_dijkstra(Graph* graph, int start, int end, int* predecessors, float* distances)
{
    int size = Graph_size(graph);

    assert(0 <= start && start < size);
    assert(end < size);

    BinHeap* heap = Heap_create(size);
    Heap_insert(heap, start, 0.0f);

    for (int i = 0; i < size; i++)
    {
        predecessors[i] = -1;
        distances[i] = INFINITY;
    }
    distances[start] = 0.0f;

    while (heap->capacity > 0)
    {
        int currID = Heap_pop(heap);
        if (currID == end)
            break;

        for (ArcList* arc = Graph_getArcList(graph, currID);
            arc != NULL; arc = arc->next)
        {
            int nextID = arc->target;
            float dist = distances[currID] + arc->weight;
            if (distances[nextID] > dist)
            {
                distances[nextID] = dist;
                predecessors[nextID] = currID;
                Heap_insert(heap, nextID, dist);
            }
        }
    }
    Heap_destroy(heap);
}

Path *Graph_dijkstraGetPath(int *predecessors, float *distances, int end)
{
    assert(predecessors && distances);
    assert(end >= 0);

    if (predecessors[end] < 0)
    {
        return NULL;
    }

    Path *path = Path_create(end);
    int currID = end;

    while (predecessors[currID] >= 0)
    {
        currID = predecessors[currID];
        ListInt_insertFirst(path->list, currID);
    }

    path->distance = distances[end];

    return path;
}

Path *Path_create(int start)
{
    Path *path = (Path *)calloc(1, sizeof(Path));
    AssertNew(path);

    path->distance = 0.0f;
    path->list = ListInt_create();
    ListInt_insertLast(path->list, start);

    return path;
}

void Path_destroy(Path *path)
{
    if (path == NULL) return;

    ListInt_destroy(path->list);
    free(path);
}

void Path_print(Path *path)
{
    if (path == NULL)
    {
        printf("path : NULL\n");
        return;
    }

    printf("path (distance = %f) : ", path->distance);
    ListInt_print(path->list);
}
