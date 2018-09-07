#include "utility.h"

int intRand(int min, int max)
{
	return 	(rand() % (max + 1 - min) + min);
}
