#include <stdlib.h>
#include <stdio.h>

#include "image_io.h"
#include "range_blocks.h"
#include "domain_blocks.h"
#include "decompression.h"

Fonction* trouveIFS(CanalImage img, const int RBsize)
{
    int nbRB = 0;
    RangeBlock *rangeBlocks = trouveRBs(img, RBsize, &nbRB);
    Fonction *ifs = malloc(nbRB * sizeof(Fonction));

    for(int i = 0 ; i < nbRB ; ++i)
    {
        //printf("\ndébut recherche RB (%d sur %d) : \n", i, nbRB);
        ifs[i] = trouveDB(img, rangeBlocks[i], RBsize);
    }

    free(rangeBlocks);

    return ifs;
}

int main()
{
    const int RBsize = 32;

    const char *fichierEntree = "images/arch_logo.png";
    Image img = chargeCarre(fichierEntree, RBsize);

    Fonction *ifsRouge = trouveIFS(extraitCanal(img, CANAL_ROUGE), RBsize);

    printf(" \n=======================\n ");
    printf(" = début décompression =\n ");
    printf(" =======================\n ");

    CanalImage reconstruiteRouge = decompresseCanal(ifsRouge, RBsize, img.largeur, img.hauteur, 20);
    Image reconstruite = imageVide(img.largeur, img.hauteur, img.nbCanaux);
    remplaceCanal(reconstruite, CANAL_ROUGE, reconstruiteRouge);

    const char *fichierSortie = "images/created.bmp";
    sauveFichier(reconstruite, fichierSortie);

    free(ifsRouge);
    freeImage(&img);
    freeCanalImage(&reconstruiteRouge);
    freeImage(&reconstruite);

    printf("fin.\n");
}
