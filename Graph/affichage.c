#include "affichage.h"

void affichage_json(int nb_points, Path*** PathArray, Path *tournee, int* array_points, float** interArray)
{
    FILE* pfile = fopen("../output.geojson", "w");
    ListIntNode *parcourt = tournee->list->sentinel.next;
    fprintf(pfile, "{\n");
    fprintf(pfile, "    \"type\": \"FeatureCollection\",\n");
    fprintf(pfile, "    \"features\": [\n");
    for (int i = 0; i < nb_points; i++)
    {
        fprintf(pfile, "        {\n");
        fprintf(pfile, "            \"type\": \"Feature\",\n");
        fprintf(pfile, "            \"properties\": {\n");
        fprintf(pfile, "                \"_umap_options\": {\n");
        fprintf(pfile, "                    \"color\": \"#2196F3\",\n");
        fprintf(pfile, "                    \"weight\": 6,\n");
        fprintf(pfile, "                    \"iconClass\": \"Drop\", \n");
        fprintf(pfile, "                    \"showLabel\": null\n");
        fprintf(pfile, "                },\n");
        fprintf(pfile, "                \"name\": \"Point %d\"\n", i);
        fprintf(pfile, "            }, \"geometry\": {\n");
        fprintf(pfile, "                \"type\": \"Point\", \n");
        fprintf(pfile, "                \"coordinates\": [%f,%f]\n", interArray[array_points[i]][0], interArray[array_points[i]][1]);
        fprintf(pfile, "            }\n");
        fprintf(pfile, "        },\n");
    }
    for (int i = 0; i < nb_points; i++)
    {
        fprintf(pfile, "        {\n");
        fprintf(pfile, "            \"type\": \"Feature\",\n");
        fprintf(pfile, "            \"properties\": {\n");
        fprintf(pfile, "                \"_umap_options\": {\n");
        fprintf(pfile, "                    \"color\": \"#2196F3\",\n");
        fprintf(pfile, "                    \"weight\": 6,\n");
        fprintf(pfile, "                    \"showLabel\": null\n");
        fprintf(pfile, "                },\n");
        fprintf(pfile, "                \"name\": \"From %d to %d\"\n", parcourt->value , parcourt->next->value);
        fprintf(pfile, "            }, \"geometry\": {\n");
        fprintf(pfile, "                \"type\": \"LineString\", \n");
        fprintf(pfile, "                \"coordinates\": [\n");
        fprintf(pfile, "                    ");
        ListInt* list = PathArray[parcourt->value][parcourt->next->value]->list;
        ListIntNode sentinel = list->sentinel;
        ListIntNode* node = sentinel.next;
        for (int k = 0; k < list->nodeCount; k++)
        {
            if (k != list->nodeCount - 1)
                fprintf(pfile, "[%f,%f], ", interArray[node->value][0], interArray[node->value][1]);
            else
                fprintf(pfile, "[%f,%f]\n", interArray[node->value][0], interArray[node->value][1]);
            node = node->next;
        }
        fprintf(pfile, "                ]\n");
        fprintf(pfile, "            }\n");
        if (i != nb_points - 1)
            fprintf(pfile, "        },\n");
        else
            fprintf(pfile, "        }\n");
        parcourt = parcourt->next;
    }
    fprintf(pfile, "    ]\n");
    fprintf(pfile, "}\n");

    fclose(pfile);

}
