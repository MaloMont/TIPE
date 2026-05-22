#include <stdlib.h>
#include <stdio.h>

#include "image_io.h"
#include "range_blocks.h"
#include "domain_blocks.h"

Fonction* trouveIFS(CanalImage img, const int RBsize)
{
    int nbRB = 0;
    RangeBlock *rangeBlocks = trouveRBs(img, RBsize, &nbRB);
    Fonction *ifs = malloc(nbRB * sizeof(Fonction));

    for(int i = 0 ; i < nbRB ; ++i)
    {
        printf("\ndébut recherche RB (%d sur %d) : \n", i, nbRB);
        ifs[i] = trouveDB(img, rangeBlocks[i], RBsize);
    }

    free(rangeBlocks);

    return ifs;
}

int main()
{
    const int RBsize = 32;

    const char *fichierEntree = "images/arch_logo.png";
    Image img = chargeFichier(fichierEntree);

    Fonction *ifsRouge = trouveIFS(canalRouge(img), RBsize);

    const char *fichierSortie = "images/created.bmp";
    sauveFichier(img, fichierSortie);

    free(ifsRouge);
    freeImage(&img);

    printf("fin.\n");
}
