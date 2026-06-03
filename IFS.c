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
            if(ifs.fs[c][i].pente == 0.)
                fprintf(fichier, "%f\n", ifs.fs[c][i].y0);
            else
                fprintf(fichier, "%d %d %f %f\n", ifs.fs[c][i].x, ifs.fs[c][i].y, ifs.fs[c][i].pente, ifs.fs[c][i].y0);
        }
    }
    fclose(fichier);
}

IFS decodeIFS(char *nomFichier, double scale)
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
    resultat.largeur *= scale;
    resultat.hauteur *= scale;
    resultat.RBsize  *= scale;

    resultat.fs = malloc(resultat.nbCanaux * sizeof(Fonction*));

    for(int c = 0 ; c < resultat.nbCanaux ; ++c)
    {
        resultat.fs[c] = malloc(resultat.nbFonctions * sizeof(Fonction));
        for(int i = 0 ; i < resultat.nbFonctions ; ++i)
        {
            char buffer[50];
            fscanf(fichier,"%s", buffer);
            if(strlen(buffer) > 6){
                resultat.fs[c][i].y0 = atof(buffer);
                resultat.fs[c][i].pente = 0.;
                int nbRBparLigne = resultat.largeur/resultat.RBsize;
                resultat.fs[c][i].y = - (i / nbRBparLigne) * resultat.RBsize;
                resultat.fs[c][i].x = - (i % nbRBparLigne) * resultat.RBsize;
            }else{
                resultat.fs[c][i].x = atoi(buffer);
                fscanf(fichier, "%d %lf %lf\n", &resultat.fs[c][i].y, &resultat.fs[c][i].pente, &resultat.fs[c][i].y0);
            }
            resultat.fs[c][i].x *= scale;
            resultat.fs[c][i].y *= scale;
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
