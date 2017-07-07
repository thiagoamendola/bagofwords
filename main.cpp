#include "main.h"


int main(int argc, char **argv) {
    /*

    //Image* image = readImage("obj8__325.pgm");
    Image* image = readImage("images/coil-100/samples/000001_00000793.png");
    printf("AE ow\n");
    GVector* intersections = getPointsOfInterest(image, NULL);

    printf("eta\n");
    printf("RESULTS %d/%d\n", intersections->size, image->nx * image->ny);

    destroyVector(&intersections);
    destroyImage(&image);
    return 0;
    */
///*
    size_t numberOfVisualWords = 500;

    char const* const fileName_createDict = "images/coil-100/train_paths.txt";
    char const* const fileName_createTrain = "images/coil-100/train_paths.txt";
    char const* const fileName_createTest = "images/coil-100/test_paths.txt";

    GVector* vectorSamplesUsed2CreateDict =  splitsLinesInTextFile(fileName_createDict);
    GVector* vectorSamplesUsed2TrainClassifier =  splitsLinesInTextFile(fileName_createTrain);
    GVector* vectorSamplesUsed2TestClassifier =  splitsLinesInTextFile(fileName_createTest);

    BagOfVisualWordsManager* bowManager = createBagOfVisualWordsManager();

    bowManager->pathsToImages_dictionery = vectorSamplesUsed2CreateDict;
    bowManager->pathsToImages_train = vectorSamplesUsed2TrainClassifier;
    bowManager->pathsToImages_test = vectorSamplesUsed2TestClassifier;

    //bowManager->imageSamplerFunction = gridSamplingBow;
    //ArgumentList* gridSamplingArguments = createArgumentList();
    //ARGLIST_PUSH_BACK_AS(size_t,gridSamplingArguments,64);
    //ARGLIST_PUSH_BACK_AS(size_t,gridSamplingArguments,64);
    //bowManager->argumentListOfSampler = gridSamplingArguments;
    bowManager->imageSamplerFunction = getPointsOfInterest;

    bowManager->freeFunction2SamplerOutput = destroyImageVoidPointer;

    //bowManager->featureExtractorFunction = computeColorHistogramBow;
    //ArgumentList* colorFeatureExtractorArguments = createArgumentList();
    //size_t nbins = 7;
    //ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,nbins);
    //ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,nbins*nbins*nbins);
    //bowManager->argumentListOfFeatureExtractor = colorFeatureExtractorArguments;
    bowManager->featureExtractorFunction = HoGDescriptor;

    bowManager->distanceFunction = computeNormalizedL2Norm;
    bowManager->argumentListOfDistanceFunction = NULL;

    bowManager->clusteringFunction = kmeansClusteringBow;
    ArgumentList* clusteringMethodArguments = createArgumentList();
    ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,numberOfVisualWords); //number of words
    ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,100); //maximum number of iterations
    ARGLIST_PUSH_BACK_AS(double,clusteringMethodArguments,0.0001); //tolerance
    ARGLIST_PUSH_BACK_AS(int,clusteringMethodArguments,0); //seed
    ARGLIST_PUSH_BACK_AS(DistanceFunction,clusteringMethodArguments,computeNormalizedL2Norm); //seed
    ARGLIST_PUSH_BACK_AS(ArgumentList*,clusteringMethodArguments,NULL); //seed
    bowManager->argumentListOfClustering = clusteringMethodArguments;

    ///////////
    computeDictionery(bowManager);


    bowManager->mountHistogramFunction = computeCountHistogram_bow;
    bowManager->argumentListOfHistogramMounter = NULL;


    //SVM Classifier
    SVM_Classifier* classifiersvm = createSVMClassifier();
    classifiersvm->param.kernel_type = RBF;
    classifiersvm->param.gamma = 3.5;
    bowManager->classifier = (void*)classifiersvm;
    bowManager->fitFunction = svm_Classifier_fit;
    bowManager->storeTrainData = false;
    bowManager->predictFunction = svm_Classifier_predict;
    bowManager->storePredictedData = false;
    bowManager->freeFunctionClassifier = destroySVMClassifierForVoidPointer;




    //ArgumentList* colorFeatureExtractorArguments = createArgumentList();
    //size_t nbins = 7;
    //ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,nbins); //nBins per channel
    //ARGLIST_PUSH_BACK_AS(size_t,colorFeatureExtractorArguments,nbins*nbins*nbins); //total number of channels
    //bowManager->argumentListOfFeatureExtractor = colorFeatureExtractorArguments; //passando a lista de argumentos do feature extractor para o bow manager
    //bowManager->distanceFunction = computeNormalizedL1Norm;
    //bowManager->argumentListOfDistanceFunction = NULL;
    //bowManager->clusteringFunction = kmeansClusteringBow;
    //ArgumentList* clusteringMethodArguments = createArgumentList();
    //ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,numberOfVisualWords); //number of words
    //ARGLIST_PUSH_BACK_AS(size_t,clusteringMethodArguments,100); //maximum number of iterations
    //ARGLIST_PUSH_BACK_AS(double,clusteringMethodArguments,0.0001); //tolerance
    //ARGLIST_PUSH_BACK_AS(int,clusteringMethodArguments,0); //seed
    //ARGLIST_PUSH_BACK_AS(DistanceFunction,clusteringMethodArguments,computeNormalizedL1Norm); //seed
    //ARGLIST_PUSH_BACK_AS(ArgumentList*,clusteringMethodArguments,NULL); //seed
    //bowManager->argumentListOfClustering = clusteringMethodArguments;
    /////////////////////////////////////////////////////////////////


    trainClassifier(bowManager);
    GVector* labelsPredicted = predictLabels(bowManager);

    //////////////////////////
    //Le os true labels das imagens e checa com os labels predizidos pelo o classificador.
    //computa uma simples acuracia (numero de amostras rotuladas corretamente / numero de amostras do conjunto)
    GVector* trueLabels = createNullVector(bowManager->pathsToImages_test->size,sizeof(int));
    int hit = 0;
    printf("file | predicted true\t\tcorrect\n");
    char symbol;
    for (size_t index = 0; index < bowManager->pathsToImages_test->size; ++index) {
        symbol = 'X';
        char * path = VECTOR_GET_ELEMENT_AS(char*,bowManager->pathsToImages_test,index);
        VECTOR_GET_ELEMENT_AS(int,trueLabels,index) = findTrueLabelInName(path);
        if(VECTOR_GET_ELEMENT_AS(int,trueLabels,index) == VECTOR_GET_ELEMENT_AS(int,labelsPredicted,index)){
            hit++;
            symbol = 'O';
        }
        printf("%s | %d %d\t\t%c\n",
               path,
               VECTOR_GET_ELEMENT_AS(int,labelsPredicted,index),
               VECTOR_GET_ELEMENT_AS(int,trueLabels,index),symbol
        );
    }
    double acuracia = ((double)hit)/bowManager->pathsToImages_test->size;
    printf("acuracia: %f\n",acuracia);

    destroyVector(&trueLabels);
    destroyVector(&labelsPredicted);
    destroyBagOfVisualWordsManager(&bowManager);
    return 0;
    //*/
}
