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
    //printf("rows:%d cols:%d \n",bag->vocabulary->nFeaturesVectors,bag->vocabulary->featureVector[0]->size);
    VocabularyTraining* vr = kMeansClustering(bag->vocabulary, nclusters * bag->vocabulary->nFeaturesVectors);
    printf("rows:%d cols:%d \n", vr->dictionary->nFeaturesVectors,vr->dictionary->featureVector[0]->size);

    //Training
    DirectoryManager* trainingDir = loadDirectory("images/training/", 1);
    TrainingKnowledge* trainK = createTrainingKnowledge((int)trainingDir->nfiles, bag->vocabulary->nFeaturesVectors);
    //for each images
    for (int i = 0; i < (int)trainingDir->nfiles; ++i) {
        Image* currentImage = readImage(trainingDir->files[i]->path);
        trainK = trainWithImage(i, currentImage, trainK, vr);
        //Get label for given histogram
        char *filename = strrchr(trainingDir->files[i]->path, '/');
        trainK->labels[i] = int(filename[4]);
        //printf("%s\n",trainingDir->files[i]->path);
        //for(int k=0; k < trainK->nvocabulary; k++)
        //    printf(" %d",trainK->imageHistograms[i][k]);
        //printf("\n");
        destroyImage(&currentImage);
    }

    //Test
    DirectoryManager* testDir = loadDirectory("images/test/", 1);
    TrainingKnowledge* testK = createTrainingKnowledge((int)testDir->nfiles, bag->vocabulary->nFeaturesVectors);
    for (int i = 0; i < (int)testDir->nfiles; ++i) {
        Image* currentImage = readImage(testDir->files[i]->path);
        testK = trainWithImage(i, currentImage, testK, vr);
        destroyImage(&currentImage);
    }
    findLabels(trainK, testK);

    //Results
    int imageSum=0, imagesCorrect=0;
    for (int i = 0; i < (int)testDir->nfiles; ++i) {
        char *filename = strrchr(testDir->files[i]->path, '/');
        int correctLabel = int(filename[4]);
        if(correctLabel == testK->labels[i]){
            imagesCorrect++;
        }else{
            printf(" %s     =>   %c \n", testDir->files[i]->path, testK->labels[i]);
        }
        imageSum++;
    }
    printf("Total of test images: %d \n", imageSum);
    printf("Total of correct guesses: %d \n", imagesCorrect);
    printf("Acuracy: %f \n", (float)imagesCorrect/(float)imageSum);



    for(int i = 0; i < testK->nlabels; i++)
        free(testK->imageHistograms[i]);
    free(testK->imageHistograms);
    free(testK->labels);
    free(testK);
    for(int i = 0; i < trainK->nlabels; i++)
        free(trainK->imageHistograms[i]);
    free(trainK->imageHistograms);
    free(trainK->labels);
    free(trainK);
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
