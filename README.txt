
# README TP FINAL

## REPARTITION

### Tom:

- Tas binaire
- Dijkstra
- Extraction des graphes
- Graph_acoConstructPath
- Bonus

### Timothée:
- Geojson
- Glouton
- ACO sauf Graph_acoConstructPath
- Bonus
- Libération de la mémoire

## FONCTIONNALITES:
- Toutes les fonctions du TP de base
- Création d'une tournée personnalisée grâce au ficher config.txt

### Bonus:

- Il faut écrire le nom en majuscule, sans accents, sans apostrophes et sans tirets (mais les espaces pris en compte)
- Cela doit correspondre au champ nom_commune_postal du fichier ./BonusData/communes-departement-region.csv
- Le programme retourne un geojson à la racine du projet
 (note: Les "Saint" sont abrégés en "ST")

-----
#### Format config.txt:
Combien de villes souhaitez-vous visiter : 
NOMBRE_DE_VILLES
Nom des villes :
VILLE_1
VILLE_2
...
VILLE_N-1
VILLE_N
-----
####  Exemple config.txt: 
Combien de villes souhaitez-vous visiter :
4
Nom des villes :
DINAN
ST MALO
LA GARNACHE
L EPINE AUX BOIS
-----


## Détails sur l’exécution:

### Fichiers
Le bonus a besoin de:
    ./Data/france_graph.txt
    ./Data/france_inter.txt

###  Coût d'execution
~30s - 1min en fonction du nombre de points
~600mb de ram en release (~1,4 GO en debug)


### Commande de compilation Linux:

gcc *.c -Wall -g -lm
