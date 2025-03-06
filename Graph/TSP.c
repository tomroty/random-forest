#include "TSP.h"

Path* Graph_tspFromHeuristic(Graph* graph, int station)
{
	bool* explored = calloc(graph->size, sizeof(bool));
	explored[station] = true;
	int prev = station;
	Path *path = Path_create(station);
	for (int i = 0; i < (graph->size)-1; i++)
	{
		ArcList* arc = Graph_getArcList(graph, prev);
		int next = -1;
		float next_distance = -1;
		int arcCount = Graph_getArcCount(graph, prev);
		for (int j = 0; j < arcCount; j++)
		{
			if ((next < 0 || arc->weight < next_distance)&& explored[arc->target] == false)
			{
				next = arc->target;
				next_distance = arc->weight;
			}
			arc = arc->next;
		}
		explored[next] = true;
		ListInt_insertLast(path->list, next);
		float* tmp = Graph_getArc(graph, prev, next);
		path->distance += *tmp;
		prev = next;
	}
	ListInt_insertLast(path->list, station);
	float* tmp = Graph_getArc(graph, prev, station);
	path->distance += *tmp;
	free(explored);
	return path;
}

float* Graph_acoGetProbabilities(
	Graph* graph, Graph* pheromones, int station,
	bool* explored, float alpha, float beta)
{
	int arcCount = Graph_getArcCount(graph, station);
	float* proba = calloc(graph->size, sizeof(float));
	float sum = 0;
	ArcList* arc = Graph_getArcList(graph, station);
	for (int i = 0; i < arcCount; i++)
	{
		if (explored[arc->target] == false)
		{
			float* distance = Graph_getArc(graph, station, arc->target);
			float *pheromone = Graph_getArc(pheromones, station, arc->target);
			proba[arc->target] = (float) powf(*pheromone, alpha) * powf(1 / *distance, beta);
			sum += proba[arc->target];
		}
		else
		{
			proba[arc->target] = 0;
		}
		arc = arc->next;
	}
	for (int i = 0; i < arcCount+1; i++)
	{
		proba[i] /= sum;
	}
	return proba;
}

Path* Graph_tspFromACO(
	Graph* graph, int station, int iterationCount, int antCount,
	float alpha, float beta, float rho, float q)
{
	Path* path = (Path*)calloc(1, sizeof(Path));
	AssertNew(path);
	path->distance = INFINITY;
	Graph* pheromones = Graph_create(graph->size);
	for (int i = 0; i < graph->size; i++)
	{
		for (int j = 0; j < graph->size; j++)
		{
			if(i!=j)
				Graph_setArc(pheromones, i, j, 1.0);
		}
	}
	for (int i = 0; i < iterationCount; i++)
	{
		Path** tmp = calloc(antCount, sizeof(Path*));
		for (int j = 0; j < antCount; j++)
		{
			tmp[j] = Graph_acoConstructPath(graph, pheromones, station, alpha, beta);
			//Path_print(tmp[j]);
			if (tmp[j]->distance < path->distance)
			{
				if (path->list)
					ListInt_destroy(path->list);
				path->list = ListInt_copy(tmp[j]->list);
				path->distance = tmp[j]->distance;
			}
		}
		Graph_acoPheromoneGlobalUpdate(pheromones, rho);
		for (int j = 0; j < antCount; j++)
		{
			Graph_acoPheromoneUpdatePath(pheromones, tmp[j], q);
		}
		for (int j = 0; j < antCount; j++)
		{
			Path_destroy(tmp[j]);
		}
		free(tmp);
	}
	Graph_destroy(pheromones);
	return path;
}

Path* Graph_acoConstructPath(Graph* graph, Graph* pheromones, int station, float alpha, float beta)
{
	bool* explored = calloc(graph->size, sizeof(bool));
	explored[station] = true;
	int prev = station;
	Path* path = Path_create(station);
	for (int i = 0; i < (graph->size) -1 ; i++)
	{
		int arcCount = Graph_getArcCount(graph, station);
		float* proba = Graph_acoGetProbabilities(graph, pheromones, prev, explored, alpha, beta);
		int next = Graph_tirageSuivantLoi(proba, arcCount, explored);
		assert(next != prev);
		explored[next] = true;
		ListInt_insertLast(path->list, next);
		float* tmp = Graph_getArc(graph, prev, next);
		assert(tmp);
		path->distance += *tmp;
		prev = next;
		free(proba);
	}
	ListInt_insertLast(path->list, station);
	float* tmp = Graph_getArc(graph, prev, station);
	path->distance += *tmp;
	free(explored);
	return path;
}

void Graph_acoPheromoneUpdatePath(Graph* pheromones, Path* path, float q)
{
	ListIntNode* node = path->list->sentinel.next;
	int count = path->list->nodeCount;
	for (int i = 0; i < count-1; i++)
	{
		float* distance = Graph_getArc(pheromones, node->value, node->next->value);
		ArcList* arc = Graph_getArcList(pheromones, node->value);
		while (arc->target != node->next->value)
		{
			arc = arc->next;
		}
		arc->weight += (q / *(distance));
		node = node->next;
	}
}

void Graph_acoPheromoneGlobalUpdate(Graph* pheromones, float rho)
{
	for (int i = 0; i < pheromones->size; i++)
	{
		ArcList* arc = Graph_getArcList(pheromones, i);
		int arcCount = Graph_getArcCount(pheromones, i);
		for (int j = 0; j < arcCount; j++)
		{
			arc->weight *= (1 - rho);
			arc = arc->next;
		}
	}
}

int Graph_tirageSuivantLoi(float* p, int n, bool* explored)
{
	float random = (float)rand() / RAND_MAX;
	float sum = 0;
	int itmp = 0;
	for (int i = 0; i < n+1; i++)
	{
		itmp = i;
		sum += p[i];
		if (random < sum)
		{
			break;
		}
	}
	while (explored[itmp] == true)
	{
		itmp--;
	}
	return itmp;
}