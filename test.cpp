#include "libFL/include/FL.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(int argc, char **argv){

    float nclusters = 0.1;

    //Dictionary creation
    BagOfVisualWords* bag = (BagOfVisualWords *)malloc(sizeof(BagOfVisualWords));
    bag->directoryManager = loadDirectory("images/dict/", 1);
    bag->vocabulary = computeFeatureVectors(bag->directoryManager, 32);
    printf("rows:%d cols:%d \n",bag->vocabulary->nFeaturesVectors,bag->vocabulary->featureVector[0]->size);
    VocabularyTraining* vr = kMeansClustering(bag->vocabulary, nclusters * bag->vocabulary->nFeaturesVectors);
    printf("rows:%d cols:%d \n", vr->dictionary->nFeaturesVectors,vr->dictionary->featureVector[0]->size);

    //Training
    DirectoryManager* trainingDir = loadDirectory("images/training/", 1);
    TrainingKnowledge* tk = createTrainingKnowledge((int)trainingDir->nfiles, bag->vocabulary->nFeaturesVectors);
    //for each images
    for (int img = 0; img < (int)trainingDir->nfiles; ++img) {
        Image* currentImage = readImage(trainingDir->files[img]->path);
        tk = trainWithImage(img, currentImage, tk, vr);
        printf("%s\n",trainingDir->files[img]->path);
        for(int k=0; k < tk->nvocabulary; k++)
            printf(" %d",tk->imageHistograms[img][k]);
        printf("\n");
        //Get label for given histogram
        destroyImage(&currentImage);
    }

    //Test
    DirectoryManager* testDir = loadDirectory("images/test/", 1);
    //Do the same steps and compare the image histograms








    for(int i = 0; i < tk->nlabels; i++)
        free(tk->imageHistograms[i]);
    free(tk->imageHistograms);
    free(tk->labels);
    free(tk);
    destroyFeatureMatrix(&bag->vocabulary);
    destroyDirectoryManager(&bag->directoryManager);
    free(bag);
    destroyFeatureMatrix(&vr->dictionary);
    free(vr->labels);
    free(vr);
    destroyDirectoryManager(&trainingDir);
    destroyDirectoryManager(&testDir);




    return 0;
}
