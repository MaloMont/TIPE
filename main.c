#include <stdlib.h>
#include <stdio.h>

#include "image_io.h"
#include "range_blocks.h"
#include "domain_blocks.h"
#include "decompression.h"
#include "IFS.h"

Fonction* trouveIFS(CanalImage img, const int RBsize, int *nbRB, int pas)
{
    printf("trouveIFS.\n");
    RangeBlock *rangeBlocks = trouveRBs(img, RBsize, nbRB);
    Fonction *ifs = malloc((*nbRB) * sizeof(Fonction));

    for(int i = 0 ; i < (*nbRB) ; ++i)
    {
        if(i % 1000 == 0)
            printf("range block %d sur %d\n", i, *nbRB);
        ifs[i] = trouveDB(img, rangeBlocks[i], RBsize, pas);
    }

    free(rangeBlocks);

    return ifs;
}

IFS compresseImage(const char* fichierEntree, int pas, int RBsize)
{
    Image img = chargeCarre(fichierEntree, RBsize);
    
    int nbFonctions;
    int nbCanaux = 3;

    IFS ifs = {img.largeur, img.hauteur, 0, nbCanaux, RBsize, malloc(nbCanaux * sizeof(Fonction*))};

    CanalImage canal = extraitCanal(img, CANAL_ROUGE);
    ifs.fs[0] = trouveIFS(canal, RBsize, &nbFonctions, pas);
    freeCanalImage(&canal);

    canal = extraitCanal(img, CANAL_VERT);
    ifs.fs[1] = trouveIFS(canal, RBsize, &nbFonctions, pas);
    freeCanalImage(&canal);

    canal = extraitCanal(img, CANAL_BLEU);
    ifs.fs[2] = trouveIFS(canal, RBsize, &nbFonctions, pas);
    freeCanalImage(&canal);

    ifs.nbFonctions = nbFonctions;

    freeImage(&img);

    return ifs;
}

int main()
{
    int compression = 1;
    printf("compresser ? (1 pour oui, 0 pour non)\n> ");
    scanf("%d", &compression);

    char fichierIFS[100];
    printf("fichier de destination pour la compression (par exemple out.ifs) :\n> ");
    scanf("%s", fichierIFS);

    if(compression == 1)
    {
        char fichierEntree[100];
        printf("image à compresser :\n> ");
        scanf("%s", fichierEntree);
        

        int pas = 1;
        printf("pas pour la recherche de domain blocks :\n> ");
        scanf("%d", &pas);

        int RBsize = 10;
        printf("taille pour les range blocks :\n> ");
        scanf("%d", &RBsize);

        IFS ifs = compresseImage(fichierEntree, pas, RBsize);
        encodeIFS(fichierIFS, ifs);

        freeIFS(ifs);
    }


    printf("\n=======================\n");
    printf("= début décompression =\n");
    printf("=======================\n");

    int nbIter = 200;
    printf("nbIter :\n> ");
    scanf("%d", &nbIter);

    int scale = 1;
    printf("facteur d'agrandissement :\n> ");
    scanf("%d", &scale);

    char fichierSortie[100];
    printf("image de sortie pour la décompression (en .bmp):\n> ");
    scanf("%s", fichierSortie);

    IFS alt = decodeIFS(fichierIFS, scale);

    Image reconstruite = decompresseImage(alt.fs, alt.RBsize, alt.largeur, alt.hauteur, alt.nbCanaux, nbIter);
    sauveFichier(reconstruite, fichierSortie);

    freeImage(&reconstruite);
    freeIFS(alt);

    printf("fin.\n");
}
