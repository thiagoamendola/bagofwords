#include "pointsOfInterest.h"

//
bool isSuperpixelIntersection(Image* image, Point2D* pixelPos){
	int k, l;
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
		isIntersection = true;
	}

	destroyPoint2DList(&adjacency);
	destroyIntList(&categoriesFound);
	return isIntersection;
}

//
Point2DList* getSuperpixelIntersections(Image* image){
	int i, j;
	Point2DList* intersections = createPoint2DList(image->nx * image->ny);
	//Check each pixel excluding borders
	for(i=1; i < image->nx-1; i++){
		for(j=1; j < image->ny-1; j++){
			//Check if this is a superpixel intersection
			Point2D *center = createPoint2D(i, j);
			if(isSuperpixelIntersection(image,center)){
				//Mark this as a superpixel intersection
				pushPoint2DList(intersections,i,j);
			}
			destroyPoint2D(&center);
		}
	}

	return intersections;
}

// <- Kernels should be reutilized
double getGradientFromPixel(Image* image, Point2D* pixelPos){
	int k, sumx, sumy;
	double result;
	Point2DList* adjacency = newAdjacency8();
	Matrix* Gx = createMatrix(3, 3, sizeof(int));
	Matrix* Gy = createMatrix(3, 3, sizeof(int));
	//Create kernel for Gx
	mts(Gx,0,0,(int)1);
	mts(Gx,0,1,(int)0);
	mts(Gx,0,2,(int)-1);
	mts(Gx,1,0,(int)2);
	mts(Gx,1,1,(int)0);
	mts(Gx,1,2,(int)-2);
	mts(Gx,2,0,(int)1);
	mts(Gx,2,1,(int)0);
	mts(Gx,2,2,(int)-1);
	//Create kernel for Gy
	mts(Gy,0,0,(int)1);
	mts(Gy,0,1,(int)2);
	mts(Gy,0,2,(int)1);
	mts(Gy,1,0,(int)0);
	mts(Gy,1,1,(int)0);
	mts(Gy,1,2,(int)0);
	mts(Gy,2,0,(int)-1);
	mts(Gy,2,1,(int)-2);
	mts(Gy,2,2,(int)-1);
	//Initialize sums
	sumx = 0;
	sumy = 0;
	//Sum unit product with the kernels
	for(k=0; k<adjacency->size; k++){
		Point2D* adj = adjacency->points[k];
		int x = pixelPos->x+adj->x;
		int y = pixelPos->y+adj->y;
		int i = (int)k/3;
		int j = (int)k%3;
		sumx += MATRIX_GET_ELEMENT_PO_AS(int, Gx,i,j) * imgv(image, x, y);
		sumy += MATRIX_GET_ELEMENT_PO_AS(int, Gy,i,j) * imgv(image, x, y);
	}
	//Calculate result
	result = sqrt((double)(sumx*sumx + sumy*sumy));
	//Destroy structures
	destroyPoint2DList(&adjacency);
	destroyMatrix(&Gx);
	destroyMatrix(&Gy);

	return result;
}

char oldcounter[20]="";

//
GVector* getPointsOfInterest(Image* image, BagOfVisualWordsManager* bagOfVisualWordsManager){
	int k, l, auxCnt;
	Point2D *intersec;
	GVector* vec;
	Image* superpixels;
	//Load equivalent superpixel image
	char *filename = strrchr(image->filename,'/')+1;
	char file[200];
	strncpy(file, filename, strlen(filename) - 4);
	char fullpath[100];
	strncpy(fullpath, image->filename, strlen(image->filename) - strlen(filename)-1-strlen("/samples"));
	char *folder = fullpath + strlen(fullpath) - 1;
	auxCnt = 0;
	while(auxCnt<2){
		if((*folder)=='/'){
			auxCnt++;
		}
		if(auxCnt<2)
			folder = folder-1;
	}
	folder += 1;
	char sppath[200];
	snprintf(sppath, sizeof(sppath), "%s/superpixels/%s.pgm", folder,file);
	superpixels = readImagePGM(sppath);
	//Print
	char counter[10];
	strncpy(counter, strrchr(image->filename,'_')-3, 3);
	if(strcmp(counter, oldcounter)!=0){
		printf("-> class %s/100\n", counter);
		strcpy(oldcounter, counter);
	}
	//Obtain the intersections between multiple superpixels
	Point2DList* intersections = getSuperpixelIntersections(superpixels);
	double gradients[intersections->size];
	//Get gradients
	for(k=0; k<intersections->size; k++){
		intersec = intersections->points[k];
		gradients[k] = getGradientFromPixel(image, intersec);
		//printf("oooww %f <= %d/%d/%d \n", gradients[k], k, intersections->size, image->nx * image->ny);
	}
	//Get gradient positions
	Point2DList* gradPos = createPoint2DList(WORDSPERIMAGE);
	Point2D* aux;
	double limitGradient = 1000000;
	int maxGradIndex;
	for(l=0; l<WORDSPERIMAGE; l++){
		maxGradIndex = -1;
		//Check for the l-st intersection by gradient
		for(k=0; k<intersections->size; k++){
			//Check if this value is lesser then the previous greatest
			if(gradients[k] < limitGradient){
				//Check biggest gradient below the valid ones
				if(maxGradIndex == -1 || gradients[k] > gradients[maxGradIndex]){
					maxGradIndex = k;
				}
			}
		}
		limitGradient = gradients[maxGradIndex];
		aux = intersections->points[maxGradIndex];
		//printf("gradient %f <= %d/%d/%d on (%d,%d)\n", limitGradient, l, WORDSPERIMAGE, intersections->size, aux->x, aux->y);
		pushPoint2DList(gradPos, aux->x, aux->y);
	}

	//Obtain patches around points of interest
	vec = createVector(gradPos->size, sizeof(Image*));
	vec->freeFunction = destroyImageVoidPointer;
	vec->size = vec->capacity;
	for (size_t i = 0; i < vec->size; ++i) {
		Point2D* pt = gradPos->points[i];
		int halfSize = (PATCHSIZE-1)/2;
		VECTOR_GET_ELEMENT_AS(Image*, vec, i) = extractSubImage(image,pt->x-halfSize,pt->y-halfSize,2*halfSize,2*halfSize,true);
		//destroyPoint2D(&pt);
	}

	destroyImage(&superpixels);
	destroyPoint2DList(&intersections);
	destroyPoint2DList(&gradPos);

	return vec;
}





//
//
//}
