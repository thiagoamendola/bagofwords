#ifndef __POINTSOFINTEREST__
#define __POINTSOFINTEREST__


#include "libFL/include/FL.h"
#include "common.h"

#define WORDSPERIMAGE 64

bool isSuperpixelIntersection(Image* image, Point2D pixel);
Point2DList* getSuperpixelIntersections(Image* imgSp);
double getGradientFromPixel(Image* image, Point2D* pixelPos);
Point2DList* getPointsOfInterest(Image* image);


#endif
