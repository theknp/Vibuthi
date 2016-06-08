#include "stdafx.h"
#include "ImageProcessUtility.h"


#include "stdafx.h"
#include "math.h"
#include "opencv\cv.hpp"



using namespace cv;

void ImageProcessingUtility::threshodImage(unsigned short *origImage, int imWidth, int imHeight, int threshold,  unsigned short *thresholdedImage)
{
	thresholdedImage = new unsigned short[imHeight*imWidth];

	Size size;
	size.width = imWidth;
	size.height = imHeight;
	Mat inImage(size, CV_16U, (void*)origImage);
	Mat thImage(size, CV_16U, (void*)thresholdedImage);
	
	cv::threshold(inImage, thImage, threshold, 255, cv::THRESH_TOZERO);
	
}