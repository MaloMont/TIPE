#include "decompression.h"

#include <stdlib.h>
#include <stdio.h>

CanalPixel** redimensionneZoneRect(double** zone, int largeur, int hauteur)
{
    int nl = largeur / 2;
    int nh = hauteur / 2;
    CanalPixel **nouvelleZone = malloc(nl * sizeof(CanalPixel*));

    for(int i = 0 ; i < nl ; ++i)
    {
        nouvelleZone[i] = malloc(nh * sizeof(CanalPixel));
        for(int j = 0 ; j < nh ; ++j)
        {
            nouvelleZone[i][j] = (CanalPixel)((zone[i * 2][j * 2] + zone[i * 2 + 1][j * 2]
                                + zone[i * 2][j * 2 + 1] + zone[i * 2 + 1][j * 2 + 1]) / 4.);
        }
    }

    return nouvelleZone;
}

void iter(const Fonction* ifs, CanalImage *canal, const int sizeRB)
{
    double** aux = malloc(2*canal->largeur * sizeof(double*));
    for(int i = 0; i<2*canal->largeur; i++)
        aux[i] = calloc(2*canal->hauteur, sizeof(double));
    
    for(int j = 0; j<100; j++){
        for(int x = 0; x<2*canal->largeur; x++)
            for(int y = 0; y<2*canal->hauteur; y++)
            {
                int i = (x/2)/sizeRB + ((y/2)/sizeRB)*(canal->largeur/sizeRB);
                //printf("(%d, %d)[RB %d] size: %d ; largeur: %d ; ifs y : %d\n", x, y, i, sizeRB, canal->largeur, ifs[i].y);
                aux[x][y] = (double) ifs[i].pente * aux[x + 2*ifs[i].x][y + 2*ifs[i].y] + ifs[i].y0;
            }
    }

    for(int i = 0 ; i < canal->largeur ; ++i)
        free(canal->data[i]);
    free(canal->data);

    canal->data = redimensionneZoneRect(aux, 2*canal->largeur, 2*canal->hauteur);

    for(int i = 0; i<2*canal->largeur; i++)
        free(aux[i]);
    free(aux);
}

CanalImage decompresseCanal(const Fonction* ifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter)
{
    CanalImage canal;
    canal.largeur = largeur;
    canal.hauteur = hauteur;
    canal.data = malloc(largeur * sizeof(CanalPixel*));
    for(int i = 0; i<largeur; i++){
        canal.data[i] = malloc(hauteur * sizeof(CanalPixel));
        for(int j = 0; j<hauteur; j++)
            canal.data[i][j] = 122;
    }

    for(int i = 0; i<nbIter; i ++)
    {
        printf("itération %d sur %d.\n", i+1, nbIter);
        iter(ifs, &canal, sizeRB);
    }

    return canal;
}

Image decompresseImage(Fonction** tabifs, const int RBsize, const int largeur, const int hauteur, const int nbCanaux, const int nbIter)
{
    Image img = imageVide(largeur, hauteur, 4);
    for(int i = 0; i<nbCanaux; i++){
        CanalImage reconstruite = decompresseCanal(tabifs[i], RBsize, largeur, hauteur, nbIter);
        remplaceCanal(img, i, reconstruite);
        freeCanalImage(&reconstruite);
    }

    for(int i = nbCanaux; i<4; i++)
        for(int x = 0; x<largeur; x++)
            for(int y = 0; y<hauteur; y++)
                img.data[i][x][y] = 255;
    
    return img;
}
