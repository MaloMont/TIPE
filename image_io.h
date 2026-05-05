#ifndef IMAGE_IO_H
#define IMAGE_IO_H

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

Image chargeFichier(const char* fichier);
void sauveFichier(Image img, const char* fichier);
void freeImage(Image *img);

#endif