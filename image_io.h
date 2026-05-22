#ifndef IMAGE_IO_H
#define IMAGE_IO_H

typedef unsigned char CanalPixel;

typedef struct
{
    int largeur;
    int hauteur;

    CanalPixel** data; // --> data[x][y]
} CanalImage;

typedef struct
{
    CanalPixel rouge;
    CanalPixel vert;
    CanalPixel bleu;
    CanalPixel alpha;
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
CanalImage canalRouge(Image img);

CanalPixel** pixelsZone(CanalImage img, int x, int y, int size);
CanalPixel** redimensionneZone(CanalPixel** zone, int ancienneTaille, int nouvelleTaille);


#endif