#include "pointsOfInterest.h"

//
bool isSuperpixelIntersection(Image* image, Point2D* pixelPos){
	int k, l;
	printf("################### (%d,%d)=%f\n",pixelPos->x, pixelPos->y,imageVal(image, pixelPos->x, pixelPos->y));
	Point2DList* adjacency = newAdjacency8();
	IntList* categoriesFound = createIntList(image->nx * image->ny);
	//Check each adjacency position
	for(k=0; k<adjacency->size; k++){
		//Check if adjacency position centered in the current pixel is within the matrix size
		if(isValidMatrixPosition(image, pixelPos, adjacency->points[k])){
			//Check if value was already found
			bool newValue = true;
			Point2D* adj = adjacency->points[k];
			int x = pixelPos->x+adj->x;
			int y = pixelPos->y+adj->y;
			for(l=0; l<categoriesFound->size; l++){
				int val = categoriesFound->val[l];
				if(val == imgv(image, x, y)){
					newValue = false;
				}
			}
			//If not, push current value to a list
			if(newValue){
				pushIntList(categoriesFound, imgv(image, x, y));
			}
		}
	}
	//If size of list with different values in adjacency is greater then 2, we've found an intersection
	bool isIntersection = false;
	if(categoriesFound->size > 2){
		printf("BORA \n");
		isIntersection = true;
	}

	destroyPoint2DList(&adjacency);
	destroyIntList(&categoriesFound);
	return isIntersection;
}


//
Point2DList* getSuperpixelIntersections(Image* imgSp){
	int i, j;
	Point2DList* intersections = createPoint2DList(imgSp->nx * imgSp->ny);
	//Check each pixel excluding borders
	for(i=1; i < imgSp->nx-1; i++){
		for(j=1; j < imgSp->ny-1; j++){
			//Check if this is a superpixel intersection
			Point2D *center = createPoint2D(i, j);
			if(isSuperpixelIntersection(imgSp,center)){
				printf("VRAU \n");
				//Mark this as a superpixel intersection
				pushPoint2DList(intersections,i,j);
			}
			destroyPoint2D(&center);
		}
	}

	return intersections;
}










//
//
//}
