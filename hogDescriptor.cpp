
#include "main.h"

//#include <iostream>
//#include <cmath>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

//using namespace std;
//using namespace cv;

//Parameters
#define N_BINS 16		   //Number of bins
#define N_DIVS 3			//Number of cells = N_DIVS*N_DIVS
#define N_PHOG N_DIVS * N_DIVS * N_BINS
#define BIN_RANGE (2 * CV_PI) / N_BINS


Matrix* HoGDescriptor(GVector* outputSampler, BagOfVisualWordsManager* bagOfVisualWordsManager){
    //Convert each patch in outputSampler into a Mat image

    //Run HoG

    //Convert returned Mat to Matrix* and return

}
/*
//Input: Grayscale image
//Output: HOG features
Mat hog(const Mat &Img)
{
	Mat Hog;
	Hog = Mat::zeros(1, N_PHOG, CV_32FC1);

	Mat Ix, Iy;

	float bin_midpoint[N_BINS];
	for (int i = 0; i < N_BINS; i++)
		bin_midpoint[i] = i * BIN_RANGE + (BIN_RANGE / 2);

	//Find orientation gradients in x and y directions
	Sobel(Img, Ix, CV_16S, 1, 0, 3);
	Sobel(Img, Iy, CV_16S, 0, 1, 3);

	int cellx = Img.cols/N_DIVS;
	int celly = Img.rows/N_DIVS;

	int img_area = Img.rows * Img.cols;

	for (int m=0; m < N_DIVS; m++) {
		for (int n=0; n < N_DIVS; n++) {
			 for (int i=0; i<cellx; i++) {
				for (int j=0; j<celly; j++) {
					float px, py, grad, norm_grad, angle, angle_distance_from_midpoint, angle_in_midpoint = 1.0, angle_in_neighbor;
					int there_is_neighbor = 1;

					//px = Ix.at(m*cellx+i, n*celly+j);
					//py = Iy.at(m*cellx+i, n*celly+j);
					px = static_cast<float>(Ix.at<int16_t>((m*cellx)+i, (n*celly)+j ));
					py = static_cast<float>(Iy.at<int16_t>((m*cellx)+i, (n*celly)+j ));

					grad = static_cast<float>(std::sqrt(1.0 * px * px + py * py));
					norm_grad = grad/img_area;

					//Orientation
					angle = std::atan2(py,px);

					//convert to 0 to 360 (0 to 2*pi)
					if (angle < 0)
						angle+= 2*CV_PI;

					//find appropriate bin for angle
					int nth_bin = round(angle / BIN_RANGE);
					int bin_neighbor;
					angle_distance_from_midpoint = angle - bin_midpoint[int(nth_bin)];
					if (angle_distance_from_midpoint <= FLT_EPSILON && angle_distance_from_midpoint >= -FLT_EPSILON) {
						angle_in_midpoint = 1.0;
						there_is_neighbor = 0;
					}
					else if (angle_distance_from_midpoint < -FLT_EPSILON) {
						bin_neighbor = (nth_bin - 1) % N_BINS;
						angle_in_midpoint = abs(angle - bin_midpoint[bin_neighbor]) / BIN_RANGE;
					}
					else if (angle_distance_from_midpoint > FLT_EPSILON) {
						bin_neighbor = (nth_bin + 1) % N_BINS;
						angle_in_midpoint = abs(angle - bin_midpoint[bin_neighbor]) / BIN_RANGE;
					}

					angle_in_neighbor = 1.0 - angle_in_midpoint;
					//add magnitude of the edges in the hog matrix
					Hog.at<float>(0, (m * N_DIVS + n) * N_BINS + nth_bin) += angle_in_midpoint * norm_grad;
					if (there_is_neighbor == 1)
						Hog.at<float>(0, (m * N_DIVS + n) * N_BINS + bin_neighbor) += angle_in_neighbor * norm_grad;
				}
			 }
		}
	}

	Mat accumulated_hog = Mat::zeros(1, N_BINS, CV_32FC1);
	for (int i = 0; i < N_DIVS * N_DIVS; i++) {
		for (int j = 0; j < N_BINS; j++) {
			accumulated_hog.at<float>(0, j) = Hog.at<float>(0, i * N_BINS + j);
		}
	}

	// Accumulating all HoG features into one array of bins
	float max = 0.0;
	for (int i = 0; i < N_BINS; i++) {
		if (accumulated_hog.at<float>(0, i) > max) {
			max = accumulated_hog.at<float>(0, i);
		}
	}

	//Normalization
	for (int i = 0; i < N_BINS; i++) {
		accumulated_hog.at<float>(0, i) /= max;
	}

	//Normalization
	// for (int i=0; i< N_DIVS * N_DIVS; i++) {
	//	float max=0;
	//	int j;
	//	for (j=0; j<N_BINS; j++) {
	//		if (Hog.at<float>(0, i*N_BINS + j) > max)
	//			max = Hog.at<float>(0, i * N_BINS + j);
	//	}
	//	for (j=0; j<N_BINS; j++)
	//		Hog.at<float>(0, i * N_BINS + j)/=max;
	// }
	return accumulated_hog;
}
*/
