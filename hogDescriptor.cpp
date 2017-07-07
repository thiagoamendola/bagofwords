#include "hogDescriptor.h"


Matrix* HoGDescriptor(GVector* outputSampler, BagOfVisualWordsManager* bagOfVisualWordsManager){
    int k;
    Matrix* descriptor = createMatrix(outputSampler->size, N_PHOG,sizeof(float));
    //For each patch in outputSampler
    for(int i=0; i<outputSampler->size; i++){
        //Convert patch into a Mat image
        Image* img = VECTOR_GET_ELEMENT_AS(Image*, outputSampler, i);
        void *voidVec = img->channel[0];
        cv::Mat greyMat(img->nx, img->ny, CV_32FC1, voidVec);
        //Run HoG
        cv::Mat resp = hog(greyMat);
        //Convert returned Mat to Matrix* and return
        for(int j=0; j<N_PHOG; j++){
            //printf("hmm %d %f\n", j, resp.at<float>(0, j));
            MATRIX_GET_ELEMENT_PO_AS(float, descriptor, i, j) = resp.at<float>(0, j);
            //printf("ta %d\n", j);
        }
    }

    return descriptor;
}

//Input: Grayscale image
//Output: HOG features
Mat hog(const Mat &Img)
{
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32FC1;
	const float bin_range = BIN_RANGE;
	Mat Hog(1, N_PHOG, CV_32FC1, cvScalar(0.0));
	// Hog = Mat::zeros(1, N_PHOG, CV_32FC1);
	Mat accumulated_hog(1, N_BINS, CV_32FC1, cvScalar(0.0));
	// accumulated_hog = Mat::zeros(1, N_BINS, CV_32FC1);
	Mat Ix, Iy;

	float bin_midpoint[N_BINS];
	for (int i = 0; i < N_BINS; i++) {
		bin_midpoint[i] = i * BIN_RANGE + (BIN_RANGE / 2);
		// printf("%f\n", bin_midpoint[i]);
	}

	//Find orientation gradients in x and y directions
	//Sobel(Img, Ix, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	//Sobel(Img, Iy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
    Scharr(Img, Ix, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
    Scharr(Img, Iy, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);


	int cellx = Img.cols/N_DIVS;
	int celly = Img.rows/N_DIVS;

	int img_area = Img.rows * Img.cols;

	for (int m=0; m < N_DIVS; m++) {
		for (int n=0; n < N_DIVS; n++) {
			 for (int i=0; i<cellx; i++) {
				for (int j=0; j<celly; j++) {
					float px, py, grad, norm_grad, angle, angle_distance_from_midpoint, angle_in_midpoint = 1.0, angle_in_neighbor;
					bool there_is_neighbor = true;

					//px = Ix.at(m*cellx+i, n*celly+j);
					//py = Iy.at(m*cellx+i, n*celly+j);
					px = static_cast<float>(Ix.at<int16_t>((m*cellx)+i, (n*celly)+j ));
					py = static_cast<float>(Iy.at<int16_t>((m*cellx)+i, (n*celly)+j ));

					grad = static_cast<float>(std::sqrt(1.0 * px * px + py * py));
					norm_grad = grad/img_area;

					//Orientation
					angle = std::atan2(py,px);

					//convert to 0 to 360 (0 to 2*pi)
					if (angle < 0) angle+= CV_PI;

					//find appropriate bin for angle
					int nth_bin = (int)(abs(angle) / bin_range);
					int bin_neighbor;
					angle_distance_from_midpoint = angle - bin_midpoint[int(nth_bin)];
					if (angle_distance_from_midpoint <= FLT_EPSILON && angle_distance_from_midpoint >= -FLT_EPSILON) {
						angle_in_midpoint = 1.0;
						there_is_neighbor = false;
					}
					else if (angle_distance_from_midpoint < -FLT_EPSILON) {
						bin_neighbor = ((nth_bin - 1) + N_BINS) % N_BINS;
						angle_in_midpoint = abs(angle - bin_midpoint[bin_neighbor]) / bin_range;
					}
					else if (angle_distance_from_midpoint > FLT_EPSILON) {
						bin_neighbor = ((nth_bin + 1) + N_BINS) % N_BINS;
						angle_in_midpoint = abs(angle - bin_midpoint[bin_neighbor]) / bin_range;
					}

					angle_in_neighbor = 1.0 - angle_in_midpoint;
					//add magnitude of the edges in the hog matrix
					Hog.at<float>(0, (m * N_DIVS + n) * N_BINS + nth_bin) += angle_in_midpoint * norm_grad;
					if (there_is_neighbor) {
						Hog.at<float>(0, (m * N_DIVS + n) * N_BINS + bin_neighbor) += angle_in_neighbor * norm_grad;
					}
				}
			 }
		}
	}

	for (int i = 0; i < N_DIVS * N_DIVS; i++) {
		for (int j = 0; j < N_BINS; j++) {
			accumulated_hog.at<float>(0, j) = Hog.at<float>(0, i * N_BINS + j);
		}
	}
    //printf("aaa %f\n", accumulated_hog.at<float>(0, 1));
#ifdef RETURN_ACC
    // Accumulating all HoG features into one array of bins
    float max = 0.0;
    for (int i = 0; i < N_BINS; i++) {
        if (accumulated_hog.at<float>(0, i) > max) {
            max = accumulated_hog.at<float>(0, i);
        }
    }
    if (max > FLT_EPSILON) {
        // Normalization of the accumulated hog
        for (int i = 0; i < N_BINS; i++) {
            accumulated_hog.at<float>(0, i) /= max;
        }
        return accumulated_hog;
    }
#endif
#ifdef RETURN_HOG
    // Normalization of the original HoG
    for (int i=0; i< N_DIVS * N_DIVS; i++) {
        float max=0;
        int j;
        for (j=0; j<N_BINS; j++) {
            //printf("%f ", Hog.at<float>(0, i * N_BINS + j));
            if (Hog.at<float>(0, i*N_BINS + j) > max)
                max = Hog.at<float>(0, i * N_BINS + j);
        }
        if (max > FLT_EPSILON) {
            for (j=0; j<N_BINS; j++)
                Hog.at<float>(0, i * N_BINS + j)/=max;
        }
    }
    return Hog;
#endif
}
