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
        printf("\ndébut recherche RB (%d sur %d) : \n", i+1, nbRB);
        ifs[i] = trouveDB(img, rangeBlocks[i], RBsize);
    }

    free(rangeBlocks);

    return ifs;
}

int main()
{
    const int RBsize = 5;
    const int nbIter = 100;

    const char *fichierEntree = "images/arch_logo.png";
    Image img = chargeCarre(fichierEntree, RBsize);

    Fonction *ifsRouge = trouveIFS(extraitCanal(img, CANAL_ROUGE), RBsize);
    Fonction *ifsVert = trouveIFS(extraitCanal(img, CANAL_VERT), RBsize);
    Fonction *ifsBleu = trouveIFS(extraitCanal(img, CANAL_BLEU), RBsize);

    printf(" \n=======================\n ");
    printf(" = début décompression =\n ");
    printf(" =======================\n ");

    Image reconstruite = imageVide(img.largeur, img.hauteur, img.nbCanaux);
    CanalImage reconstruiteRouge = decompresseCanal(ifsRouge, RBsize, img.largeur, img.hauteur, nbIter);
    remplaceCanal(reconstruite, CANAL_ROUGE, reconstruiteRouge);
    CanalImage reconstruiteV = decompresseCanal(ifsVert, RBsize, img.largeur, img.hauteur, nbIter);
    remplaceCanal(reconstruite, CANAL_VERT, reconstruiteV);
    CanalImage reconstruiteB = decompresseCanal(ifsBleu, RBsize, img.largeur, img.hauteur, nbIter);
    remplaceCanal(reconstruite, CANAL_BLEU, reconstruiteB);

    const char *fichierSortie = "images/created.bmp";
    sauveFichier(reconstruite, fichierSortie);

    free(ifsRouge);
    free(ifsBleu);
    free(ifsVert);
    freeImage(&img);
    freeCanalImage(&reconstruiteRouge);
    freeImage(&reconstruite);

    printf("fin.\n");
}
