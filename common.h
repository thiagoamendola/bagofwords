#ifndef __COMMON__
#define __COMMON__

#include "libFL/include/FL.h"


typedef struct _intlist{
	int size;
	int* val;
} IntList;

typedef struct _point2d{
	int x, y;
} Vector2D, Point2D;

typedef struct _point2dlist{
    int size;
    Point2D** points;
} Point2DList;




//
#define imgv(image, x, y) (int)imageVal(image, x, y)

void mts(Matrix* matrix, int i, int j, int value);

IntList* createIntList(int size);
void pushIntList(IntList *l, int value);
void destroyIntList(IntList **l);



Point2D* createPoint2D(int x, int y);
void destroyPoint2D(void* p);

Point2DList* createPoint2DList(int size);
void pushPoint2DList(Point2DList *l, int x, int y);
void destroyPoint2DList(Point2DList **pl);

Point2DList* newAdjacency4();
Point2DList* newAdjacency8();

bool isValidMatrixPosition(Image* image, Point2D* center, Point2D* mask);

void destroyImage(void* img);


#endif
