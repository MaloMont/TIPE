#include "decompression.h"

#include <stdlib.h>
#include <stdio.h>

void iter(const Fonction* ifs, CanalImage *canal, const int sizeRB)
{
    CanalImage aux;
    aux.largeur = 2*canal->largeur;
    aux.hauteur = 2*canal->hauteur;
    aux.data = malloc(aux.largeur * sizeof(CanalPixel*));
    for(int i = 0; i<aux.largeur; i++)
        aux.data[i] = malloc(aux.hauteur * sizeof(CanalPixel));

    for(int x = 0; x<aux.largeur; x++)
        for(int y = 0; y<aux.hauteur; y++)
        {
            int i = (x/2)/sizeRB + ((y/2)/sizeRB)*(canal->largeur/sizeRB);
            //printf("(%d, %d)[RB %d] size: %d ; largeur: %d ; ifs y : %d\n", x, y, i, sizeRB, canal->largeur, ifs[i].y);
            aux.data[x][y] = (CanalPixel)(double) ifs[i].pente * canal->data[(x/2) + ifs[i].x][(y/2) + ifs[i].y] + ifs[i].y0;
        }

    for(int i = 0 ; i < canal->largeur ; ++i)
        free(canal->data[i]);
    free(canal->data);

    canal->data = redimensionneZoneRectangle(aux.data, aux.largeur, aux.hauteur);

    freeCanalImage(&aux);
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

Image decompresseImage(const Fonction** tabifs, const int RBsize, const int largeur, const int hauteur, const int nbCanaux, const int nbIter)
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
