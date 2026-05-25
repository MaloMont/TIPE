#ifndef ENCODE_H
#define ENCODE_H

typedef struct
{
    int x, y;
    double pente, y0;
} Fonction;

typedef struct
{
    int largeur, hauteur;
    int nbFonctions;
    int nbCanaux;
    int RBsize;
    Fonction **fs;
} IFS;

void encodeIFS(char *nomFichier, const IFS ifs);
IFS decodeIFS(char *nomFichier);
void freeIFS(IFS ifs);

#endif