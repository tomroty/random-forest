#include "Settings.h"
#include "Graph.h"
#include "ListInt.h"
#include "ShortestPath.h"
#include "TSP.h"
#include "affichage.h"
#include "TownInput.h"
#include <time.h> 

int main()
{
#if 1 //MAIN BONUS
    csv** csvTown = InputFetch();



    /////////// recuperation des données

    FILE* finput = fopen("../BonusData/tmp.txt", "r");
    assert(finput);

    char pathGraph[128] = { 0 };
    char pathInter[128] = { 0 };
    int nb_points = 0;
    fscanf(finput, "%s\n", pathGraph);
    fscanf(finput, "%s\n", pathInter);
    fscanf(finput, "%d\n", &nb_points);
    int* array_points = calloc(nb_points, sizeof(int));

    //printf("%s\n%s\n%d %d\n", pathGraph, pathInter, array_points[0]);

    FILE* finter = fopen(pathInter, "r");
    if (!finter)
    {
        printf("Vous avez surement oublie les instances sur la France, faut lire le readme un peu c'est quoi ce travail\n");
		abort();
    }


    int size = 0;
    fscanf(finter, "%d\n", &size);
    //printf("%d\n", size);

    float** interArray = calloc(size, sizeof(float*));
    for (int i = 0; i < size; i++)
    {
        interArray[i] = calloc(2, sizeof(float));
        fscanf(finter, "%f %f\n", &interArray[i][0], &interArray[i][1]);
        //printf("%d %f %f\n",i, interArray[i][0], interArray[i][1]);
    }

    printf("Calcul des coordoonees...\n");
    float* tmp_lat = calloc(nb_points, sizeof(float));
    for (int i = 0; i < nb_points; i++)
    {
        tmp_lat[i] = atof(csvTown[i]->latitude);
    }
    float* tmp_long = calloc(nb_points, sizeof(float));
    for (int i = 0; i < nb_points; i++)
    {
        tmp_long[i] = atof(csvTown[i]->longitude);
        free(csvTown[i]);
    }
    free(csvTown);

    for (int i = 0; i < nb_points; i++)
    {
        float difflong;
        float difflat;
        float diffTot;
        float min_diffTot = INFINITY;
        int min_Id = size + 1;
        for (int j = 0; j < size; j++)
        {
            difflong = fabs(interArray[j][0] - tmp_long[i]);
            difflat = fabs(interArray[j][1] - tmp_lat[i]);
            diffTot = difflong + difflat;
            if (diffTot < min_diffTot)
            {
                min_diffTot = diffTot;
                min_Id = j;
            }
        }
        //printf("minid = %d\n", min_Id);
        //printf("%f\n", interArray[min_Id][0]);
        //printf("%f\n", interArray[min_Id][1]);
        array_points[i] = min_Id;
    }

    free(tmp_lat);
    free(tmp_long);

    printf("Chargement du graphe...\n");
    Graph* graph = Graph_load(pathGraph);

    Path*** PathArray = calloc(nb_points, sizeof(Path**));
    for (int i = 0; i < nb_points; i++)
    {
        PathArray[i] = calloc(nb_points, sizeof(Path*));
    }


    printf("Creation du sous-graphe...\n");
    Graph* sousgraph = Graph_create(nb_points);
    for (int i = 0; i < nb_points; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (array_points[i] != array_points[j])
            {
                PathArray[i][j] = Graph_shortestPath(graph, array_points[i], array_points[j]);
                PathArray[j][i] = PathArray[i][j];
                Graph_setArc(sousgraph, i, j, PathArray[i][j]->distance);
                Graph_setArc(sousgraph, j, i, PathArray[j][i]->distance);
            }
        }
    }


    /////////////OUTPUT ACO
    printf("Calcul de la tournee...\n");
    Path* tournee = Graph_tspFromACO(sousgraph, 0, 1000, 100, 2.0f, 3.0f, 0.1f, 2.0f);
    printf("Distance: %.1f\nNombre de noeuds: %d\n", tournee->distance, (tournee->list->nodeCount));
    ListIntNode* node = tournee->list->sentinel.next;
    printf("Chemin: ");
    for (int i = 0; i < tournee->list->nodeCount; i++)
    {
        printf("%d ", node->value);
        node = node->next;
    }
    printf("\n");

    ///////////// Creation du geojson

    printf("Creation du geojson...\n");
    affichage_json(nb_points, PathArray, tournee, array_points, interArray);


    //////////////// liberation de la memoire

    free(array_points);

    for (int i = 0; i < size; i++)
    {
        free(interArray[i]);
    }
    free(interArray);

    for (int i = 0; i < nb_points; i++)
    {
        for (int j = 0; j < i; j++)
        {
            Path_destroy(PathArray[i][j]);
        }
        free(PathArray[i]);
    }
    free(PathArray);

    Path_destroy(tournee);

    Graph_destroy(graph);
    Graph_destroy(sousgraph);

    fclose(finput);
    fclose(finter);
