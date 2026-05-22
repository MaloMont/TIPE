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
    CanalImage rouge = { img.largeur, img.hauteur, malloc(img.largeur * sizeof(CanalPixel*)) };

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
