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
    printf("searching for RB (%d %d, %d %d)", cible.x, cible.y, cible.x + RBsize, cible.y + RBsize);
    CanalPixel **pixelsRB = pixelsZone(img, cible.x, cible.y, RBsize);
    double *valeursRB = convertitTableau(pixelsRB, RBsize, RBsize);
    libereTableau2D(pixelsRB, RBsize);

    int DBsize = 2 * RBsize;
    Fonction fonction = {0, 0, 0, 0};
    double bestError = 0;

    for(int xDB = 0 ; xDB + DBsize < img.largeur ; ++xDB)
    {
        for(int yDB = 0 ; yDB + DBsize < img.hauteur ; ++yDB)
        {
            // TODO: pas opti
            CanalPixel **pixelsDB = pixelsZone(img, xDB, yDB, DBsize);
            CanalPixel **pixelsDBredim = redimensionneZone(pixelsDB, DBsize, RBsize);
            double *valeursDB = convertitTableau(pixelsDBredim, RBsize, RBsize);
            libereTableau2D(pixelsDB, DBsize);
            libereTableau2D(pixelsDBredim, RBsize);

            double error = 0, pente = 0, y0 = 0;
            if( linreg(RBsize * RBsize, valeursDB, valeursRB, &pente, &y0, &error) != 0)
                printf("WARNING: la régression linéaire a levé une erreur.\n");

            else if(error < bestError)
            {
                bestError = error;
                fonction.x = xDB - cible.x;
                fonction.y = yDB - cible.y;
                fonction.pente = pente;
                fonction.y0 = y0;
            }

            free(valeursDB);

            if(error == 0)
            {
                free(valeursRB);
                return fonction;
            }
        }
    }    

    free(valeursRB);

    return fonction;
}
