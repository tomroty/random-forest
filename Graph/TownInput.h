#pragma once
#include "Settings.h"

typedef struct csv
{
    char code_commune_INSEE[6];
    char nom_commune_postal[128];
    char code_postal[6];
    char libelle_acheminement[128];
    char ligne_5[128];
    char latitude[128];
    char longitude[128];
    char code_commune[6];
    char article[6];
    char nom_commune[128];
    char nom_commune_complet[128];
    char code_departement[6];
    char nom_departement[128];
    char code_region[6];
    char nom_region[128];
} csv;

//Charge le fichier config.txt
csv** InputFetch();

//Charge le CSV, cherche les villes correspondates de TownArray et renvoie un tableau de la struct CSV
csv** GetTownFromCSV(char** TownArray, int nb_villes);