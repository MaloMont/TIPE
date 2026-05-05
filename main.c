#include <stdio.h>

#include "image_io.h"

int main()
{
    const char *fichierEntree = "images/arch_logo.png";
    Image img = chargeFichier(fichierEntree);
    for(int i = 0 ; i < img.largeur ; ++i)
    {
        for(int j = 0 ; j < img.hauteur ; ++j)
        {
            img.data[i][j].blue = 100;
            img.data[i][j].red = 150;
            img.data[i][j].green = 100;
        }
    }

    const char *fichierSortie = "images/created.bmp";
    sauveFichier(img, fichierSortie);
    freeImage(&img);
}


