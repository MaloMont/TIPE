#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_write.h"


typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} Pixel;

typedef struct
{
    int largeur;
    int hauteur;
    int nbCanaux;

    Pixel** data; // --> data[x][y]
} Image;

Image chargeFichier(const char* fichier)
{
    Image img;
	Pixel* temp = (Pixel*)stbi_load(fichier, &img.largeur, &img.hauteur, &img.nbCanaux, 0);
    img.data = malloc(img.largeur * sizeof(Pixel*));
    for(int i = 0 ; i < img.largeur ; ++i)
    {
        img.data[i] = malloc(img.hauteur * sizeof(Pixel));
        for(int j = 0 ; j < img.hauteur ; ++j)
            img.data[i][j] = temp[i * img.largeur + j];
    }
    free(temp);
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

int main()
{
    const char *fichierEntree = "images/arch_logo.png";
    Image img = chargeFichier(fichierEntree);
    for(int i = 0 ; i < img.largeur ; ++i)
    {
        for(int j = 0 ; j < img.hauteur ; ++j)
        {
            img.data[i][j].blue = 100;
            img.data[i][j].red = 150;
            img.data[i][j].green = 100;
        }
    }

    const char *fichierSortie = "images/created.bmp";
    sauveFichier(img, fichierSortie);
    freeImage(&img);
}


