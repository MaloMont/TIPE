#include <stdio.h>
#include <stdlib.h>

#include "domain_blocks.h"
#include "range_blocks.h"
#include "image_io.h"
#include "linreg.h"

/* prend un tableau 2D de pixels et le transforme en tableau 1D : [x][y] devient [x * w + y] */
double* convertitTableau(CanalPixel **tableau2D, int largeur, int hauteur)
{
    double *tableau1D = malloc(largeur * hauteur * sizeof(double));
    for(int i = 0 ; i < largeur ; ++i)
    {
        for(int j = 0 ; j < hauteur ; ++j)
        {
            tableau1D[i * largeur + j] = tableau2D[i][j];
        }
    }
    return tableau1D;
}

void libereTableau2D(CanalPixel **T, int largeur)
{
    for(int i = 0 ; i < largeur ; ++i)
        free(T[i]);
    free(T);
}

/* ça se voit */
Fonction trouveDB(CanalImage img, RangeBlock cible, int RBsize)
{
    //printf("searching for RB (%d %d, %d %d)", cible.x, cible.y, cible.x + RBsize, cible.y + RBsize);
    double *valeursRB = valeursZone1D(img, cible.x, cible.y, RBsize);

    int DBsize = 2 * RBsize;
    Fonction fonction = {0, 0, 0, 0};
    double bestEcart = 1000000.;

    int pas = 4;

    for(int xDB = 0 ; xDB + DBsize < img.largeur ; xDB += pas)
    {
        for(int yDB = 0 ; yDB + DBsize < img.hauteur ; yDB += pas)
        {
            double *valeursDB = valeursZone1D(img, xDB, yDB, DBsize);
            redimensionneEnPlace1D(valeursDB, DBsize, RBsize);

            double ecart = bestEcart + 1., pente = 0., y0 = 0.;
            if(linreg(RBsize * RBsize, valeursDB, valeursRB, &pente, &y0, &ecart) != 0)
                printf("WARNING: la régression linéaire a levé une erreur.\n");

            else if(ecart < bestEcart)
            {
                bestEcart = ecart;
                fonction.x = xDB - cible.x;
                fonction.y = yDB - cible.y;
                fonction.pente = pente;
                fonction.y0 = y0;
            }

            free(valeursDB);

            if(ecart == 0)
            {
                free(valeursRB);
                printf("%d %d %f %f\n", fonction.x, fonction.y, fonction.pente, fonction.y0);
                return fonction;
            }
        }
    }    

    free(valeursRB);
    printf("%d %d %f %f\n", fonction.x, fonction.y, fonction.pente, fonction.y0);
    return fonction;
}
