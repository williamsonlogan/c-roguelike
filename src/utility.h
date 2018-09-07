#ifndef _UTILITY_H__
#define _UTILITY_H__

#include <stdlib.h>

#define VERTICAL 0
#define HORIZONTAL 1

typedef struct Coord
{
	int x;
	int y;
} Coord;

int intRand(int min, int max);

#endif // _UTILITY_H__
