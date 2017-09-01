#ifndef SAVER_H
#define SAVER_H

#include "constants.h"

int save(int map[MAP_WIDTH][MAP_HEIGHT], char dest[50]);
int load(int  map[MAP_WIDTH][MAP_HEIGHT], char target[50]);


#endif
