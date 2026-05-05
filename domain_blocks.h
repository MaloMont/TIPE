#ifndef DOMAIN_BLOCKS_H
#define DOMAIN_BLOCKS_H

#include "image_io.h"
#include "range_blocks.h"

typedef struct
{
    int x, y;
    int pente, y0;
} Fonction;

Fonction trouveDB(CanalImage img, RangeBlock cible, int RBsize);

#endif