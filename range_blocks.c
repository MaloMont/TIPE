#include <stdlib.h>
#include <stdio.h>

#include "range_blocks.h"
#include "image_io.h"

// Pour une image dont les dimensions sont des multiples de size
RangeBlock* trouveRBs(CanalImage img, int size, int *nbRB)
{
    int nbParLigne = img.largeur / size, nbParColonne = img.hauteur / size;
    *nbRB = nbParLigne * nbParColonne;
    RangeBlock *partition = malloc(nbParLigne * nbParColonne * sizeof(RangeBlock));

    printf("taille : %d x %d.\n", img.largeur, img.hauteur);
    printf("range blocks :\n");
    for(int i = 0 ; i < nbParLigne ; ++i)
    {
        for(int j = 0 ; j < nbParColonne ; ++j)
        {
            partition[i + nbParLigne * j].x = i * size;
            partition[i + nbParLigne * j].y = j * size;
        }
    }

    return partition;
}

