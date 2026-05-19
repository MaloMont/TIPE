#include <assert.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_write.h"

#include "image_io.h"
#include "domain_blocks.h"

Image chargeFichier(const char* fichier)
{
    Image img;
	Pixel* tmp = (Pixel*)stbi_load(fichier, &img.largeur, &img.hauteur, &img.nbCanaux, 0);
    img.data = malloc(img.largeur * sizeof(Pixel*));
    for(int i = 0 ; i < img.largeur ; ++i)
    {
        img.data[i] = malloc(img.hauteur * sizeof(Pixel));
        for(int j = 0 ; j < img.hauteur ; ++j)
            img.data[i][j] = tmp[i * img.largeur + j];
    }
    free(tmp);
    return img;
}

void sauveFichier(Image img, const char* fichier)
{
    Pixel* data = malloc(img.largeur * img.hauteur * sizeof(Pixel));

    for(int i = 0; i < img.largeur; i++)
        for(int j = 0; j < img.hauteur; j++)
            data[i * img.largeur + j] = img.data[i][j];

    stbi_write_bmp(fichier, img.largeur, img.hauteur, img.nbCanaux, data);

    free(data);
}

void freeImage(Image *img)
{
    for(int i = 0; i < img->largeur; i++)
        free(img->data[i]);

    free(img->data);
}

CanalImage canalRouge(Image img)
{
    CanalImage rouge = { img.largeur, img.hauteur, img.nbCanaux, malloc(img.largeur * sizeof(CanalPixel*)) };

    for(int i = 0 ; i < img.largeur ; ++i)
    {
        rouge.data[i] = malloc(img.hauteur * sizeof(CanalPixel));
        for(int j = 0 ; j < img.hauteur ; ++j)
            rouge.data[i][j] = img.data[i][j].rouge;
    }

    return rouge;
}

/* extrait de img le carré de size * size commençant en (x, y) */
CanalPixel** pixelsZone(CanalImage img, int x, int y, int size)
{
    CanalPixel **pixels = malloc(size * sizeof(CanalPixel*));

    for(int i = 0 ; i < size ; ++i)
    {
        pixels[i] = malloc(size * sizeof(CanalPixel));
        for(int j = 0 ; j < size ; ++j)
            pixels[i][j] = img.data[i + x][j + y];
    }

    return pixels;
}

/* divise la taille de la zone par 2 */
CanalPixel** redimensionneZone(CanalPixel** zone, int ancienneTaille, int nouvelleTaille)
{
    if( nouvelleTaille * 2 != ancienneTaille )
    {
        printf("ERREUR : une zone de pixels ne peut être redimensionnée que par un facteur 1/2.\n");
        return NULL;
    }

    CanalPixel **nouvelleZone = malloc(ancienneTaille * sizeof(CanalPixel*));

    for(int i = 0 ; i < nouvelleTaille ; ++i)
    {
        nouvelleZone[i] = malloc(nouvelleTaille * sizeof(CanalPixel));
        for(int j = 0 ; j < nouvelleTaille ; ++j)
        {
            nouvelleZone[i][j] = (zone[i * 2][j * 2] + zone[i * 2 + 1][j * 2]
                                + zone[i * 2][j * 2 + 1] + zone[i * 2 + 1][j * 2 + 1]) / 4;
        }
    }

    return nouvelleZone;
}

void libereTableau2D(CanalPixel **T, int largeur)
{
    for(int i = 0 ; i < largeur ; ++i)
        free(T[i]);
    free(T);
}

void iter(const Fonction* ifs, CanalImage *canal, const int sizeRB)
{
    CanalImage aux;
    aux.largeur = 2*canal->largeur;
    aux.hauteur = 2*canal->hauteur;
    aux.data = malloc(2 * largeur * sizeof(CanalPixel*));
    for(int i = 0; i<aux.largeur; i++)
        aux.data[i] = malloc(2 * hauteur * sizeof(CanalPixel));

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

CanalImage restoreCanal(const Fonction* ifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter)
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

Image restoreImage(const Fonction** tabifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter)
{
    Image img;
    img.nbCanaux = 4;
    img.hauteur = hauteur;
    img.largeur = largeur;
    img.data = malloc(largeur * sizeof(Pixel*));
    for(int i = 0; i<largeur; i++)
        img.data[i] = malloc(hauteur * sizeof(Pixel));

    CanalImage rouge = restoreCanal(ifs.[0], sizeRB, hauteur, largeur, nbIter);
    CanalImage vert = restoreCanal(ifs.[1], sizeRB, hauteur, largeur, nbIter);
    CanalImage bleu = restoreCanal(ifs.[2], sizeRB, hauteur, largeur, nbIter);
    CanalImage alpha = restoreCanal(ifs.[3], sizeRB, hauteur, largeur, nbIter);

    for(int x = 0; x<largeur, x++)
        for(int y = 0; y<hauteur; y++)
            img.data[x][y] = {rouge.data[x][y], vert.data[x][y], bleu.data[x][y], alpha.data[x][y]};

    libereTableau2D(rouge.data);
    libereTableau2D(vert.data);
    libereTableau2D(bleu.data);
    libereTableau2D(alpha.data);

    return img;
}