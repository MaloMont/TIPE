#include <stdlib.h>
#include <stdio.h>

#include "image_io.h"
#include "range_blocks.h"
#include "domain_blocks.h"
#include "decompression.h"
#include "IFS.h"

Fonction* trouveIFS(CanalImage img, const int RBsize, int *nbRB)
{
    printf("trouveIFS.\n");
    RangeBlock *rangeBlocks = trouveRBs(img, RBsize, nbRB);
    Fonction *ifs = malloc((*nbRB) * sizeof(Fonction));

    for(int i = 0 ; i < (*nbRB) ; ++i)
    {
        ifs[i] = trouveDB(img, rangeBlocks[i], RBsize);
    }

    free(rangeBlocks);

    return ifs;
}

IFS compresseImage(const char* fichierEntree)
{
    const int RBsize = 5;

    Image img = chargeCarre(fichierEntree, RBsize);
    
    int nbFonctions;
    int nbCanaux = 3;

    IFS ifs = {img.largeur, img.hauteur, 0, nbCanaux, RBsize, malloc(nbCanaux * sizeof(Fonction*))};

    CanalImage canal = extraitCanal(img, CANAL_ROUGE);
    ifs.fs[0] = trouveIFS(canal, RBsize, &nbFonctions);
    freeCanalImage(&canal);

    canal = extraitCanal(img, CANAL_VERT);
    ifs.fs[1] = trouveIFS(canal, RBsize, &nbFonctions);
    freeCanalImage(&canal);

    canal = extraitCanal(img, CANAL_BLEU);
    ifs.fs[2] = trouveIFS(extraitCanal(img, CANAL_BLEU), RBsize, &nbFonctions);
    freeCanalImage(&canal);

    ifs.nbFonctions = nbFonctions;

    freeImage(&img);

    return ifs;
}

int main()
{
    const int nbIter = 100;
    const char *fichierEntree = "images/arch_logo.png";

    IFS ifs = compresseImage(fichierEntree);
    encodeIFS("save.ifs", ifs);

    printf("\n=======================\n ");
    printf("= début décompression =\n ");
    printf("=======================\n ");

    IFS alt = decodeIFS("save.ifs");

    Image reconstruite = decompresseImage(alt.fs, alt.RBsize, alt.largeur, alt.hauteur, alt.nbCanaux, nbIter);
    char *fichierSortie = "images/created.bmp";
    sauveFichier(reconstruite, fichierSortie);

    freeImage(&reconstruite);
    freeIFS(ifs);
    freeIFS(alt);

    printf("fin.\n");
}
