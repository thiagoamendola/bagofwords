#include "libFL/include/FL.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(int argc, char **argv){

    float nclusters = 20;

    //Dictionary creation
    BagOfVisualWords* bag = (BagOfVisualWords *)malloc(sizeof(BagOfVisualWords));
    bag->directoryManager = loadDirectory("images/dict/", 1);
    bag->vocabulary = computeFeatureVectors(bag->directoryManager, 64);
    printf("rows:%d cols:%d \n",bag->vocabulary->nFeaturesVectors,bag->vocabulary->featureVector[0]->size);
    TrainingKnowledge* tr = kMeansClustering(bag->vocabulary, nclusters);
    printf("rows:%d cols:%d \n", tr->dictionary->nFeaturesVectors,tr->dictionary->featureVector[0]->size);
    //Fuse BagOfVisualWords and TrainingKnowledge?

    //Training
    DirectoryManager* trainingDir = loadDirectory("images/training/", 1);
    //for each images
    //computeFeatureVectors
    //for each descriptor found
    //cmp with vocabulary
    //sum each ocurrence to word histogram

    //Test
    DirectoryManager* testDir = loadDirectory("images/test/", 1);
    //Do the same steps and compare the image histograms







    for (int i=0; i< tr->nlabels; i++)
        printf("[%d] -> %d\n", i, tr->labels[i]);




    destroyFeatureMatrix(&bag->vocabulary);
    destroyDirectoryManager(&bag->directoryManager);
    free(bag);
    destroyFeatureMatrix(&tr->dictionary);
    free(tr->labels);
    free(tr);
    destroyDirectoryManager(&trainingDir);
    destroyDirectoryManager(&testDir);




    return 0;
}
