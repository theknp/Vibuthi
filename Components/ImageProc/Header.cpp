#include "stdafx.h"
#include "Header.h"
#include "math.h"

#include "OpenCV\CV.hpp"
//#include "OpenCV\CxCore.h"
//#include "OpenCV\highgui.h"


using namespace cv;
#define DEBUG_PRINT 0

#define MAX_SEARCH_RANGE_X 128
#define MAX_SEARCH_RANGE_Y 128

// Calculates log2 of number.  
double log2(double n)
{
	// log(n)/log(2) is log2.  
	return log(n) / log(2.0);
}

float roundf(float x)
{
	return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}

int ImageProcessingOps::addTwoMats()
{
	cv::Mat a(cv::Size(2, 2), CV_8U);
	cv::Mat b(cv::Size(2, 2), CV_8U);

	return 1;
}

void ImageProcessingOps::matchImages(unsigned short *origImage, int numCols, int numRows,
	unsigned short *templateImage, int numColsTemplate, int numRowsTemplate,
	double *corrImage)
{	
	int superSampleLevel = 1;
	cv::Size size;
	size.width = numCols;
	size.height = numRows;

	cv::Size size2;

	cv::Mat ipOrigImage(size, CV_16U, (void*)origImage);
	cv::Mat ipTemplateImage(size, CV_16U, (void*)templateImage);

	cv::Mat ipOrigImageF1;
	cv::Mat ipTemplateImageF1;

	cv::Mat corrSurfaceImageF2;

	try
	{
		ipOrigImage.convertTo(ipOrigImageF1, CV_32F);
		ipTemplateImage.convertTo(ipTemplateImageF1, CV_32F);

	

		matchTemplate(ipOrigImageF1, ipTemplateImageF1, corrSurfaceImageF2, CV_TM_CCOEFF_NORMED);
		printf("CHECK: Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d) :: (numCols, numRows) = (%d,%d)\n", numColsTemplate, numRowsTemplate, numCols, numRows);
		//cvResize(corrSurfaceImage, corrSurfaceImageF2, CV_INTER_LANCZOS4);

		double minVal = 0.0, maxVal = 0.0;
		cv::Point minLoc, maxLoc;
		minMaxLoc(corrSurfaceImageF2, &minVal, &maxVal, &minLoc, &maxLoc);

		//superSampleLevel = 1;
		int leftLine = (maxLoc.x - ((numColsTemplate * superSampleLevel) - 1) / 2);
		int rightLine = leftLine + (numColsTemplate * superSampleLevel) - 1;

		int topLine = (maxLoc.y - ((numRowsTemplate * superSampleLevel) - 1) / 2);
		int bottomLine = topLine + (numRowsTemplate * superSampleLevel) - 1;

		for (unsigned int idx1 = 0; idx1 < numRows - numRowsTemplate + 1; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numCols - numColsTemplate + 1; idx2++)
			{
				long vectorIdx = (idx1 * (numCols - numColsTemplate + 1)) + idx2;
				corrImage[vectorIdx] = (double)corrSurfaceImageF2.at<float>(idx1, idx2);// CV_IMAGE_ELEM(corrSurfaceImageF2, float, idx1, idx2);
			}
		}
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
		printf("matchTemplate() -> %s\n", stdException.what());
	}
}