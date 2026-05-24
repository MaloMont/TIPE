#include <assert.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_write.h"

#include "image_io.h"
#include "domain_blocks.h"

Image imageVide(int largeur, int hauteur, int nbCanaux)
{
    CanalPixel ***data = malloc(nbCanaux * sizeof(CanalPixel**));
    for(int c = 0 ; c < nbCanaux ; ++c)
    {
        data[c] = malloc(largeur * sizeof(CanalPixel*));
        for(int i = 0 ; i < largeur ; ++i)
            data[c][i] = calloc(hauteur, sizeof(CanalPixel));
    }

    Image img = (Image){
        largeur,
        hauteur,
        nbCanaux,
        data
    };

    return img;
}

//même chose que chargeFichier mais de façon à que l'image soit divisible par les range blocks
//ducoup le reste c'est du noir mais bon
Image chargeCarre(const char* fichier, int RBsize)
{
    int largeur, hauteur, nbCanaux;
    Pixel* tmp = (Pixel*)stbi_load(fichier, &largeur, &hauteur, &nbCanaux, 0);

    printf("l : %d, h : %d\n", largeur, hauteur);

    int largeurArrondie = largeur + RBsize - (largeur % RBsize);
    int hauteurArrondie = hauteur + RBsize - (hauteur % RBsize);
    
    Image img = imageVide(largeurArrondie, hauteurArrondie, nbCanaux);

    for(int i = 0 ; i < largeur ; ++i)
    {
        for(int j = 0 ; j < hauteur ; ++j)
        {
            img.data[CANAL_ROUGE][i][j] = tmp[i + largeur * j].rouge;
            img.data[CANAL_VERT][i][j]  = tmp[i + largeur * j].vert;
            img.data[CANAL_BLEU][i][j]  = tmp[i + largeur * j].bleu;
            img.data[CANAL_ALPHA][i][j] = tmp[i + largeur * j].alpha;
        }
    }

    free(tmp);
    return img;
}

Image chargeFichier(const char* fichier)
{
    int largeur, hauteur, nbCanaux;
    Pixel* tmp = (Pixel*)stbi_load(fichier, &largeur, &hauteur, &nbCanaux, 0);
    Image img = imageVide(largeur, hauteur, nbCanaux);

    for(int i = 0 ; i < img.largeur ; ++i)
    {
        for(int j = 0 ; j < img.hauteur ; ++j)
        {
            img.data[CANAL_ROUGE][i][j] = tmp[i + img.largeur * j].rouge;
            img.data[CANAL_VERT][i][j]  = tmp[i + img.largeur * j].vert;
            img.data[CANAL_BLEU][i][j]  = tmp[i + img.largeur * j].bleu;
            img.data[CANAL_ALPHA][i][j] = tmp[i + img.largeur * j].alpha;
        }
    }

    free(tmp);
    return img;
}

void sauveFichier(Image img, const char* fichier)
{
    if(img.nbCanaux != 4)
    {
        printf("l'image n'est pas au format rgb (le nombre de canaux ne colle pas).\n");
        return;
    }

    Pixel* data = malloc(img.largeur * img.hauteur * sizeof(Pixel));

    for(int i = 0; i < img.largeur; i++)
        for(int j = 0; j < img.hauteur; j++)
        {
            data[i * img.largeur + j].rouge = img.data[CANAL_ROUGE][i][j];
            data[i * img.largeur + j].vert  = img.data[CANAL_VERT][i][j];
            data[i * img.largeur + j].bleu  = img.data[CANAL_BLEU][i][j];
            data[i * img.largeur + j].alpha = img.data[CANAL_ALPHA][i][j];
        }

    stbi_write_bmp(fichier, img.largeur, img.hauteur, img.nbCanaux, data);

    free(data);
}

void freeImage(Image *img)
{
    for(int c = 0 ; c < img->nbCanaux ; ++c)
    {
        for(int i = 0; i < img->largeur; i++)
            free(img->data[c][i]);
        free(img->data[c]);
    }

    free(img->data);
}

void freeCanalImage(CanalImage *canal)
{
    for(int i = 0; i < canal->largeur; i++)
        free(canal->data[i]);

    free(canal->data);
}

void remplaceCanal(Image img, int iCanal, CanalImage canal)
{
    for(int i = 0 ; i < img.largeur ; ++i)
    {
        for(int j = 0 ; j < img.hauteur ; ++j)
        {
            img.data[iCanal][i][j] = canal.data[i][j];
        }
    }
}

CanalImage extraitCanal(Image img, int iCanal)
{
    CanalPixel **data = malloc(img.largeur * sizeof(CanalPixel*));
    
    for(int i = 0 ; i < img.largeur ; ++i)
    {
        data[i] = malloc(img.hauteur * sizeof(CanalPixel));
        for(int j = 0 ; j < img.hauteur ; ++j)
            data[i][j] = img.data[iCanal][i][j];
    }
    
    CanalImage canal = { img.largeur, img.hauteur, data};
    return canal;
}

/* extrait de img le carré de size * size commençant en (x, y)
 * le place directement dans un tableau de double
 */
double** valeursZone2D(CanalImage img, int x, int y, int size)
{
    double **pixels = malloc(size * sizeof(double*));

    for(int i = 0 ; i < size ; ++i)
    {
        pixels[i] = malloc(size * sizeof(double));
        for(int j = 0 ; j < size ; ++j)
            pixels[i][j] = img.data[i + x][j + y];
    }

    return pixels;
}


/* extrait de img le carré de size * size commençant en (x, y)
 * le place directement dans un tableau de double
 */
double* valeursZone1D(CanalImage img, int x, int y, int size)
{
    double *pixels = malloc(size * size * sizeof(double*));

    for(int i = 0 ; i < size ; ++i)
        for(int j = 0 ; j < size ; ++j)
            pixels[i + size*j] = img.data[i + x][j + y];

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


/* divise la taille de la zone par 2
 * le résultat se retrouve dans le carré L/2 L/2 en haut à gauche du carré de départ
 * le reste est laissé tel quel
 */
void redimensionneEnPlace2D(double** zone, int ancienneTaille, int nouvelleTaille)
{
    if( nouvelleTaille * 2 != ancienneTaille )
    {
        printf("ERREUR : une zone de pixels ne peut être redimensionnée que par un facteur 1/2.\n");
        return;
    }

    for(int i = 0 ; i < nouvelleTaille ; ++i)
    {
        for(int j = 0 ; j < nouvelleTaille ; ++j)
        {
            zone[i][j] = (zone[i * 2][j * 2] + zone[i * 2 + 1][j * 2]
                        + zone[i * 2][j * 2 + 1] + zone[i * 2 + 1][j * 2 + 1])
                        / 4;
        }
    }
}

/* divise la taille de la zone par 2
 * le résultat se retrouve dans le carré L/2 L/2 en haut à gauche du carré de départ
 * le reste est laissé tel quel
 */
void redimensionneEnPlace1D(double* zone, int L, int nouvelleTaille)
{
    if( nouvelleTaille * 2 != L )
    {
        printf("ERREUR : une zone de pixels ne peut être redimensionnée que par un facteur 1/2.\n");
        return;
    }

    for(int i = 0 ; i < nouvelleTaille ; ++i)
    {
        for(int j = 0 ; j < nouvelleTaille ; ++j)
        {
            zone[i + j*L] = (zone[i * 2 + j * 2 * L] + zone[i * 2 + 1 + j * 2 * L]
                        + zone[i * 2 + (j * 2 + 1)*L] + zone[i * 2 + 1 + (j * 2 + 1)*L])
                        / 4;
        }
    }
}
