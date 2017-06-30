#ifndef __POINTSOFINTEREST__
#define __POINTSOFINTEREST__

#include "libFL/include/FL.h"
#include "common.h"

bool isSuperpixelIntersection(Image* image, Point2D pixel);
Point2DList* getSuperpixelIntersections(Image* imgSp);

#endif
