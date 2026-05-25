#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include "image_io.h"
#include "domain_blocks.h"

CanalImage decompresseCanal(const Fonction* ifs, const int sizeRB, const int largeur, const int hauteur, const int nbIter);
Image decompresseImage(Fonction** tabifs, const int RBsize, const int largeur, const int hauteur, const int nbCanaux, const int nbIter);

#endif