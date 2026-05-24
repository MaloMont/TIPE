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

double** valeursZone2D(CanalImage img, int x, int y, int size);
double* valeursZone1D(CanalImage img, int x, int y, int size);
CanalPixel** redimensionneZone(CanalPixel** zone, int ancienneTaille, int nouvelleTaille);
void redimensionneEnPlace2D(double** zone, int ancienneTaille, int nouvelleTaille);
void redimensionneEnPlace1D(double* zone, int L, int nouvelleTaille);


#endif