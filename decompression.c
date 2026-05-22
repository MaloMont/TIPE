#include "decompression.h"

void iter(const Fonction* ifs, CanalImage *canal, const int sizeRB)
{
    CanalImage aux;
    aux.largeur = 2*canal->largeur;
    aux.hauteur = 2*canal->hauteur;
    aux.data = malloc(aux.largeur * sizeof(CanalPixel*));
    for(int i = 0; i<aux.largeur; i++)
        aux.data[i] = malloc(aux.hauteur * sizeof(CanalPixel));

    for(int x = 0; x<aux.largeur; x++)
        for(int y = 0; y<aux.hauteur; y++)
        {
            int i = x/sizeRB + (y/sizeRB)*(canal->largeur/sizeRB);
            aux.data[x][y] = ifs[i].pente * canal->data[x + ifs[i].x][y + ifs[i].y] + ifs[i].y0;
        }

    libereTableau2D(canal->data);

    canal->data = redimensionneZone(aux.data, 2*sizeRB, sizeRB);

    libereTableau2D(aux.data);
}

CanalImage decompresseCanal(const Fonction* ifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter)
{
    CanalImage canal;
    canal.largeur = largeur;
    canal.hauteur = hauteur;
    canal.data = malloc(largeur * sizeof(CanalPixel*));
    for(int i = 0; i<largeur; i++){
        canal.data[i] = malloc(hauteur * sizeof(CanalPixel));
        for(int j = 0; j<hauteur; j++)
            canal.data[i][j] = 122;
    }

    for(int i = 0; i<nbIter; i ++)
    {
        iter(ifs, &canal, sizeRB);
    }

    return canal;
}

Image decompresseImage(const Fonction** tabifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter)
{
    Image img;
    img.nbCanaux = 4;
    img.hauteur = hauteur;
    img.largeur = largeur;
    img.data = malloc(largeur * sizeof(Pixel*));
    for(int i = 0; i<largeur; i++)
        img.data[i] = malloc(hauteur * sizeof(Pixel));

    CanalImage rouge = decompresseCanal(tabifs[0], sizeRB, hauteur, largeur, nbIter);
    CanalImage vert = decompresseCanal(tabifs[1], sizeRB, hauteur, largeur, nbIter);
    CanalImage bleu = decompresseCanal(tabifs[2], sizeRB, hauteur, largeur, nbIter);
    CanalImage alpha = decompresseCanal(tabifs[3], sizeRB, hauteur, largeur, nbIter);

    for(int x = 0; x<largeur, x++)
        for(int y = 0; y<hauteur; y++)
            img.data[x][y] = {rouge.data[x][y], vert.data[x][y], bleu.data[x][y], alpha.data[x][y]};

    libereTableau2D(rouge.data);
    libereTableau2D(vert.data);
    libereTableau2D(bleu.data);
    libereTableau2D(alpha.data);

    return img;
}