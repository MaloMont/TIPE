#include <stdio.h>
#include <stdlib.h>

#include "domain_blocks.h"
#include "range_blocks.h"
#include "image_io.h"
#include "linreg.h"


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
}

void libereTableau2D(CanalPixel **T, int largeur)
{
    for(int i = 0 ; i < largeur ; ++i)
        free(T[i]);
    free(T);
}

Fonction trouveDB(CanalImage img, RangeBlock cible, int RBsize)
{
    CanalPixel **pixelsRB = pixelsZone(img, cible.x, cible.y, RBsize);
    double *valeursRB = convertitTableau(pixelsRB, RBsize, RBsize);
    libereTableau2D(pixelsRB, img.largeur);

    int DBsize = 2 * RBsize;
    Fonction fonction = {0, 0, 0, 0};
    double bestR = 0;

    for(int x = 0 ; x + DBsize < img.largeur ; ++x)
    {
        for(int y = 0 ; y + DBsize < img.hauteur ; ++y)
        {
            // TODO: pas opti
            CanalPixel **pixelsDB = pixelsZone(img, x, y, DBsize);
            CanalPixel **pixelsDBredim = redimensionneZone(pixelsDB, DBsize, RBsize);
            double *valeursDB = convertitTableau(pixelsDBredim, RBsize, RBsize);
            libereTableau2D(pixelsDB, DBsize);
            libereTableau2D(pixelsDBredim, RBsize);

            double r = 0, pente = 0, y0 = 0;
            if( !linreg(RBsize * RBsize, valeursDB, valeursRB, &pente, &y0, &r) )
                printf("WARNING: la régression linéaire a levé une erreur.\n");

            if(r < bestR)
            {
                bestR = r;
                fonction.x = x;
                fonction.y = y;
                fonction.pente = pente;
                fonction.y0 = y0;
            }
        }
    }
    return fonction;
}
