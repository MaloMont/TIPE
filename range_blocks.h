#ifndef RANGE_BLOCKS_H
#define RANGE_BLOCKS_H

#include "image_io.h"

typedef struct
{
    int x, y;
} RangeBlock;

RangeBlock* trouveRBs(CanalImage img, int size, int* nbRB);

#endif