#else  //MAIN TEST
    /////////// recuperation des données

    FILE* finput = fopen("../Tests/4_TSP_ACO/input1.txt", "r");
    assert(finput);

    char pathGraph[128] = { 0 };
    char pathInter[128] = { 0 };
    int nb_points = 0;
    fscanf(finput, "%s\n", pathGraph);
    fscanf(finput, "%s\n", pathInter);
    fscanf(finput, "%d\n", &nb_points);
    int* array_points = calloc(nb_points, sizeof(int));
    //printf("%d\n", nb_points);
    for (int i = 0; i < nb_points; i++)
    {
        fscanf(finput, "%d ", &array_points[i]);
    }

    //printf("%s\n%s\n%d %d\n", pathGraph, pathInter, array_points[0]);

    FILE* finter = fopen(pathInter, "r");
    int size = 0;
    fscanf(finter, "%d\n", &size);
    //printf("%d\n", size);

    float** interArray = calloc(size, sizeof(float*));
    for (int i = 0; i < size; i++)
    {
        interArray[i] = calloc(2, sizeof(float));
        fscanf(finter, "%f %f\n", &interArray[i][0], &interArray[i][1]);
        //printf("%d %f %f\n",i, interArray[i][0], interArray[i][1]);
    }
    Graph* graph = Graph_load(pathGraph);

    Path*** PathArray = calloc(nb_points, sizeof(Path**));
    for (int i = 0; i < nb_points; i++)
    {
        PathArray[i] = calloc(nb_points, sizeof(Path*));
    }

    Graph* sousgraph = Graph_create(nb_points);
    for (int i = 0; i < nb_points; i++)
    {
        for (int j = 0; j < nb_points; j++)
        {
            if (array_points[i] != array_points[j])
            {
                PathArray[i][j] = Graph_shortestPath(graph, array_points[i], array_points[j]);
                Graph_setArc(sousgraph, i, j, PathArray[i][j]->distance);
                //printf("%d %d %.1f", i, j, PathArray[i][j]->distance);  //PATH MATRIX
            }
        }
    }

    /////////////OUTPUT Heuristic

    //Path* tournee = Graph_tspFromHeuristic(sousgraph, 0);
    //printf("%.1f %d\n", tournee->distance, (tournee->list->nodeCount));
    //ListIntNode* node = tournee->list->sentinel.next;
    //for (int i = 0; i < tournee->list->nodeCount; i++)
    //{
    //    printf("%d ", node->value);
    //    node = node->next;
    //}
    //printf("\n");

    /////////////OUTPUT ACO
    Path* tournee = Graph_tspFromACO(sousgraph, 0, 1000, 100, 2.0f, 3.0f, 0.1f, 2.0f);
    printf("%.1f %d\n", tournee->distance, (tournee->list->nodeCount));
    ListIntNode* node = tournee->list->sentinel.next;
    for (int i = 0; i < tournee->list->nodeCount; i++)
    {
        printf("%d ", node->value);
        node = node->next;
    }
    printf("\n");

    ///////////// Creation du geojson

    affichage_json(nb_points, PathArray, tournee, array_points, interArray);


    //////////////// liberation de la memoire

    free(array_points);

    for (int i = 0; i < size; i++)
    {
        free(interArray[i]);
    }
    free(interArray);

    for (int i = 0; i < nb_points; i++)
    {
        for (int j = 0; j < nb_points; j++)
        {
            Path_destroy(PathArray[i][j]);
        }
        free(PathArray[i]);
    }
    free(PathArray);

    Path_destroy(tournee);

    Graph_destroy(graph);
    Graph_destroy(sousgraph);

    fclose(finput);
    fclose(finter);
#endif
}

