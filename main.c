#include <stdlib.h>

#include "image_io.h"
#include "range_blocks.h"
#include "domain_blocks.h"

Fonction* trouveIFS(CanalImage img, const int RBsize)
{
    int nbRB = 0;
    RangeBlock *rangeBlocks = trouveRBs(img, RBsize, &nbRB);
    Fonction *ifs = malloc(nbRB * sizeof(Fonction));

    for(int i = 0 ; i < nbRB ; ++i)
        ifs[i] = trouveDB(img, rangeBlocks[i], RBsize);

    return ifs;
}

int main()
{
    const char *fichierEntree = "images/arch_logo.png";
    Image img = chargeFichier(fichierEntree);

    Fonction *ifsRouge = trouveIFS(canalRouge(img), 8);

    const char *fichierSortie = "images/created.bmp";
    sauveFichier(img, fichierSortie);
    freeImage(&img);
}
