#include "IFS.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Spécification des fichiers de stockage :
 * ligne 1 : nbCanaux.
 * ligne 2 : le nombre de fonctions par canal
 * ligne 3 : la taille d'un range block
 * ligne 4 : largeur hauteur
 * nbCanaux fois : 
 *    autant de lignes que de fonctions sous la forme : x y pente y0 [symétrie](plus tard)
 */

/**
 * sauvegarde l'ifs obtenu dans un fichier
 */
void encodeIFS(char *nomFichier, const IFS ifs)
{
    // overwrite le fichier
    FILE *fichier = fopen(nomFichier, "w");
    if(!fichier)
    {
        printf("ERREUR: le fichier %s n'a pas pu être ouvert pour l'encodage.\n", nomFichier);
        return;
    }

    fprintf(fichier, "%d\n", ifs.nbCanaux);
    fprintf(fichier, "%d\n", ifs.nbFonctions);
    fprintf(fichier, "%d\n", ifs.RBsize);
    fprintf(fichier, "%d %d\n", ifs.largeur, ifs.hauteur);

    for(int c = 0 ; c < ifs.nbCanaux ; ++c)
    {
        for(int i = 0 ; i < ifs.nbFonctions ; ++i)
        {
            fprintf(fichier, "%d %d %f %f\n", ifs.fs[c][i].x, ifs.fs[c][i].y, ifs.fs[c][i].pente, ifs.fs[c][i].y0);
        }
    }
    fclose(fichier);
}

IFS decodeIFS(char *nomFichier)
{
    FILE *fichier = fopen(nomFichier, "r");
    if(!fichier)
    {
        printf("ERREUR: le fichier %s n'a pas pu être ouvert pour le décodage.\n", nomFichier);
        return (IFS){};
    }

    IFS resultat;

    fscanf(fichier, "%d\n%d\n%d\n", &resultat.nbCanaux, &resultat.nbFonctions, &resultat.RBsize);
    fscanf(fichier, "%d %d\n", &resultat.largeur, &resultat.hauteur);

    /* doublement de la taille */
    // resultat.largeur *= 2;
    // resultat.hauteur *= 2;
    // resultat.RBsize  *= 2;

    resultat.fs = malloc(resultat.nbCanaux * sizeof(Fonction*));

    for(int c = 0 ; c < resultat.nbCanaux ; ++c)
    {
        resultat.fs[c] = malloc(resultat.nbFonctions * sizeof(Fonction));
        for(int i = 0 ; i < resultat.nbFonctions ; ++i)
        {
            fscanf(fichier, "%d %d %lf %lf\n", &resultat.fs[c][i].x, &resultat.fs[c][i].y, &resultat.fs[c][i].pente, &resultat.fs[c][i].y0);
            // resultat.fs[c][i].x *= 2;
            // resultat.fs[c][i].y *= 2;
        }
    }

    fclose(fichier);

    return resultat;
}

void freeIFS(IFS ifs)
{
    for(int c = 0 ; c < ifs.nbCanaux ; ++c)
        free(ifs.fs[c]);
    free(ifs.fs);
}
