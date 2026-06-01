#ifndef IMAGE_IO_H
#define IMAGE_IO_H

typedef unsigned char CanalPixel;

#define CANAL_ROUGE  0
#define CANAL_VERT   1
#define CANAL_BLEU   2
#define CANAL_ALPHA  3

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

    CanalPixel*** data; // --> data[canal][x][y]
} Image;


Image chargeCarre(const char* fichier, int RBsize);
Image chargeFichier(const char* fichier);
void sauveFichier(Image img, const char* fichier);
void freeImage(Image *img);
void freeCanalImage(CanalImage *canal);

Image imageVide(int largeur, int hauteur, int nbCanaux);
CanalImage extraitCanal(Image img, int iCanal);
void remplaceCanal(Image img, int iCanal, CanalImage canal);

double** valeursZone2D(CanalImage img, int x, int y, int size);
double* valeursZone1D(CanalImage img, int x, int y, int size);
CanalPixel** redimensionneZone(CanalPixel** zone, int ancienneTaille, int nouvelleTaille);
CanalPixel** redimensionneZoneRectangle(CanalPixel** zone, int largeur, int hauteur);
void redimensionneEnPlace2D(double** zone, int ancienneTaille, int nouvelleTaille);
void redimensionneEnPlace1D(double* zone, int L, int nouvelleTaille);


#endif
