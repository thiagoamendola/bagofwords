#include "libFL/include/FL.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(int argc, char **argv){

    float kwords = 0.1;

    DirectoryManager* dir = loadDirectory("images/", 1);
    FeatureMatrix* fm = computeFeatureVectors(dir, 64);
    printf("rows:%d cols:%d \n",fm->nFeaturesVectors,fm->featureVector[0]->size);
    int nclusters = floor(fm->nFeaturesVectors*kwords);
    FeatureMatrix* clusters = kMeansClustering(fm, nclusters);
    printf("rows:%d cols:%d \n",clusters->nFeaturesVectors,clusters->featureVector[0]->size);




    destroyFeatureMatrix(&fm);
    destroyFeatureMatrix(&clusters);
    destroyDirectoryManager(&dir);




    return 0;
}
