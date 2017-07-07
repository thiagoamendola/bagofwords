

#ifndef __HOGDESCRIPTOR__
#define __HOGDESCRIPTOR__


#include "libFL/include/FL.h"
#include "common.h"

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

//Parameters
#define N_BINS 16		   //Number of bins
#define N_DIVS 3			//Number of cells = N_DIVS*N_DIVS
#define N_PHOG N_DIVS * N_DIVS * N_BINS
#define BIN_RANGE 2 * (CV_PI) / N_BINS
#define RETURN_HOG
// #define RETURN_ACC

Mat hog(const Mat &Img);
Matrix* HoGDescriptor(GVector* outputSampler, BagOfVisualWordsManager* bagOfVisualWordsManager);


#endif
