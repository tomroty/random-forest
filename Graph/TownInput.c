#include "TownInput.h"
#include "stdlib.h"
#include "ctype.h"


csv** InputFetch()
{

	printf("Chargement du fichier config...\n");
	FILE* finput = fopen("../config.txt", "r");
	assert(finput);

	char buffer[128];
	fgets(buffer, 128, finput);
	int nb_villes = -1;

	fscanf(finput, "%d\n", &nb_villes);

	if (!(nb_villes > 0 && nb_villes < 100))
	{
		printf("Nombre de villes incorrect.\n");
		abort();
	}

	FILE* tmp = fopen("../BonusData/tmp.txt", "w");
	fprintf(tmp, "../Data/france_graph.txt\n");
	fprintf(tmp, "../Data/france_inter.txt\n");
	fprintf(tmp, "%d\n", nb_villes);
	fclose(tmp);

	fgets(buffer, 128, finput);

	char** bufferArray = (char**)calloc(nb_villes, sizeof(char*));
	for (int i = 0; i < nb_villes; i++)
	{
		bufferArray[i] = (char*)calloc(128, sizeof(char));
		fscanf(finput, "%[^\n]\n", bufferArray[i]);

		/////// pour que linux soit content  
		int length = strlen(bufferArray[i]);
		while (length > 0)
		{
			if (isspace(bufferArray[i][length - 1]))
			{
				bufferArray[i][length - 1] = '\0';
				length--;
			}
			else
				break;
		}
	}
	csv** csvArray = GetTownFromCSV(bufferArray, nb_villes);

	for (int i = 0; i < nb_villes; i++)
	{
		free(bufferArray[i]);
	}
	free(bufferArray);

	return csvArray;

}

csv** GetTownFromCSV(char** TownArray, int nb_villes)
{
	printf("Chargement du fichier CSV...\n");
	FILE* finput = fopen("../BonusData/communes-departement-region.csv", "r");
	assert(finput);
	fseek(finput, 0, SEEK_END);
	long taille_fichier = ftell(finput);
	fseek(finput, 0, SEEK_SET);
	char* buff = calloc(taille_fichier, sizeof(char));
	fread(buff, 1, taille_fichier, finput);

	

	csv** csvArray = calloc(nb_villes, sizeof(csv*));
	for (int i = 0; i < nb_villes; i++)
	{
		csvArray[i] = calloc(1, sizeof(csv));
	}

	bool* explored = calloc(nb_villes, sizeof(bool));
	int i = 0;;
	while (i < taille_fichier)
	{
		char struc_tmp[15][1024] = { 0 };
		for (int taille = 0; taille < 15; taille++)
		{
			int j = 0;
			while ((i + j) < taille_fichier && ((buff[i + j] != ',') && (buff[i + j] != '\n')))
				j++;

			for (int k = 0; k < j; k++)
			{
				struc_tmp[taille][k] = buff[i + k];

			}
			i += j + 1;
		}
		for (int j = 0; j < nb_villes; j++)
		{
			if (!(strcmp(struc_tmp[1], TownArray[j])) && explored[j] == false)
			{
				explored[j] = true;
				strcpy(csvArray[j]->code_commune_INSEE, struc_tmp[0]);
				strcpy(csvArray[j]->nom_commune_postal, struc_tmp[1]);
				strcpy(csvArray[j]->code_postal, struc_tmp[2]);
				strcpy(csvArray[j]->libelle_acheminement, struc_tmp[3]);
				strcpy(csvArray[j]->ligne_5, struc_tmp[4]);
				strcpy(csvArray[j]->latitude, struc_tmp[5]);
				strcpy(csvArray[j]->longitude, struc_tmp[6]);
				strcpy(csvArray[j]->code_commune, struc_tmp[7]);
				strcpy(csvArray[j]->article, struc_tmp[8]);
				strcpy(csvArray[j]->nom_commune, struc_tmp[9]);
				strcpy(csvArray[j]->nom_commune_complet, struc_tmp[10]);
				strcpy(csvArray[j]->code_departement, struc_tmp[11]);
				strcpy(csvArray[j]->nom_departement, struc_tmp[12]);
				strcpy(csvArray[j]->code_region, struc_tmp[13]);
				strcpy(csvArray[j]->nom_region, struc_tmp[14]);

				#if 0 //Print complet
					printf("code_commune_INSEE: %s\n", csvArray[j]->code_commune_INSEE);
					printf("nom_commune_postal: %s\n", csvArray[j]->nom_commune_postal);
					printf("code_postal: %s\n", csvArray[j]->code_postal);
					printf("libelle_acheminement: %s\n", csvArray[j]->libelle_acheminement);
					printf("ligne_5: %s\n", csvArray[j]->ligne_5);
					printf("latitude: %s\n", csvArray[j]->latitude);
					printf("longitude: %s\n", csvArray[j]->longitude);
					printf("code_commune: %s\n", csvArray[j]->code_commune);
					printf("article: %s\n", csvArray[j]->article);
					printf("nom_commune: %s\n", csvArray[j]->nom_commune);
					printf("nom_commune_complet: %s\n", csvArray[j]->nom_commune_complet);
					printf("code_departement: %s\n", csvArray[j]->code_departement);
					printf("nom_departement: %s\n", csvArray[j]->nom_departement);
					printf("code_region: %s\n", csvArray[j]->code_region);
					printf("nom_region: %s\n", csvArray[j]->nom_region);
					printf("\n");
				#endif
			}
		}
	}
	
	

	free(buff);
	free(explored);
	fclose(finput);
	return csvArray;
}