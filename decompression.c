#include "decompression.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

void iter(const Fonction* ifs, CanalImage *canal, const int sizeRB, const int nbIter)
{
    srand(time(NULL));

    double*** aux = malloc(2 * sizeof(double**));
    for(int j = 0 ; j < 2 ; ++j)
    {
        aux[j] = malloc(canal->largeur * sizeof(double*));
        for(int i = 0; i<canal->largeur; i++)
        {
            aux[j][i] = calloc(canal->hauteur, sizeof(double));
            for(int k = 0 ; k < canal->hauteur ; ++k)
                aux[j][i][k] = rand() % 255;
        }
    }
    
    for(int j = 0; j<nbIter; j++){
        for(int x = 0; x<canal->largeur; x++)
            for(int y = 0; y<canal->hauteur; y++)
            {
                int i = x/sizeRB + (y/sizeRB)*(canal->largeur/sizeRB);
                int xinRB = x%sizeRB, yinRB = y%sizeRB;
                int xDB = x - xinRB + ifs[i].x, yDB = y - yinRB + ifs[i].y;
                double moyenne = (aux[(j+1) % 2][xDB + 2*xinRB][yDB + 2*yinRB] 
                                + aux[(j+1) % 2][xDB + 2*xinRB + 1][yDB + 2*yinRB]
                                + aux[(j+1) % 2][xDB + 2*xinRB][yDB + 2*yinRB + 1]
                                + aux[(j+1) % 2][xDB + 2*xinRB + 1][yDB + 2*yinRB + 1]
                                )/4.;
//                printf("(%d, %d)[RB %d] size: %d ; largeur: %d ; ifs y : %d\n", x, y, i, sizeRB, canal->largeur, ifs[i].y);
                aux[j % 2][x][y] = (double) ifs[i].pente * moyenne + ifs[i].y0;
            }
    }


    for(int x = 0; x<canal->largeur; x++)
        for(int y = 0; y<canal->hauteur; y++)
            canal->data[x][y] = (CanalPixel) aux[(nbIter+1)%2][x][y];
    for(int j = 0 ; j < 2 ; ++j)
    {
        for(int i = 0; i<canal->largeur; i++)
            free(aux[j][i]);
        free(aux[j]);
    }
    free(aux);
}

CanalImage decompresseCanal(const Fonction* ifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter)
{
    CanalImage canal;
    canal.largeur = largeur;
    canal.hauteur = hauteur;
    canal.data = malloc(largeur * sizeof(CanalPixel*));
    for(int i = 0; i<largeur; i++)
        canal.data[i] = malloc(hauteur * sizeof(CanalPixel));

    iter(ifs, &canal, sizeRB, nbIter);

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
