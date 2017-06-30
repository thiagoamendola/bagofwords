#include "common.h"





//
IntList* createIntList(int size){
    IntList* l = NULL;
	l = (IntList*)calloc(1,sizeof(IntList));
	l->size = 0;
	l->val = (int**)calloc(size,sizeof(int*));
    return l;
}

//
void pushIntList(IntList *l, int value){
    int* v = NULL;
	v = (int*)calloc(1,sizeof(int));
	l->val[l->size] = v;
	l->size += 1;
    return;
}

//
void destroyIntList(IntList **l){
    if ( (*l) == NULL){
        return;
    }
	for(int i=0; i<(*l)->size; i++){
		free((*l)->val[i]);
	}
	free((*l)->val);
    free(*l);
    *l= NULL;
}


//
Point2D* createPoint2D(int x, int y){
    Point2D* p = NULL;
	p = (Point2D*)calloc(1,sizeof(Point2D));
	p->x = x;
	p->y = y;
    return p;
}

//
void destroyPoint2D(Point2D **p){
    if ( (*p) == NULL){
        return;
    }
    free(*p);
    *p= NULL;
}


//
Point2DList* createPoint2DList(int size){
    Point2DList* pl = NULL;
	pl = (Point2DList*)calloc(1,sizeof(Point2DList));
	pl->size = 0;
	pl->points = (Point2D**)calloc(size,sizeof(Point2D*));
    return pl;
}

//
void pushPoint2DList(Point2DList *l, int x, int y){
	Point2D* p = createPoint2D(x,y);
	l->points[l->size] = p;
    l->size += 1;
	return;
}

//
void destroyPoint2DList(Point2DList **pl){
    if ( (*pl) == NULL){
        return;
    }
	for(int i=0; i<(*pl)->size; i++){
		destroyPoint2D(&((*pl)->points[i]));
	}
	free((*pl)->points);
    free(*pl);
    *pl= NULL;
}

//
Point2DList* newAdjacency4(){
	Point2DList* pl = createPoint2DList(5);
	pl->points[0] = createPoint2D(0,0);
	pl->points[1] = createPoint2D(0,1);
	pl->points[2] = createPoint2D(1,0);
	pl->points[3] = createPoint2D(0,-1);
	pl->points[4] = createPoint2D(-1,0);
	return pl;
}

//
Point2DList* newAdjacency8(){
	Point2DList* pl = createPoint2DList(8);
	pl->points[0] = createPoint2D(0,0);
	pl->points[1] = createPoint2D(0,1);
	pl->points[2] = createPoint2D(1,0);
	pl->points[3] = createPoint2D(0,-1);
	pl->points[4] = createPoint2D(-1,0);
	pl->points[5] = createPoint2D(1,1);
	pl->points[6] = createPoint2D(1,-1);
	pl->points[7] = createPoint2D(-1,1);
	pl->points[8] = createPoint2D(-1,-1);
	return pl;
}

//
bool isValidMatrixPosition(Image* image, Point2D* center, Point2D* mask){
	//Check for x axis
	int xval = center->x + mask->x;
	if(xval < 0 || xval >= image->nx){
		return false;
	}
	//Check for y axis
	int yval = center->y + mask->y;
	if(yval < 0 || yval >= image->ny){
		return false;
	}
	return true;
}
