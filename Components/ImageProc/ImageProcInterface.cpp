// ImageProcInterface.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "math.h"
#include "ImageProcInterface.h"

#include "OpenCV\CV.h"
#include "OpenCV\CxCore.h"
#include "OpenCV\highgui.h"

#include "opencv2\opencv.hpp"

using namespace cv;
#define DEBUG_PRINT 0

#define MAX_SEARCH_RANGE_X 128
#define MAX_SEARCH_RANGE_Y 128

// Calculates log2 of number.  
double log2( double n )  
{  
	// log(n)/log(2) is log2.  
	return log( n ) / log( 2.0 );  
}

float roundf(float x)
{
	return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}


void matchImages(	unsigned short *origImage, int numCols, int numRows, 
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


	//IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	//IplImage *ipOrigImageF1 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	//IplImage *ipOrigImageF2 = cvCreateImage(cvSize(numCols * superSampleLevel, numRows * superSampleLevel), IPL_DEPTH_32F, 1);
	
	//IplImage *dummyF = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);

	//IplImage *ipTemplateImage = cvCreateImageHeader(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_16S, 1);
	//IplImage *ipTemplateImageF1 = cvCreateImage(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_32F, 1);
	//IplImage *ipTemplateImageF2 = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);

	//IplImage *corrSurfaceImage = cvCreateImage(cvSize(numCols - numColsTemplate + 1, numRows - numRowsTemplate + 1), IPL_DEPTH_32F, 1);
	//IplImage *corrSurfaceImageF2 = cvCreateImage(cvSize((numCols * superSampleLevel) - (numColsTemplate * superSampleLevel) + 1, (numRows * superSampleLevel) - (numRowsTemplate * superSampleLevel) + 1), IPL_DEPTH_32F, 1);

	try
	{		
		ipOrigImage.convertTo(ipOrigImageF1, CV_32F);
		ipTemplateImage.convertTo(ipTemplateImageF1, CV_32F);

		//cvSetImageData(ipOrigImage, origImage, numCols * sizeof(short));
		//cvConvert(ipOrigImage, ipOrigImageF1);
		//cvResize(ipOrigImageF1, ipOrigImageF2, CV_INTER_LANCZOS4); // UPSAMPLE

		//cvSetImageData(ipTemplateImage, templateImage, numColsTemplate * sizeof(short));
		//cvConvert(ipTemplateImage, ipTemplateImageF1);
		//cvResize(ipTemplateImageF1, ipTemplateImageF2, CV_INTER_LANCZOS4); // UPSAMPLE

				
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

		//for (unsigned int idx1 = topLine; idx1 <= bottomLine; idx1++)
		//{
		//	for (unsigned int idx2 = leftLine; idx2 <= rightLine; idx2++)
		//	{
		//		//long vectorIdx = (idx1 * (numColsTemplate * superSampleLevel)) + idx2;
		//		CV_IMAGE_ELEM(dummyF, float, idx1 - topLine, idx2 - leftLine) = CV_IMAGE_ELEM(ipOrigImageF2, float, idx1, idx2);
		//	}
		//}
		//cvResize(dummyF, ipTemplateImageF1, CV_INTER_LANCZOS4);
		
		//cvResize(corrSurfaceImageF2, corrSurfaceImage, CV_INTER_AREA);

		for (unsigned int idx1 = 0; idx1 < numRows - numRowsTemplate + 1; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numCols - numColsTemplate + 1; idx2++)
			{
				long vectorIdx = (idx1 * (numCols - numColsTemplate + 1)) + idx2;
				corrImage[vectorIdx] = (double)corrSurfaceImageF2.at<float>(idx1,idx2);// CV_IMAGE_ELEM(corrSurfaceImageF2, float, idx1, idx2);
			}
		}
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
		printf("matchTemplate() -> %s\n", stdException.what());
	}

	/*cvReleaseImageHeader(&ipOrigImage);
	cvReleaseImageHeader(&ipTemplateImage);

	cvReleaseImage(&corrSurfaceImage);
	cvReleaseImage(&corrSurfaceImageF2);
	cvReleaseImage(&ipOrigImageF1);
	cvReleaseImage(&ipOrigImageF2);
	cvReleaseImage(&dummyF);
	cvReleaseImage(&ipTemplateImageF1);
	cvReleaseImage(&ipTemplateImageF2);*/
}

/*
void rotateImageData(short *inputImageData, int numCols, int numRows,
					 double angle, 
					 short *outputImageData)
{
	IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	
	IplImage *ipOrigImageF1 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	IplImage *ipOrigImageF2 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);

	cvSetImageData(ipOrigImage, inputImageData, numCols * sizeof(short));
	
	if (1)
	{
		cvConvert(ipOrigImage, ipOrigImageF1);

		CvMat *rotMatrix = cvCreateMat(2, 3, CV_32F);
		Point center = Point(numCols / 2, numRows / 2);
		double scale = 1.0;
		try
		{
			cv2DRotationMatrix(center, angle, scale, rotMatrix);
			cvWarpAffine(ipOrigImageF1, ipOrigImageF2, rotMatrix, CV_INTER_LANCZOS4 + CV_WARP_FILL_OUTLIERS);
		}
		catch (std::exception &stdException)
		{
			//printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
			printf("rotateImageData() -> %s\n", stdException.what());
		}

		//Usage: cv2DRotationMatrix(CvPoint2D32f center, double angle, double scale, CvMat* map_matrix)
		//rot_mat = getRotationMatrix2D(center, angle, scale);
		//warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
	}
	
	for (unsigned int idx1 = 0; idx1 < numRows; idx1++)
	{
		for (unsigned int idx2 = 0; idx2 < numCols; idx2++)
		{
			long vectorIdx = (idx1 * numCols) + idx2;
			//outputImageData[vectorIdx] = CV_IMAGE_ELEM(ipOrigImageF2, float, idx1, idx2);
			outputImageData[vectorIdx] = CV_IMAGE_ELEM(ipOrigImage, short, idx1, idx2);
		}
	}

	cvReleaseImageHeader(&ipOrigImage);

	cvReleaseImage(&ipOrigImageF1);
	cvReleaseImage(&ipOrigImageF2);
}

void computeStdDevFromImageData(short* imageDataIn, 
								int numCols, 
								int numRows, 
								double* stdDev)
{
	IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));
	*stdDev = 0.0;

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar cvMean;
	CvScalar cvStdDev;
	cvAvgSdv(ipImage, &cvMean, &cvStdDev, NULL);
	*stdDev = cvStdDev.val[0];
	// ---------------------------------------------------------------
	// Step 1: Release resources
	// ---------------------------------------------------------------
	cvReleaseImageHeader(&ipImage);
	return;
}

void computeStdDevFromImageDataWithBorder(short* imageDataIn, int numCols, int numRows, short borderWidth, double* stdDev)
{
	short* choppedImage = (short *)malloc( (numCols - (2 * borderWidth)) *  (numRows - (2 * borderWidth)) );
	long targetIdx = 0;
	
	for (int i = borderWidth; i < (numRows - borderWidth); i++)
	{
		for (int j = borderWidth; j < (numCols - borderWidth); j++)
		{
			long vectorIdx = (i * numCols) + j;
			choppedImage[targetIdx] = imageDataIn[vectorIdx];
			targetIdx++;
		}
	}
	
	IplImage *ipImage = cvCreateImageHeader( cvSize( (numCols - (2 * borderWidth)), (numRows - (2 * borderWidth)) ), IPL_DEPTH_16S, 1 );
	cvSetImageData(ipImage, choppedImage, (numCols - (2 * borderWidth)) * sizeof(short));
	*stdDev = 0.0;

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar cvMean;
	CvScalar cvStdDev;
	cvAvgSdv(ipImage, &cvMean, &cvStdDev, NULL);
	*stdDev = cvStdDev.val[0];
	// ---------------------------------------------------------------
	// Step 1: Release resources
	// ---------------------------------------------------------------
	cvReleaseImageHeader(&ipImage);
	return;
}


void computeMeanFromImageData(short* imageDataIn, int numCols, int numRows, double* mean)
{
	IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));
	*mean = 0.0;

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar cvMean;
	CvScalar cvStdDev;
	cvAvgSdv(ipImage, &cvMean, &cvStdDev, NULL);
	*mean = cvMean.val[0];
	// ---------------------------------------------------------------
	// Step 1: Release resources
	// ---------------------------------------------------------------
	cvReleaseImageHeader(&ipImage);
	return;
}

void histoEqualizeImageData(short* inImageData, int numCols, int numRows, short* outImageData)
{
	//////IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16U, 1);
	//////cvSetImageData(ipImage, inImageData, numCols * sizeof(short));
	//////IplImage *ipImage2 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_8U, 1);

	//////IplImage *outImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_16U, 1);
	//////IplImage *outImage2 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_8U, 1);

	//////try
	//////{
	//////	//cvConvert(ipImage,  ipImage2);
	//////	cvCvtColor(ipImage, ipImage2, COLOR_BGR2GRAY);
	//////	//cvConvert(outImage, outImage2);
	//////

	//////// ---------------------------------------------------------------
	//////// Step 0: Normalize Histo
	//////// ---------------------------------------------------------------

	//////	cvEqualizeHist(ipImage2, outImage2);
	//////	cvCvtColor(outImage2, outImage, COLOR_GRAY2BGR);
	//////	//cvConvert(ipImage2, outImage);

	//////	for (unsigned int idx1 = 0; idx1 < numRows; idx1++)
	//////	{
	//////		for (unsigned int idx2 = 0; idx2 < numCols; idx2++)
	//////		{
	//////			long vectorIdx = (idx1 * numCols) + idx2;
	//////			outImageData[vectorIdx] = CV_IMAGE_ELEM(outImage, short, idx1, idx2);
	//////		}
	//////	}
	//////}
	//////catch (std::exception &stdException)
	//////{
	//////	printf("Trying to perform histoEqualizeImageData with numCols, numRows  = (%d, %d)\n", numCols, numRows);
	//////	printf("histoEqualizeImageData() -> %s\n", stdException.what());
	//////}

	//////// ---------------------------------------------------------------
	//////// Step 1: Release resources
	//////// ---------------------------------------------------------------
	//////cvReleaseImageHeader(&ipImage);
	//////cvReleaseImage(&outImage);
	return;
}

void subtractVectors(short* imageData1,
					short* imageData2,
					int numCols,
					int numRows,
					short* imageDataOut)
{
	IplImage *ipImage1 = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage1, imageData1, numCols * sizeof(short));

	IplImage *ipImage2 = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage2, imageData2, numCols * sizeof(short));

	IplImage *outImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);

	cvSub(ipImage1, ipImage2, outImage);

	for (unsigned int idx1 = 0; idx1 < numRows; idx1++)
	{
		for (unsigned int idx2 = 0; idx2 < numCols; idx2++)
		{
			long vectorIdx = (idx1 * numCols) + idx2;
			imageDataOut[vectorIdx] = CV_IMAGE_ELEM(outImage, short, idx1, idx2);
		}
	}

	// ---------------------------------------------------------------
	// Step 1: Release resources
	// ---------------------------------------------------------------
	cvReleaseImageHeader(&ipImage1);
	cvReleaseImageHeader(&ipImage2);
	cvReleaseImage(&outImage);
	return;
}

// cvScaleAdd(const CvArr* src1, CvScalar scale, const CvArr* src2, CvArr* dst)¶
// out = (input - constToSubtr)* scale + constToAdd
void scaleAndAddImageData(	short* imageDataIn,
							int numCols,
							int numRows,
							double scale,
							short constToSubtr,
							short constToAdd,
							short* imageDataOut)
{
	IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));

	IplImage *ipImageF = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	cvConvert(ipImage, ipImageF);

	IplImage *addImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar subValue = cvScalar(constToSubtr);
	CvScalar scaleValue = cvScalar(scale);
	CvScalar addValue = cvScalar(constToAdd);

	cvSet(addImage, addValue);
	cvSubS(ipImageF, subValue, ipImageF);
	cvScaleAdd(ipImageF, scaleValue, addImage, addImage);

	for (unsigned int idx1 = 0; idx1 < numRows; idx1++)
	{
		for (unsigned int idx2 = 0; idx2 < numCols; idx2++)
		{
			long vectorIdx = (idx1 * numCols) + idx2;
			imageDataOut[vectorIdx] = cvRound(CV_IMAGE_ELEM(addImage, float, idx1, idx2));
		}
	}

	// ---------------------------------------------------------------
	// Step 1: Release resources
	// ---------------------------------------------------------------
	cvReleaseImageHeader(&ipImage);
	cvReleaseImageHeader(&ipImageF);
	cvReleaseImage(&addImage);
	return;
}

void matchTemplateV2(	short *origImage, int numCols, int numRows,
						short *templateImage, int numColsTemplate, int numRowsTemplate,
						double *corrImage,
						short *alignedImage)
{
	int superSampleLevel = 2;
	IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	IplImage *ipOrigImageF1 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	IplImage *ipOrigImageF2 = cvCreateImage(cvSize(numCols * superSampleLevel, numRows * superSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *dummyF = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *ipTemplateImage = cvCreateImageHeader(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_16S, 1);
	IplImage *ipTemplateImageF1 = cvCreateImage(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_32F, 1);
	IplImage *ipTemplateImageF2 = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *corrSurfaceImage = cvCreateImage(cvSize(numCols - numColsTemplate + 1, numRows - numRowsTemplate + 1), IPL_DEPTH_32F, 1);
	IplImage *corrSurfaceImageF2 = cvCreateImage(cvSize((numCols * superSampleLevel) - (numColsTemplate * superSampleLevel) + 1, (numRows * superSampleLevel) - (numRowsTemplate * superSampleLevel) + 1), IPL_DEPTH_32F, 1);

	try
	{
		cvSetImageData(ipOrigImage, origImage, numCols * sizeof(short));
		cvConvert(ipOrigImage, ipOrigImageF1);
		cvResize(ipOrigImageF1, ipOrigImageF2, CV_INTER_LANCZOS4); // UPSAMPLE

		cvSetImageData(ipTemplateImage, templateImage, numColsTemplate * sizeof(short));
		cvConvert(ipTemplateImage, ipTemplateImageF1);
		cvResize(ipTemplateImageF1, ipTemplateImageF2, CV_INTER_LANCZOS4); // UPSAMPLE

		cvMatchTemplate(ipOrigImageF2, ipTemplateImageF2, corrSurfaceImageF2, CV_TM_CCOEFF_NORMED);
		//printf("CHECK: Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d) :: (numCols, numRows) = (%d,%d)\n", numColsTemplate, numRowsTemplate, numCols, numRows);
		//cvResize(corrSurfaceImage, corrSurfaceImageF2, CV_INTER_LANCZOS4);

		double minVal = 0.0, maxVal = 0.0;
		CvPoint minLoc, maxLoc;
		cvMinMaxLoc(corrSurfaceImageF2, &minVal, &maxVal, &minLoc, &maxLoc, 0);

		//superSampleLevel = 1;
		int leftLine = (maxLoc.x - ((numColsTemplate * superSampleLevel) - 1) / 2);
		int rightLine = leftLine + (numColsTemplate * superSampleLevel) - 1;

		int topLine = (maxLoc.y - ((numRowsTemplate * superSampleLevel) - 1) / 2);
		int bottomLine = topLine + (numRowsTemplate * superSampleLevel) - 1;

		for (unsigned int idx1 = topLine; idx1 <= bottomLine; idx1++)
		{
			for (unsigned int idx2 = leftLine; idx2 <= rightLine; idx2++)
			{
				//long vectorIdx = (idx1 * (numColsTemplate * superSampleLevel)) + idx2;
				CV_IMAGE_ELEM(dummyF, float, idx1 - topLine, idx2 - leftLine) = CV_IMAGE_ELEM(ipOrigImageF2, float, idx1, idx2);
			}
		}
		cvResize(dummyF, ipTemplateImageF1, CV_INTER_LANCZOS4);
		
		for (unsigned int idx1 = 0; idx1 < numRowsTemplate; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numColsTemplate; idx2++)
			{
				long vectorIdx = (idx1 * numColsTemplate) + idx2;
				alignedImage[vectorIdx] = CV_IMAGE_ELEM(ipTemplateImageF1, float, idx1, idx2);
			}
		}

		cvResize(corrSurfaceImageF2, corrSurfaceImage, CV_INTER_AREA);

		for (unsigned int idx1 = 0; idx1 < numRows - numRowsTemplate + 1; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numCols - numColsTemplate + 1; idx2++)
			{
				long vectorIdx = (idx1 * (numCols - numColsTemplate + 1)) + idx2;
				corrImage[vectorIdx] = CV_IMAGE_ELEM(corrSurfaceImage, float, idx1, idx2);
			}
		}
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
		printf("matchTemplate() -> %s\n", stdException.what());
	}

	cvReleaseImageHeader(&ipOrigImage);
	cvReleaseImageHeader(&ipTemplateImage);

	cvReleaseImage(&corrSurfaceImage);
	cvReleaseImage(&corrSurfaceImageF2);
	cvReleaseImage(&ipOrigImageF1);
	cvReleaseImage(&ipOrigImageF2);
	cvReleaseImage(&dummyF);
	cvReleaseImage(&ipTemplateImageF1);
	cvReleaseImage(&ipTemplateImageF2);
}

// <FOCUS_METRICS>
void computeHistogram(IplImage *imageData, IplImage *histVector, int numHistBins)
{
	IplImage **imageData2 = (IplImage **)malloc(sizeof(IplImage *));
	imageData2[0] = imageData;
	float range[] = { 0, 4096 };
	float *ranges[] = { range };

	CvHistogram* cvHist = cvCreateHist(1, &numHistBins, CV_HIST_ARRAY, ranges, 1);
	cvCalcHist(imageData2, cvHist, 0, (const CvArr *)NULL);

	for (int hist_i = 0; hist_i<numHistBins; hist_i++)
	{
		CV_IMAGE_ELEM(histVector, double, hist_i, 0) = (double)cvQueryHistValue_1D(cvHist, hist_i);
	}

	if (DEBUG_PRINT)
	{
		//printHistogram(cvHist, numHistBins);
	}
}

double getEntropy(IplImage *image)
{
	int numBins = 512;

	IplImage *histVector = cvCreateImage(cvSize(1, numBins), IPL_DEPTH_64F, 1);
	cvSetZero(histVector);

	computeHistogram(image, histVector, numBins);
	cvScale(histVector, histVector, (1.0 / (image->height * image->width)), 0);

	double sumPiLog2Pi = 0.0;
	double checkSum = 0.0;

	for (int cnt = 0; cnt < 512; cnt++)
	{
		double histVal = CV_IMAGE_ELEM(histVector, double, cnt, 0);
		if (histVal != 0)
		{
			double logProb = log2(histVal);
			sumPiLog2Pi += (histVal * logProb);
			checkSum += histVal;
		}
	}

	cvReleaseImage(&histVector);
	return -1.0 * sumPiLog2Pi;
}

// GT_TODO: Modify this to suit needs
double GetSharpness(IplImage *in)
{
	IplImage* out = cvCreateImage(cvSize(in->width, in->height), in->depth, 1);

	// aperture size of 1 corresponds to the correct matrix
	cvLaplace(in, out, 1);

	double maxLap = -32767;
	short* imgData = (short*)out->imageData;
	for (int i = 0; i<(out->imageSize / 2); i++)
	{
		if (imgData[i] > maxLap)
		{
			maxLap = (double)imgData[i];
		}
	}

	maxLap /= 10.0;

	cvReleaseImage(&out);
	return maxLap;
}

// OpenCV port of 'GLVN' algorithm (Santos97)
double normalizedGraylevelVariance(IplImage *in)
{
	const cv::Mat src(in);
	cv::Scalar mu, sigma;
	cv::meanStdDev(src, mu, sigma);

	double focusMeasure = (sigma.val[0] * sigma.val[0]) / mu.val[0];
	return focusMeasure;
}
// </FOCUS_METRICS>

void extractFocusMetricsFromImageData(short* imageData, int numCols, int numRows, double* metrics)
{
	IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	IplImage *ipOrigImageF1 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	cvSetImageData(ipOrigImage, imageData, numCols * sizeof(short));
	cvConvert(ipOrigImage, ipOrigImageF1);

	//IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	//metrics[0] = computeFocus_Type1(imageData);
	metrics[0] = GetSharpness(ipOrigImage) * 10;
	metrics[1] = normalizedGraylevelVariance(ipOrigImage);
	metrics[2] = getEntropy(ipOrigImageF1) * 10; //varianceOfLaplacian(imageData);
	
	cvReleaseImage(&ipOrigImageF1);
	cvReleaseImageHeader(&ipOrigImage);
}

void matchTemplateDFT(	short *origImage, int numCols, int numRows,
						short *templateImage, int numColsTemplate, int numRowsTemplate,
						double *corrImage)
{
	int superSampleLevel = 1;
	IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	IplImage *ipOrigImageF1 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	IplImage *ipOrigImageF2 = cvCreateImage(cvSize(numCols * superSampleLevel, numRows * superSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *dummyF = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *ipTemplateImage = cvCreateImageHeader(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_16S, 1);
	IplImage *ipTemplateImageF1 = cvCreateImage(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_32F, 1);
	IplImage *ipTemplateImageF2 = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *corrSurfaceImage = cvCreateImage(cvSize(numCols - numColsTemplate + 1, numRows - numRowsTemplate + 1), IPL_DEPTH_32F, 1);
	IplImage *corrSurfaceImageF2 = cvCreateImage(cvSize((numCols * superSampleLevel) - (numColsTemplate * superSampleLevel) + 1, (numRows * superSampleLevel) - (numRowsTemplate * superSampleLevel) + 1), IPL_DEPTH_32F, 1);

	try
	{
		cvSetImageData(ipOrigImage, origImage, numCols * sizeof(short));
		cvConvert(ipOrigImage, ipOrigImageF1);
		cvResize(ipOrigImageF1, ipOrigImageF2, CV_INTER_LANCZOS4); // UPSAMPLE

		cvSetImageData(ipTemplateImage, templateImage, numColsTemplate * sizeof(short));
		cvConvert(ipTemplateImage, ipTemplateImageF1);
		cvResize(ipTemplateImageF1, ipTemplateImageF2, CV_INTER_LANCZOS4); // UPSAMPLE

		cvMatchTemplate(ipOrigImageF2, ipTemplateImageF2, corrSurfaceImageF2, CV_TM_CCOEFF_NORMED);
		printf("CHECK: Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d) :: (numCols, numRows) = (%d,%d)\n", numColsTemplate, numRowsTemplate, numCols, numRows);
		//cvResize(corrSurfaceImage, corrSurfaceImageF2, CV_INTER_LANCZOS4);

		double minVal = 0.0, maxVal = 0.0;
		CvPoint minLoc, maxLoc;
		cvMinMaxLoc(corrSurfaceImageF2, &minVal, &maxVal, &minLoc, &maxLoc, 0);

		superSampleLevel = 1;
		int leftLine = (maxLoc.x - ((numColsTemplate * superSampleLevel) - 1) / 2);
		int rightLine = leftLine + (numColsTemplate * superSampleLevel) - 1;

		int topLine = (maxLoc.y - ((numRowsTemplate * superSampleLevel) - 1) / 2);
		int bottomLine = topLine + (numRowsTemplate * superSampleLevel) - 1;

		//for (unsigned int idx1 = topLine; idx1 <= bottomLine; idx1++)
		//{
		//	for (unsigned int idx2 = leftLine; idx2 <= rightLine; idx2++)
		//	{
		//		//long vectorIdx = (idx1 * (numColsTemplate * superSampleLevel)) + idx2;
		//		CV_IMAGE_ELEM(dummyF, float, idx1 - topLine, idx2 - leftLine) = CV_IMAGE_ELEM(ipOrigImageF2, float, idx1, idx2);
		//	}
		//}
		//cvResize(dummyF, ipTemplateImageF1, CV_INTER_LANCZOS4);

		cvResize(corrSurfaceImageF2, corrSurfaceImage, CV_INTER_AREA);

		for (unsigned int idx1 = 0; idx1 < numRows - numRowsTemplate + 1; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numCols - numColsTemplate + 1; idx2++)
			{
				long vectorIdx = (idx1 * (numCols - numColsTemplate + 1)) + idx2;
				corrImage[vectorIdx] = CV_IMAGE_ELEM(corrSurfaceImage, float, idx1, idx2);
			}
		}
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
		printf("matchTemplate() -> %s\n", stdException.what());
	}

	cvReleaseImageHeader(&ipOrigImage);
	cvReleaseImageHeader(&ipTemplateImage);

	cvReleaseImage(&corrSurfaceImage);
	cvReleaseImage(&corrSurfaceImageF2);
	cvReleaseImage(&ipOrigImageF1);
	cvReleaseImage(&ipOrigImageF2);
	cvReleaseImage(&dummyF);
	cvReleaseImage(&ipTemplateImageF1);
	cvReleaseImage(&ipTemplateImageF2);
}

/\*
cvResize(cvCorr1X, cvCorr5X, CV_INTER_CUBIC);
cvMinMaxLoc(cvCorr1X, &minVal, &maxVal, &minLoc, &maxLoc, 0);
cvMinMaxLoc(cvCorr5X, &minVal, &maxVal, &minLoc, &maxLoc, 0);
//cvMinMaxLoc(cvCorr1X, &minVal, &maxVal, &minLoc, &maxLoc, 0);

resultLoc.x = maxLoc.x/5.0;
resultLoc.y = maxLoc.y/5.0;

CvPoint2D32f newCenter;
//newCenter.x = alignImageWidth/2  + floor(resultLoc.x - targetROI.x + 0.5);
//newCenter.y = alignImageHeight/2 + floor(resultLoc.y - targetROI.y + 0.5);

newCenter.x = alignImageWidth/2  + (resultLoc.x - targetROI.x);
newCenter.y = alignImageHeight/2 + (resultLoc.y - targetROI.y);

cvGetRectSubPix(cvTargetImage, cvAlignOutput, newCenter);

cvSetImageROI(cvAlignOutput, targetROI);
cvCopy(cvAlignOutput, cvChoppedImage, NULL);
cvResetImageROI(cvAlignOutput);
*\/

int matchTemplateWithUpsampledCorr(	short *origImage, int numCols, int numRows,
									short *templateImage, int numColsTemplate, int numRowsTemplate,
									short *outImage, // Same size as template Image
									double CORR_UPSAMPLE_MAG,
									double *corrImage) // Pre-allocated array of size [(numCols - numColsTemplate + 1) * CORR_UPSAMPLE_MAG] x [(numRows - numRowsTemplate + 1) * CORR_UPSAMPLE_MAG]
{
	int superSampleLevel = 2;
	int corrUpSampleLevel = CORR_UPSAMPLE_MAG;
	int retVal = 0;
	int numColsUpsampledCorrImage = (numCols * superSampleLevel * corrUpSampleLevel) - (numColsTemplate * superSampleLevel * corrUpSampleLevel) + 1;
	int numRowsUpsampledCorrImage = (numRows * superSampleLevel * corrUpSampleLevel) - (numRowsTemplate * superSampleLevel * corrUpSampleLevel) + 1;
	float* dummyCheck = (float *)malloc(numColsUpsampledCorrImage * numRowsUpsampledCorrImage * sizeof(float));
	
	IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	IplImage *ipOrigImageF1 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	IplImage *ipOrigImageF2 = cvCreateImage(cvSize(numCols * superSampleLevel, numRows * superSampleLevel), IPL_DEPTH_32F, 1);
	IplImage *ipOrigImageF3 = cvCreateImage(cvSize(numCols * superSampleLevel * corrUpSampleLevel, numRows * superSampleLevel * corrUpSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *dummyF = cvCreateImage(cvSize(numColsTemplate * superSampleLevel * corrUpSampleLevel, numRowsTemplate * superSampleLevel * corrUpSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *ipTemplateImage = cvCreateImageHeader(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_16S, 1);
	IplImage *ipTemplateImageF1 = cvCreateImage(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_32F, 1);
	IplImage *ipTemplateImageF2 = cvCreateImage(cvSize(numColsTemplate * superSampleLevel, numRowsTemplate * superSampleLevel), IPL_DEPTH_32F, 1);
	IplImage *ipTemplateImageF3 = cvCreateImage(cvSize(numColsTemplate * superSampleLevel * corrUpSampleLevel, numRowsTemplate * superSampleLevel * corrUpSampleLevel), IPL_DEPTH_32F, 1);

	IplImage *corrSurfaceImage = cvCreateImage(cvSize(numCols - numColsTemplate + 1, numRows - numRowsTemplate + 1), IPL_DEPTH_32F, 1);
	IplImage *corrSurfaceImageF2 = cvCreateImage(cvSize((numCols * superSampleLevel) - (numColsTemplate * superSampleLevel) + 1, (numRows * superSampleLevel) - (numRowsTemplate * superSampleLevel) + 1), IPL_DEPTH_32F, 1);
	IplImage *corrSurfaceImageF3 = cvCreateImage(cvSize((numCols * superSampleLevel * corrUpSampleLevel) - (numColsTemplate * superSampleLevel * corrUpSampleLevel) + 1, (numRows * superSampleLevel * corrUpSampleLevel) - (numRowsTemplate * superSampleLevel * corrUpSampleLevel) + 1), IPL_DEPTH_32F, 1);

	try
	{
		cvSetImageData(ipOrigImage, origImage, numCols * sizeof(short));
		cvConvert(ipOrigImage, ipOrigImageF1);
		cvResize(ipOrigImageF1, ipOrigImageF2, CV_INTER_LANCZOS4); // UPSAMPLE
		cvResize(ipOrigImageF1, ipOrigImageF3, CV_INTER_LANCZOS4); // UPSAMPLE

		cvSetImageData(ipTemplateImage, templateImage, numColsTemplate * sizeof(short));
		cvConvert(ipTemplateImage, ipTemplateImageF1);
		cvResize(ipTemplateImageF1, ipTemplateImageF2, CV_INTER_LANCZOS4); // UPSAMPLE
		cvResize(ipTemplateImageF1, ipTemplateImageF3, CV_INTER_LANCZOS4); // UPSAMPLE

		cvMatchTemplate(ipOrigImageF2, ipTemplateImageF2, corrSurfaceImageF2, CV_TM_CCOEFF_NORMED);
		printf("CHECK: Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d) :: (numCols, numRows) = (%d,%d)\n", numColsTemplate, numRowsTemplate, numCols, numRows);
		cvResize(corrSurfaceImageF2, corrSurfaceImageF3, CV_INTER_LANCZOS4);

		double minVal = 0.0, maxVal = 0.0;
		CvPoint minLoc, maxLoc;
		cvMinMaxLoc(corrSurfaceImageF3, &minVal, &maxVal, &minLoc, &maxLoc, 0);

		for (unsigned int idx1 = 0; idx1 < numRowsUpsampledCorrImage; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numColsUpsampledCorrImage; idx2++)
			{
				long vectorIdx = (idx1 * numColsUpsampledCorrImage) + idx2;
				float tempVal = CV_IMAGE_ELEM(corrSurfaceImageF3, float, idx1, idx2);
				dummyCheck[vectorIdx] = tempVal;
			}
		}

		maxLoc.x -= (corrSurfaceImageF3->width / 2);
		maxLoc.y -= (corrSurfaceImageF3->height / 2);

		int leftLine = ((numCols * superSampleLevel * corrUpSampleLevel) / 2) - maxLoc.x - ((numColsTemplate * superSampleLevel * corrUpSampleLevel)) / 2;
		int rightLine = leftLine + ((numColsTemplate * superSampleLevel * corrUpSampleLevel));

		int topLine = ((numRows * superSampleLevel * corrUpSampleLevel) / 2) - maxLoc.y - ((numRowsTemplate * superSampleLevel * corrUpSampleLevel)) / 2;
		int bottomLine = topLine + ((numRowsTemplate * superSampleLevel * corrUpSampleLevel));

		for (unsigned int idx1 = topLine; idx1 < bottomLine; idx1++)
		{
			for (unsigned int idx2 = leftLine; idx2 < rightLine; idx2++)
			{
				//long vectorIdx = (idx1 * (numColsTemplate * superSampleLevel)) + idx2;
				CV_IMAGE_ELEM(dummyF, float, idx1 - topLine, idx2 - leftLine) = CV_IMAGE_ELEM(ipOrigImageF3, float, idx1, idx2);
			}
		}
		
		cvResize(dummyF, ipTemplateImageF1, CV_INTER_LANCZOS4);

		for (unsigned int idx1 = 0; idx1 < numRowsTemplate; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numColsTemplate; idx2++)
			{
				long vectorIdx = (idx1 * numColsTemplate) + idx2;
				outImage[vectorIdx] = CV_IMAGE_ELEM(ipTemplateImageF1, float, idx1, idx2);
			}
		}

		for (unsigned int idx1 = 0; idx1 < ((numRows - numRowsTemplate) * superSampleLevel) + 1; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < ((numCols - numColsTemplate) * superSampleLevel) + 1; idx2++)
			{
				long vectorIdx = (idx1 * (numCols - numColsTemplate + 1) * superSampleLevel) + idx2;
				float tempVal = CV_IMAGE_ELEM(corrSurfaceImageF2, float, idx1, idx2);
				corrImage[vectorIdx] = tempVal;
			}
		}

		retVal = 0;
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
		printf("matchTemplate() -> %s\n", stdException.what());
		retVal = -1;
	}

	cvReleaseImageHeader(&ipOrigImage);
	cvReleaseImageHeader(&ipTemplateImage);

	cvReleaseImage(&corrSurfaceImage);
	cvReleaseImage(&corrSurfaceImageF2);
	cvReleaseImage(&ipOrigImageF1);
	cvReleaseImage(&ipOrigImageF2);
	cvReleaseImage(&ipOrigImageF3);

	cvReleaseImage(&dummyF);

	cvReleaseImage(&ipTemplateImageF1);
	cvReleaseImage(&ipTemplateImageF2);
	cvReleaseImage(&ipTemplateImageF3);

	if (dummyCheck != NULL)
	{
		free(dummyCheck);
	}

	return retVal;
}

int matchTemplateWithUpsampledCorr2(short *origImage, int numCols, int numRows,
									short *templateImage, int numColsTemplate, int numRowsTemplate,
									short *outImage, // Same size as template Image
									double CORR_UPSAMPLE_MAG,
									double *corrImage)
{
	int numColsCorr1X = numCols - numColsTemplate + 1;
	int numRowsCorr1X = numRows - numRowsTemplate + 1;
	int retVal = 0;
	
	IplImage *ipOrigImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	IplImage *ipOrigImageF = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	IplImage *ipOrigImageF2 = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	
	IplImage *opImageF = cvCreateImage(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_32F, 1);
	CvRect targetROI;
	targetROI.width = numColsTemplate;
	targetROI.height = numRowsTemplate;

	IplImage *ipTemplateImage = cvCreateImageHeader(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_16S, 1);
	IplImage *ipTemplateImageF = cvCreateImage(cvSize(numColsTemplate, numRowsTemplate), IPL_DEPTH_32F, 1);

	IplImage *corrSurfaceImage = cvCreateImage(cvSize(numColsCorr1X, numRowsCorr1X), IPL_DEPTH_32F, 1);
	IplImage *cvCorr5X = cvCreateImage(cvSize(numColsCorr1X * CORR_UPSAMPLE_MAG, numRowsCorr1X * CORR_UPSAMPLE_MAG), IPL_DEPTH_32F, 1);

	try
	{
		cvSetImageData(ipOrigImage, origImage, numCols * sizeof(short));
		cvConvert(ipOrigImage, ipOrigImageF);

		cvSetImageData(ipTemplateImage, templateImage, numColsTemplate * sizeof(short));
		cvConvert(ipTemplateImage, ipTemplateImageF);

		cvMatchTemplate(ipOrigImageF, ipTemplateImageF, corrSurfaceImage, CV_TM_CCOEFF_NORMED);
		printf("CHECK: Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d) :: (numCols, numRows) = (%d,%d)\n", numColsTemplate, numRowsTemplate, numCols, numRows);

		double minVal = 0.0, maxVal = 0.0;
		CvPoint minLoc, maxLoc;
		double resultLoc_x = 0.0, resultLoc_y = 0.0;
		
		cvMinMaxLoc(corrSurfaceImage, &minVal, &maxVal, &minLoc, &maxLoc, 0);

		cvResize(corrSurfaceImage, cvCorr5X, CV_INTER_LANCZOS4);
		cvMinMaxLoc(cvCorr5X, &minVal, &maxVal, &minLoc, &maxLoc, 0);

		resultLoc_x = double(maxLoc.x)/CORR_UPSAMPLE_MAG;
		resultLoc_y = double(maxLoc.y)/CORR_UPSAMPLE_MAG;

		resultLoc_x -= (numColsCorr1X/2);
		resultLoc_y -= (numRowsCorr1X/2);

		CvPoint2D32f newCenter;
		//newCenter.x = alignImageWidth/2  + floor(resultLoc.x - targetROI.x + 0.5);
		//newCenter.y = alignImageHeight/2 + floor(resultLoc.y - targetROI.y + 0.5);

		newCenter.x = (numCols + 1)/ 2 + (resultLoc_x); // Final '- 1' is because cvGetRectSubPix works as: [des <- src(x + center.x - (dst.cols - 1)/2, y + center.y - (dst.rows - 1)/2)
		newCenter.y = (numRows + 1)/ 2 + (resultLoc_y);

		// *********
		cvGetRectSubPix(ipOrigImageF, ipOrigImageF2, newCenter);
		// *********
		
		targetROI.x = numCols/2 - (numColsTemplate/2);
		targetROI.y = numRows/2 - (numRowsTemplate/2);
		cvSetImageROI(ipOrigImageF2, targetROI);
		cvCopy(ipOrigImageF2, opImageF, NULL);
		cvResetImageROI(ipOrigImageF2);

		for (unsigned int idx1 = targetROI.y; idx1 < (targetROI.y + numRowsTemplate); idx1++)
		{
			for (unsigned int idx2 = targetROI.x; idx2 < (targetROI.x + numColsTemplate); idx2++)
			{
				long vectorIdx = ((idx1 - targetROI.y)* numColsTemplate) + (idx2 - targetROI.x);
				float tempVal = CV_IMAGE_ELEM(ipOrigImageF2, float, idx1, idx2);
				outImage[vectorIdx] = round(tempVal);
			}
		}

		for (unsigned int idx1 = 0; idx1 < numRowsCorr1X * CORR_UPSAMPLE_MAG; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numColsCorr1X * CORR_UPSAMPLE_MAG; idx2++)
			{
				long vectorIdx = (idx1 * (numRowsCorr1X * CORR_UPSAMPLE_MAG)) + idx2;
				float tempVal = CV_IMAGE_ELEM(cvCorr5X, float, idx1, idx2);
				corrImage[vectorIdx] = tempVal;
			}
		}
		retVal = 0;
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform matchTemplate with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numColsTemplate, numRowsTemplate);
		printf("matchTemplate() -> %s\n", stdException.what());

		retVal = -1;
	}

	cvReleaseImageHeader(&ipOrigImage);
	cvReleaseImageHeader(&ipTemplateImage);

	cvReleaseImage(&cvCorr5X);
	cvReleaseImage(&corrSurfaceImage);
	
	cvReleaseImage(&opImageF);
	cvReleaseImage(&ipOrigImageF2);
	cvReleaseImage(&ipOrigImageF);
	cvReleaseImage(&ipTemplateImageF);

	return retVal;
}

void sharpenImage(short *imageDataIn, short *imageDataOut, int numCols, int numRows, short kernelSize)
{
	try
	{
		IplImage *outImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
		IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);

		cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));

		//cv::GaussianBlur(frame, image, cv::Size(0, 0), 3);
		//cv::addWeighted(frame, 1.5, image, -0.5, 0, image);
		cvSmooth(ipImage, outImage, CV_GAUSSIAN, kernelSize, kernelSize);
		cvAddWeighted(ipImage, 1.5, outImage, -0.5, 0, outImage);

		for (unsigned int idx1 = 0; idx1 < numCols; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numRows; idx2++)
			{
				int vectorIdx = (idx1 * numCols) + idx2;
				imageDataOut[vectorIdx] = cvRound(CV_IMAGE_ELEM(outImage, short, idx1, idx2));
			}
		}

		cvReleaseImageHeader(&ipImage);
		cvReleaseImage(&outImage);
		//cvReleaseImage(&ipImageF);
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform applyFilter with kernelSize = %d \n", kernelSize);
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
	}
}

void applyFilter(short *imageDataIn, short *imageDataOut, short numRows, short numCols, short* kernel, short kernelSize)
{
	try
	{
		IplImage *outImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
		IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
		IplImage *ipKernel = cvCreateImageHeader(cvSize(kernelSize, kernelSize), IPL_DEPTH_16S, 1);
		//IplImage *ipImageF = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);

		cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));
		cvSetImageData(ipKernel, kernel, kernelSize * sizeof(short));
		
		//Mat mtx(ipKernel); // convert IplImage* -> Mat
		//CvMat myKernel = mtx; // convert Mat -> CvMat

		CvMat myKernel = cvMat(kernelSize, kernelSize, CV_16S, kernel);
		CvScalar sum1 = cvSum(ipKernel);

		cvFilter2D(ipImage, outImage, &myKernel); // , 0.0, BORDER_DEFAULT);

		for (unsigned int idx1 = 0; idx1 < numCols; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numRows; idx2++)
			{
				int vectorIdx = (idx1 * numCols) + idx2;
				imageDataOut[vectorIdx] = cvRound(CV_IMAGE_ELEM(outImage, short, idx1, idx2) / sum1.val[0]);
			}
		}

		cvReleaseImageHeader(&ipImage);
		cvReleaseImage(&outImage);
		//cvReleaseImage(&ipImageF);
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform applyFilter with kernelSize = %d \n", kernelSize);
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
	}
}

void applyLPF(short *imageDataIn, short *imageDataOut, short numRows, short numCols, short kernelSize)
{
	try
	{
		IplImage *outImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
		IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
		//IplImage *ipImageF = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
		
		cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));
		//cvConvert(ipImage, ipImageF);
		
		cvSmooth(ipImage, outImage, CV_GAUSSIAN, kernelSize, kernelSize);

		for (unsigned int idx1 = 0; idx1 < numCols; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numRows; idx2++)
			{
				int vectorIdx = (idx1 * numCols) + idx2;
				imageDataOut[vectorIdx] = cvRound(CV_IMAGE_ELEM(outImage, short, idx1, idx2));
			}
		}

		cvReleaseImageHeader(&ipImage);
		cvReleaseImage(&outImage);
		//cvReleaseImage(&ipImageF);
	}
	catch (std::exception &stdException)
	{
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
	}
}

int resizeImage(char *imageFileName, float UPSAMPLE_MAG, short *imageDataOut, int numColsOut, int numRowsOut)
{
	try
	{
		IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
		IplImage *cvPreInterpImage2 = cvCreateImage(cvSize(imageData->width, imageData->height), IPL_DEPTH_32F, 1);
		//IplImage *cvPreInterpImage = cvCreateImageHeader(cvSize(imageData->width, imageData->height), IPL_DEPTH_16S, 1);

		int alignImageWidth = numColsOut; //cvRound((UPSAMPLE_MAG)* imageData->width);
		int alignImageHeight = numRowsOut; // cvRound((UPSAMPLE_MAG)* imageData->height);
		IplImage *cvInterpImage = cvCreateImage(cvSize(alignImageWidth, alignImageHeight), IPL_DEPTH_32F, 1);

		//cvSetImageData(cvPreInterpImage, imageData->imageData, imageData->width * sizeof(short));
		cvConvert(imageData, cvPreInterpImage2); // To Float

		if (UPSAMPLE_MAG == 1.0)
		{
			cvCopy(cvPreInterpImage2, cvInterpImage, NULL);
		}
		else if (UPSAMPLE_MAG > 1.0)
		{
			cvResize(cvPreInterpImage2, cvInterpImage, CV_INTER_LANCZOS4); // UPSAMPLE
			//cvSmooth(cvInterpImage, cvInterpImage, CV_GAUSSIAN, 5, 5); // SMOOTH
		}
		else
		{
			cvResize(cvPreInterpImage2, cvInterpImage, CV_INTER_AREA); // CV_INTER_AREA
			cvSmooth(cvInterpImage, cvInterpImage, CV_GAUSSIAN, 5, 5);
		}

		//*imageDataOut = (short *)malloc(alignImageWidth * alignImageHeight * sizeof(short));

		for (unsigned int idx1 = 0; idx1 < alignImageHeight; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < alignImageWidth; idx2++)
			{
				int vectorIdx = (idx1 * alignImageWidth) + idx2;
				imageDataOut[vectorIdx] = cvRound(CV_IMAGE_ELEM(cvInterpImage, float, idx1, idx2));
			}
		}

		//cvReleaseImageHeader(&cvPreInterpImage);
		cvReleaseImage(&cvPreInterpImage2);
		cvReleaseImage(&cvInterpImage);
		cvReleaseImage(&imageData);

		return 0;
	}
	catch (std::exception &stdException)
	{
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
		//FILE *fp2 = fopen( "D:\\LOG.txt", "a+");
		//fprintf(fp2, " ================================ \n\n");
		//fprintf(fp2, stdException.what());
		//fclose(fp2);
		return -1;
	}
}

void resizeImageFromData(short *inputData, int numRowsIn, int numColsIn, float UPSAMPLE_MAG, short *imageDataOut, int numColsOut, int numRowsOut)
{
	try
	{
		IplImage *inputImage = cvCreateImageHeader(cvSize(numColsIn, numRowsIn), IPL_DEPTH_16S, 1);
		IplImage *cvPreInterpImage2 = cvCreateImage(cvSize(numColsIn, numRowsIn), IPL_DEPTH_32F, 1);

		int alignImageWidth = numColsOut; //cvRound((UPSAMPLE_MAG)* imageData->width);
		int alignImageHeight = numRowsOut; // cvRound((UPSAMPLE_MAG)* imageData->height);
		IplImage *cvInterpImage = cvCreateImage(cvSize(alignImageWidth, alignImageHeight), IPL_DEPTH_32F, 1);

		cvSetImageData(inputImage, inputData, numColsIn * sizeof(short));
		cvConvert(inputImage, cvPreInterpImage2); // To Float
		if (UPSAMPLE_MAG >= 1.0)
		{
			cvResize(cvPreInterpImage2, cvInterpImage, CV_INTER_LANCZOS4); // UPSAMPLE
		}
		else
		{
			cvResize(cvPreInterpImage2, cvInterpImage, CV_INTER_AREA); // CV_INTER_AREA
			//cvSmooth(cvInterpImage, cvInterpImage, CV_GAUSSIAN, 5, 5);
		}

		//*imageDataOut = (short *)malloc(alignImageWidth * alignImageHeight * sizeof(short));

		for (unsigned int idx1 = 0; idx1 < alignImageHeight; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < alignImageWidth; idx2++)
			{
				int vectorIdx = (idx1 * alignImageWidth) + idx2;
				//short temp = cvRound((float *)((*cvInterpImage).imageData)[vectorIdx]); //cvRound(CV_IMAGE_ELEM(cvInterpImage, float, idx2, idx1));
				short temp = 0; 
				if (alignImageHeight == 1)
				{
					temp = cvRound(CV_IMAGE_ELEM(cvInterpImage, float, idx1, idx2));
				}
				else
				{
					temp = cvRound(CV_IMAGE_ELEM(cvInterpImage, float, idx1, idx2));
				}
				imageDataOut[vectorIdx] = temp;
			}
		}

		//cvReleaseImageHeader(&cvPreInterpImage);
		cvReleaseImage(&cvPreInterpImage2);
		cvReleaseImage(&cvInterpImage);
		cvReleaseImageHeader(&inputImage);
	}
	catch (std::exception &stdException)
	{
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
		//FILE *fp2 = fopen( "D:\\LOG.txt", "a+");
		//fprintf(fp2, " ================================ \n\n");
		//fprintf(fp2, stdException.what());
		//fclose(fp2);
	}
}


int loadImageData(char* imageFileName, unsigned short* imageDataArray, int numRows, int numCols)
{
	IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

	if ( (numRows != imageData->height) || (numCols != imageData->widthStep / sizeof(short)) )
	{
		cvReleaseImage(&imageData);
		return -1;
	}

	long numPix = imageData->height * imageData->width;
	long vecIdx = 0;
	
	for (int imatX = 0; imatX < imageData->height; imatX++)
	{
		for (int imatY = 0; imatY < imageData->width; imatY++)
		{
			imageDataArray[vecIdx] = CV_IMAGE_ELEM(imageData, unsigned short, imatX, imatY);
			vecIdx++;
		}
	}

	cvReleaseImage(&imageData);
	return vecIdx;
}

int loadImageDataShort(char* imageFileName, short* imageDataArray, int numRows, int numCols)
{
	IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

	if ((numRows != imageData->height) || (numCols != imageData->widthStep / sizeof(short)))
	{
		cvReleaseImage(&imageData);
		return -1;
	}

	long numPix = imageData->height * imageData->width;
	long vecIdx = 0;

	for (int imatX = 0; imatX < imageData->height; imatX++)
	{
		for (int imatY = 0; imatY < imageData->width; imatY++)
		{
			imageDataArray[vecIdx] = CV_IMAGE_ELEM(imageData, short, imatY, imatX);
			vecIdx++;
		}
	}

	cvReleaseImage(&imageData);
	return vecIdx;
}

double computeFocus_Type1(IplImage *imageData)
{
	double focusMetric = 0.0;
	float kernelArray[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
	CvMat *kernel1 = cvCreateMat(9, 1, CV_32FC1);
	
	for (int i = 0; i < 9; i++)
	{ 
		cvmSet(kernel1, i, 0, kernelArray[i]);
	}

	cvFilter2D(imageData, imageData, kernel1, cvPoint(-1, -1));

	CvScalar imageSum = cvSum(imageData);
	focusMetric = (double)(imageSum.val[0]) / (double)(imageData->height * imageData->height);

	return focusMetric;
}

// OpenCV port of 'LAPM' algorithm (Nayar89)
double modifiedLaplacian(IplImage *in)
{
	cv::Mat src(in);
	cv::Mat M = (Mat_<double>(3, 1) << -1, 2, -1);
	cv::Mat G = cv::getGaussianKernel(3, -1, CV_64F);

	cv::Mat Lx;
	cv::sepFilter2D(src, Lx, CV_64F, M, G);

	cv::Mat Ly;
	cv::sepFilter2D(src, Ly, CV_64F, G, M);

	cv::Mat FM = cv::abs(Lx) + cv::abs(Ly);

	double focusMeasure = cv::mean(FM).val[0];
	return focusMeasure;
}

// OpenCV port of 'LAPV' algorithm (Pech2000)
double varianceOfLaplacian(IplImage *in)
{
	cv::Mat src(in);
	cv::Mat lap;
	cv::Laplacian(src, lap, CV_64F);

	cv::Scalar mu, sigma;
	cv::meanStdDev(lap, mu, sigma);

	double focusMeasure = abs(sigma.val[0]); // *sigma.val[0];
	return focusMeasure;
}

// OpenCV port of 'TENG' algorithm (Krotkov86)
double tenengrad(IplImage *in, int ksize)
{
	double focusMeasure = 0.0;
	IplImage* xSobel = cvCreateImage(cvSize(in->width, in->height), in->depth, 1);
	IplImage* ySobel = cvCreateImage(cvSize(in->width, in->height), in->depth, 1);

	try
	{
		cvSobel(in, xSobel, 1, 0, ksize);
		cvSobel(in, ySobel, 0, 1, ksize);

		cvMul(xSobel, xSobel, xSobel, 1.0);
		cvMul(ySobel, ySobel, ySobel, 1.0);

		cvAdd(xSobel, ySobel, xSobel, NULL);
		CvScalar out = cvAvg(xSobel, NULL);

		focusMeasure = out.val[0];
	}
	catch (std::exception &stdException)
	{
		const char* err = stdException.what();
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
	}
	return focusMeasure;
}


void extractFocusMetricsFromImage(char* imageFileName, double* metrics)
{
	IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	//metrics[0] = computeFocus_Type1(imageData);
	metrics[0] = GetSharpness(imageData);
	metrics[2] = getEntropy(imageData) * 10; //varianceOfLaplacian(imageData);
	metrics[1] = normalizedGraylevelVariance(imageData);
	cvReleaseImage(&imageData);
}


void computeImageHistogram(char* imageFileName, int* histVectorOut, int numBins)
{
	IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	IplImage *histVector = cvCreateImage(cvSize(1, numBins), IPL_DEPTH_64F, 1);
	cvSetZero(histVector);

	computeHistogram(imageData, histVector, numBins);

	for (int hist_i = 0; hist_i<numBins; hist_i++)
	{
		histVectorOut[hist_i] = (unsigned long)roundf(CV_IMAGE_ELEM(histVector, double, hist_i, 0));
	}

	cvReleaseImage(&imageData);
	cvReleaseImage(&histVector);
}

void computeImageHistogramFromData(short* imageData, int numRows, int numCols, int* histVectorOut, int numBins)
{
	//IplImage *imageData = cvLoadImage(imageFileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	IplImage *ipImageF = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);
	IplImage *histVector = cvCreateImage(cvSize(1, numBins), IPL_DEPTH_64F, 1);
	cvSetZero(histVector);

	cvSetImageData(ipImage, imageData, numCols * sizeof(short));
	cvConvert(ipImage, ipImageF);
	computeHistogram(ipImageF, histVector, numBins);

	for (int hist_i = 0; hist_i<numBins; hist_i++)
	{
		histVectorOut[hist_i] = (unsigned long)roundf(CV_IMAGE_ELEM(histVector, double, hist_i, 0));
	}

	cvReleaseImageHeader(&ipImage);
	cvReleaseImage(&ipImageF);
	cvReleaseImage(&histVector);
}

void printHistogram(CvHistogram *cvHist, int numBins)
{
	FILE *fp = fopen( "D:\\RSM.txt", "a+");
	if (fp != NULL)
	{
		fprintf(fp, " ================================ \n");
		for (int i = 0; i<numBins; i++)
		{
			fprintf(fp, "%f ", cvQueryHistValue_1D(cvHist, i));
		}
		fprintf(fp, " ================================ \n\n");
		fclose(fp);
	}
}

void print2DImage(IplImage *imageData)
{
	FILE *fp2 = fopen( "D:\\RSM2.txt", "a+");
	if (fp2 != NULL)
	{
		fprintf(fp2, " ================================ \n");
		for (int i = 0; i<256; i++)
		{
			for (int j = 0; j<256; j++)
			{
				fprintf(fp2, "%d ", ((short*)imageData->imageData)[i* imageData->widthStep + j]);
			}
			fprintf(fp2, "%\n ");
		}
		fprintf(fp2, " ================================ \n\n");
		fclose(fp2);
	}
}

// 3rd order central moment (about the mean)
//x0 = x - repmat(nanmean(x,dim), tile);
//s2 = nanmean(x0.^2,dim); % this is the biased variance estimator
//m3 = nanmean(x0.^3,dim);
//s = m3 ./ s2.^(1.5);

void computeSkewness(IplImage *dataVector, double pixelMean, int numPix, double *skewness)
{
	double s2 = 0, m3 = 0;

	try
	{
		IplImage *tempVector = cvCreateImage(cvSize(1, numPix), IPL_DEPTH_64F, 1);
		IplImage *sqVector = cvCreateImage(cvSize(1, numPix), IPL_DEPTH_64F, 1);
		IplImage *cubeVector = cvCreateImage(cvSize(1, numPix), IPL_DEPTH_64F, 1);
		CvScalar mean1, mean2, mean3;
		cvSetIdentity(tempVector, cvScalar(pixelMean));

		cvSub(dataVector, tempVector, dataVector, NULL);
		cvPow(dataVector, sqVector, 2.0);
		cvPow(dataVector, cubeVector, 3.0);

		mean2 = cvAvg(sqVector, (const CvArr*) NULL);
		mean3 = cvAvg(cubeVector, (const CvArr*) NULL);

		*skewness = (double)mean3.val[0] / pow((double)mean2.val[0], (double)1.5);

		cvReleaseImage(&tempVector);
		cvReleaseImage(&sqVector);
		cvReleaseImage(&cubeVector);
	}
	catch (std::exception &stdException)
	{
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
		//FILE *fp2 = fopen( "D:\\LOG.txt", "a+");
		//fprintf(fp2, " ================================ \n\n");
		//fprintf(fp2, stdException.what());
		//fclose(fp2);
	}
}

void computeSkewnessFromHisto(IplImage *histVector, double pixelMean, int numBins, double *skewness)
{
	double s2 = 0, m3 = 0;

	try
	{
		int numGL = 4096;
		IplImage *GLVector = cvCreateImage(cvSize(1, numGL), IPL_DEPTH_64F, 1);
		IplImage *tempVector = cvCreateImage(cvSize(1, numGL), IPL_DEPTH_64F, 1);
		IplImage *sqVector = cvCreateImage(cvSize(1, numGL), IPL_DEPTH_64F, 1);
		IplImage *cubeVector = cvCreateImage(cvSize(1, numGL), IPL_DEPTH_64F, 1);
		CvScalar mean1, mean2, mean3;
		cvSetIdentity(tempVector, cvScalar(pixelMean));

		for (int i = 0; i < numGL; i++)
		{
			CV_IMAGE_ELEM(GLVector, double, i, 0) = i;
		}

		CvScalar histSum = cvSum(histVector);
		cvSub(GLVector, tempVector, GLVector, NULL);
		cvMul(GLVector, histVector, GLVector, (double)(1.0 /histSum.val[0]));
		cvPow(GLVector, sqVector, 2.0);
		cvPow(GLVector, cubeVector, 3.0);

		mean2 = cvAvg(sqVector, (const CvArr*)NULL);
		mean3 = cvAvg(cubeVector, (const CvArr*)NULL);

		*skewness = (double)mean3.val[0] / pow((double)mean2.val[0], (double)1.5);

		cvReleaseImage(&GLVector);
		cvReleaseImage(&tempVector);
		cvReleaseImage(&sqVector);
		cvReleaseImage(&cubeVector);
	}
	catch (std::exception &stdException)
	{
		printf("CAlgoSelectorImpl::processJob() -> %s\n", stdException.what());
		//FILE *fp2 = fopen( "D:\\LOG.txt", "a+");
		//fprintf(fp2, " ================================ \n\n");
		//fprintf(fp2, stdException.what());
		//fclose(fp2);
	}
}

//% Center X, compute its fourth and second moments, and compute the uncorrected kurtosis.
//x0 = x - repmat(nanmean(x,dim), tile);
//s2 = nanmean(x0.^2,dim); % this is the biased variance estimator
//m4 = nanmean(x0.^4,dim);
//k = m4 ./ s2.^2;
void computeKurtosis(IplImage *histVector, double pixelMean, int numHistBins, double *kurtosis)
{
	try
	{
		double s2 = 0, m3 = 0;
		IplImage *tempVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
		IplImage *sqVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
		IplImage *pow4Vector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
		CvScalar mean1, mean2, mean4;
		cvSetIdentity(tempVector, cvScalar(pixelMean));

		cvSub(histVector, tempVector, histVector, NULL);
		cvPow(histVector, sqVector, 2.0);
		cvPow(histVector, pow4Vector, 4.0);

		mean2 = cvAvg(sqVector, (const CvArr*)NULL);
		mean4 = cvAvg(pow4Vector, (const CvArr*)NULL);

		*kurtosis = (double)mean4.val[0] / pow((double)mean2.val[0], (double)2.0);

		cvReleaseImage(&tempVector);
		cvReleaseImage(&sqVector);
		cvReleaseImage(&pow4Vector);
	}
	catch (std::exception &stdException)
	{ 

	}
}

void computeKurtosisFromHisto(IplImage *histVector, double pixelMean, int numHistBins, double *kurtosis)
{
	try
	{
		double s2 = 0, m3 = 0;
		int numGL = 4096;
		IplImage *GLVector = cvCreateImage(cvSize(1, numGL), IPL_DEPTH_64F, 1);
		IplImage *tempVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
		IplImage *sqVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
		IplImage *pow4Vector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
		CvScalar mean1, mean2, mean4;
		cvSetIdentity(tempVector, cvScalar(pixelMean));

		for (int i = 0; i < numGL; i++)
		{
			CV_IMAGE_ELEM(GLVector, double, i, 0) = i;
		}

		CvScalar histSum = cvSum(histVector);
		cvSub(GLVector, tempVector, GLVector, NULL);
		cvMul(GLVector, histVector, GLVector, (double)(1.0 / histSum.val[0]));
		cvPow(GLVector, sqVector, 2.0);
		cvPow(GLVector, pow4Vector, 4.0);

		mean2 = cvAvg(sqVector, (const CvArr*)NULL);
		mean4 = cvAvg(pow4Vector, (const CvArr*)NULL);

		*kurtosis = (double)mean4.val[0] / pow((double)mean2.val[0], (double)2.0);

		cvReleaseImage(&GLVector);
		cvReleaseImage(&tempVector);
		cvReleaseImage(&sqVector);
		cvReleaseImage(&pow4Vector);
	}
	catch (std::exception &stdException)
	{

	}
}


void extractStatsFromImageData(short *imageDataIn, int numCols, int numRows, int numHistBins, double* metrics, int* minMax)
{
	IplImage *ipImage = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage, imageDataIn, numCols * sizeof(short));

	double skewness = 0.0;
	double kurtosis = 0.0;

	metrics[0] = 0.0;
	metrics[1] = 0.0;
	metrics[2] = 0.0;
	metrics[3] = 0.0;

	CvMat gray_img_hdr;
	CvMat *imageDataVector1 = cvReshape(ipImage, &gray_img_hdr, 0, ipImage->width * ipImage->height);
	IplImage header1;
	IplImage *imageDataVector = cvGetImage(imageDataVector1, &header1);
	IplImage *imageDataVectorF = cvCreateImage(cvSize(1, ipImage->width * ipImage->height), IPL_DEPTH_64F, 1);
	cvConvert(imageDataVector, imageDataVectorF);

	IplImage *histVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
	cvSetZero(histVector);

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar cvMean;
	CvScalar cvStdDev;
	cvAvgSdv(ipImage, &cvMean, &cvStdDev, NULL);

	double minVal = 0, maxVal = 0;
	CvPoint minLoc, maxLoc;

	cvMinMaxLoc(ipImage, &minVal, &maxVal, &minLoc, &maxLoc, (CvArr*)NULL);
	minMax[0] = roundf((float)minVal);
	minMax[1] = roundf((float)maxVal);

	// ---------------------------------------------------------------
	// Step 1: Compute 1D Histogram
	// ---------------------------------------------------------------
	computeHistogram(ipImage, imageDataVector, ipImage->width * ipImage->height);

	// ---------------------------------------------------------------
	// Step 2: Compute Skewness and Kurtosis
	// ---------------------------------------------------------------
	computeSkewness(imageDataVectorF, cvMean.val[0], ipImage->width * ipImage->height, &skewness);
	computeKurtosis(imageDataVectorF, cvMean.val[0], ipImage->width * ipImage->height, &kurtosis);

	// ---------------------------------------------------------------
	// Step 3: Fill Data-structs
	// ---------------------------------------------------------------
	metrics[0] = cvMean.val[0];
	metrics[1] = cvStdDev.val[0];
	metrics[2] = skewness;
	metrics[3] = kurtosis;

	// ---------------------------------------------------------------
	// Step 4: Release resources
	// ---------------------------------------------------------------
	cvReleaseImage(&ipImage);
	cvReleaseImage(&histVector);
	cvReleaseImage(&imageDataVectorF);

	return;
}

void extractStatsFromImage(char* fileName, int numHistBins, double* metrics, int* minMax)
{
	double skewness = 0.0;
	double kurtosis = 0.0;

	metrics[0] = 0.0;
	metrics[1] = 0.0;
	metrics[2] = 0.0;
	metrics[3] = 0.0;

	IplImage *imageData = cvLoadImage(fileName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	CvMat gray_img_hdr;
	CvMat *imageDataVector1 = cvReshape(imageData, &gray_img_hdr, 0, imageData->width * imageData->height);
	IplImage header1;
	IplImage *imageDataVector = cvGetImage(imageDataVector1, &header1);
	IplImage *imageDataVectorF = cvCreateImage(cvSize(1, imageData->width * imageData->height), IPL_DEPTH_64F, 1);
	cvConvert(imageDataVector, imageDataVectorF);
	
	IplImage *histVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
	cvSetZero(histVector);

	if (DEBUG_PRINT)
	{
		print2DImage(imageData);
	}

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar cvMean;
	CvScalar cvStdDev;
	cvAvgSdv(imageData, &cvMean, &cvStdDev, NULL);

	double minVal = 0, maxVal = 0;
	CvPoint minLoc, maxLoc;

	cvMinMaxLoc(imageData, &minVal, &maxVal, &minLoc, &maxLoc, (CvArr*)NULL);
	minMax[0] = roundf((float)minVal);
	minMax[1] = roundf((float)maxVal);

	// ---------------------------------------------------------------
	// Step 1: Compute 1D Histogram
	// ---------------------------------------------------------------
	computeHistogram(imageData, imageDataVector, imageData->width * imageData->height);

	// ---------------------------------------------------------------
	// Step 2: Compute Skewness and Kurtosis
	// ---------------------------------------------------------------
	computeSkewness(imageDataVectorF, cvMean.val[0], imageData->width * imageData->height, &skewness);
	computeKurtosis(imageDataVectorF, cvMean.val[0], imageData->width * imageData->height, &kurtosis);

	// ---------------------------------------------------------------
	// Step 3: Fill Data-structs
	// ---------------------------------------------------------------
	metrics[0] = cvMean.val[0];
	metrics[1] = cvStdDev.val[0];
	metrics[2] = skewness;
	metrics[3] = kurtosis;

	// ---------------------------------------------------------------
	// Step 4: Release resources
	// ---------------------------------------------------------------
	cvReleaseImage(&imageData);
	cvReleaseImage(&histVector);
	cvReleaseImage(&imageDataVectorF);

	return;
}

void extractStatsFromHistogram(unsigned int *histData, int numHistBins, double* metrics)
{
	double skewness = 0.0;
	double kurtosis = 0.0;

	metrics[0] = 0.0;
	metrics[1] = 0.0;
	metrics[2] = 0.0;
	metrics[3] = 0.0;

	IplImage *histVector = cvCreateImage(cvSize(1, numHistBins), IPL_DEPTH_64F, 1);
	cvSetZero(histVector);

	for (int hist_i = 0; hist_i<numHistBins; hist_i++)
	{
		CV_IMAGE_ELEM(histVector, double, hist_i, 0) = (double)histData[hist_i];
	}

	// ---------------------------------------------------------------
	// Step 0: Compute Mean/Std-Dev
	// ---------------------------------------------------------------
	CvScalar cvMean;
	CvScalar cvStdDev;
	double sumHist = 0.0, sumHistSq = 0.0, sumHistBins = 0.0;
	for (int hist_i = 0; hist_i<numHistBins; hist_i++)
	{
		sumHistBins += histData[hist_i];
		sumHist += ((double)histData[hist_i] * (double)hist_i);
		//sumHistSq += ((double)histData[hist_i] * (double)histData[hist_i] * (double)hist_i);
	}
	sumHist /= sumHistBins;
	cvMean.val[0] = sumHist;
	
	sumHistSq = 0.0;
	for (int hist_i = 0; hist_i<numHistBins; hist_i++)
	{
		sumHistSq += ((double)(hist_i - sumHist) * (double)(hist_i - sumHist) * (double)histData[hist_i]);
	}
	sumHistSq /= sumHistBins;
	cvStdDev.val[0] = sqrt((double)sumHistSq);

	// ---------------------------------------------------------------
	// Step 1: Compute Skewness and Kurtosis
	// ---------------------------------------------------------------
	computeSkewness(histVector, cvMean.val[0], numHistBins, &skewness);
	computeKurtosis(histVector, cvMean.val[0], numHistBins, &kurtosis);

	// ---------------------------------------------------------------
	// Step 2: Fill Data-structs
	// ---------------------------------------------------------------
	metrics[0] = cvMean.val[0];
	metrics[1] = cvStdDev.val[0];
	metrics[2] = skewness;
	metrics[3] = kurtosis;

	// ---------------------------------------------------------------
	// Step 3: Release resources
	// ---------------------------------------------------------------
	cvReleaseImage(&histVector);

	return;
}

void constructTallMatrixFromImageData(IplImage* imageData, IplImage* outTallMatrix, short kernelSize)
{
	int numHorizBlocks = (imageData->width - kernelSize) + 1;
	int numVerticalBlocks = (imageData->height - kernelSize) + 1;;
	int totalNumRowsForTallMatrix = numHorizBlocks * numVerticalBlocks;
	int totalNumColsForTallMatrix = kernelSize * kernelSize;

	for (unsigned int idx1 = 0; idx1 < numVerticalBlocks; idx1++)
	{
		for (unsigned int idx2 = 0; idx2 < numHorizBlocks; idx2++)
		{
			for (unsigned int idx3 = 0; idx3 < kernelSize; idx3++) // Across the vertical direction
			{
				for (unsigned int idx4 = 0; idx4 < kernelSize; idx4++) // Across the horizontal direction
				{
					int rowIdxForTallMatrix = (idx1 * numHorizBlocks) + idx2;
					int colIdxForTallMatrix = (idx3 * kernelSize) + idx4;
					int rowNum = idx1 + idx3;
					int colNum = idx2 + idx4;
					CV_IMAGE_ELEM(outTallMatrix, float, idx1, colIdxForTallMatrix) = (float)CV_IMAGE_ELEM(imageData, short, rowNum, colNum);
				}
			}
		}
	}
}

void reConstructImageDataFromBMatrix(IplImage* BMatrix, IplImage *outputImageData, short kernelSize)
{
	int rowIdx = 0;
	for (unsigned int idx1 = (kernelSize / 2); idx1 < outputImageData->height - (kernelSize / 2); idx1++)
	{
		for (unsigned int idx2 = (kernelSize / 2); idx2 < outputImageData->width - (kernelSize / 2); idx2++)
		{
			CV_IMAGE_ELEM(outputImageData, short, idx1, idx2) = CV_IMAGE_ELEM(BMatrix, float, rowIdx, 0);
			rowIdx++;
		}
	}
}

void reConstructImageDataFromTallMatrix(IplImage* outTallMatrix, IplImage* imageData, short kernelSize)
{
	int numHorizBlocks = (imageData->width - kernelSize) + 1;
	int numVerticalBlocks = (imageData->height - kernelSize) + 1;;
	int totalNumRowsForTallMatrix = numHorizBlocks * numVerticalBlocks;
	int totalNumColsForTallMatrix = kernelSize * kernelSize;

	for (unsigned int idx1 = (kernelSize / 2); idx1 < (imageData->height - (kernelSize / 2)); idx1++)
	{
		for (unsigned int idx2 = (kernelSize / 2); idx2 < (imageData->width - (kernelSize / 2)); idx2++)
		{
			for (unsigned int idx3 = 0; idx3 < kernelSize; idx3++) // Across the vertical direction
			{
				for (unsigned int idx4 = 0; idx4 < kernelSize; idx4++) // Across the horizontal direction
				{
					int rowIdxForTallMatrix = (idx1 * numHorizBlocks) + idx2;
					int colIdxForTallMatrix = (idx3 * kernelSize) + idx4;
					int rowNum = idx1 + idx3;
					int colNum = idx2 + idx4;
					CV_IMAGE_ELEM(imageData, float, rowNum, colNum) = CV_IMAGE_ELEM(outTallMatrix, float, idx1, colIdxForTallMatrix);
				}
			}
		}
	}
}

// Computes (inv[A^ * A] * A^)
void computePseudoInverseOfTallMatrix(IplImage* tallMatrix, IplImage* outMatrix)
{
	IplImage *A_T = cvCreateImage(cvSize(tallMatrix->height, tallMatrix->width), IPL_DEPTH_32F, 1);

	IplImage *A_T_times_A = cvCreateImage(cvSize(tallMatrix->width, tallMatrix->width), IPL_DEPTH_32F, 1);
	IplImage *A_T_times_A_Inv = cvCreateImage(cvSize(tallMatrix->width, tallMatrix->width), IPL_DEPTH_32F, 1);
	IplImage *image2 = cvCreateImage(cvSize(tallMatrix->height, tallMatrix->width), IPL_DEPTH_32F, 1);
	IplImage *image3 = cvCreateImage(cvSize(tallMatrix->width, tallMatrix->width), IPL_DEPTH_32F, 1);
	
	cvSetZero(A_T_times_A);
	cvSetZero(A_T_times_A_Inv);
	cvSetZero(image2);

	// cvMulTransposed(const CvArr* src, CvArr* dst, int order, const CvArr* delta=NULL, double scale=1. )

	cvTranspose(tallMatrix, A_T);
	cvGEMM(A_T, tallMatrix, 1.0, NULL, 0.0, A_T_times_A);
	
	//cvMulTransposed(tallMatrix, image1, 1);
	cvInv(A_T_times_A, A_T_times_A_Inv);

	//cvTranspose(tallMatrix, image2);
	//cvTranspose(A_T_times_A, image3);
	cvGEMM(A_T_times_A_Inv, A_T, 1.0, NULL, 0.0, outMatrix);
}

// OutMatrix = (Nx1)
// where N = (M-k)*(M-k)
// M = dimension of input matrix, k = kernelSize
void constructBMatrix(IplImage* inputMatrix, IplImage* outMatrix, short kernelSize)
{
	int rowIdx = 0;
	for (unsigned int idx1 = (kernelSize / 2); idx1 < inputMatrix->height - (kernelSize / 2); idx1++)
	{
		for (unsigned int idx2 = (kernelSize / 2); idx2 < inputMatrix->width - (kernelSize / 2); idx2++)
		{
			CV_IMAGE_ELEM(outMatrix, float, rowIdx, 0) = (float)CV_IMAGE_ELEM(inputMatrix, short, idx1, idx2);
			rowIdx++;
		}
	}
}

void reconstructImageFromBMatrix()
{

}

void dynamicallyCompensateImageData(short* refImageData, int numCols, int numRows,
									short* currentImageData, short kernelSize,
									short* outImageData)
{
	IplImage *ipImage1 = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage1, refImageData, numCols * sizeof(short));

	IplImage *ipImage2 = cvCreateImageHeader(cvSize(numCols, numRows), IPL_DEPTH_16S, 1);
	cvSetImageData(ipImage2, currentImageData, numCols * sizeof(short));

	IplImage *transformedCurrentImage = cvCreateImage(cvSize(numCols, numRows), IPL_DEPTH_32F, 1);

	try
	{
		cvSetZero(transformedCurrentImage);

		int numHorizBlocks = (ipImage1->width - kernelSize) + 1;
		int numVerticalBlocks = (ipImage1->height - kernelSize) + 1;;
		int totalNumRowsForTallMatrix = numHorizBlocks * numVerticalBlocks;
		int totalNumColsForTallMatrix = kernelSize * kernelSize;
		IplImage *tallMatrix = cvCreateImage(cvSize(totalNumColsForTallMatrix, totalNumRowsForTallMatrix), IPL_DEPTH_32F, 1); // Nx9
		IplImage *tallMatrixForTransformedInput = cvCreateImage(cvSize(1, totalNumRowsForTallMatrix), IPL_DEPTH_32F, 1); // Nx1
		IplImage *invOfTallMatrix = cvCreateImage(cvSize(totalNumRowsForTallMatrix, totalNumColsForTallMatrix), IPL_DEPTH_32F, 1); // 9xN
		IplImage *BMatrix = cvCreateImage(cvSize(1, totalNumRowsForTallMatrix), IPL_DEPTH_32F, 1); // Nx1
		IplImage *lsEstimateMatrix = cvCreateImage(cvSize(1, totalNumColsForTallMatrix), IPL_DEPTH_32F, 1); // 9x9
		IplImage *lsEstimateMatrix_T = cvCreateImage(cvSize(totalNumColsForTallMatrix, 1), IPL_DEPTH_32F, 1); // 9x9

		constructTallMatrixFromImageData(ipImage2, tallMatrix, kernelSize);
		constructBMatrix(ipImage1, BMatrix, kernelSize);
		//computePseudoInverseOfTallMatrix(tallMatrix, invOfTallMatrix);
		cvInvert(tallMatrix, invOfTallMatrix, DECOMP_SVD);

		// GT_CONT
		cvGEMM(invOfTallMatrix, BMatrix, 1.0f, NULL, 0.0f, lsEstimateMatrix);
		cvTranspose(lsEstimateMatrix, lsEstimateMatrix_T);
		cvGEMM(tallMatrix, lsEstimateMatrix, 1.0f, NULL, 0.0f, tallMatrixForTransformedInput);
		
		reConstructImageDataFromBMatrix(tallMatrixForTransformedInput, transformedCurrentImage, kernelSize);

		for (unsigned int idx1 = 0; idx1 < numRows; idx1++)
		{
			for (unsigned int idx2 = 0; idx2 < numCols; idx2++)
			{
				long vectorIdx = (idx1 * numCols) + idx2;
				outImageData[vectorIdx] = cvRound(CV_IMAGE_ELEM(transformedCurrentImage, float, idx1, idx2));
			}
		}

		cvReleaseImageHeader(&ipImage1);
		cvReleaseImageHeader(&ipImage2);

		cvReleaseImage(&transformedCurrentImage);
		cvReleaseImage(&tallMatrix);
		cvReleaseImage(&tallMatrixForTransformedInput);
		cvReleaseImage(&invOfTallMatrix);
		cvReleaseImage(&BMatrix);
		cvReleaseImage(&lsEstimateMatrix);
	}
	catch (std::exception &stdException)
	{
		printf("Trying to perform dynamicallyCompensateImageData with numColsTemplate,numRowsTemplate  = (%d, %d)\n", numCols, numRows);
		printf("dynamicallyCompensateImageData() -> %s\n", stdException.what());
	}

	return;
}


bool InitializeAlgoPlugin()
{
	bool m_bLoaded = false;

	if (hLeafModule == NULL)
	{
		TCHAR x[100] = TEXT("LeafStandAloneIFC.dll");
		hLeafModule = LoadLibrary(x);
		if (hLeafModule == NULL)
		{
			std::string errorMsg = "Could not load LeafStandAloneIFC.dll";
			throw std::exception("Error while calling matchtemplateLeaf"); // errorMsg, __FUNCTION__, __FILE__, __LINE__);
		}
		
		m_bLoaded = true;
	}

	return m_bLoaded;
}

void zeroPadImageData(	short *smallerImage, 
						int numCols, int numRows,
						short* outputZeroPaddedImageData, int numColsWithPadding, int numRowsWithPadding)
{
	int leftLine = (numColsWithPadding / 2) - (numCols / 2);
	int rightLine = (numColsWithPadding / 2) + (numCols / 2) - 1;

	if ((numColsWithPadding % 2) == 0)
	{
		//rightLine -= 1;
	}

	int topLine = (numRowsWithPadding / 2) - (numRows / 2);
	int bottomLine = (numRowsWithPadding / 2) + (numRows / 2) - 1;

	if ((numColsWithPadding % 2) == 0)
	{
		//bottomLine -= 1;
	}

	for (int i = 0; i < numRowsWithPadding; i++)
	{
		for (int j = 0; j < numColsWithPadding; j++)
		{
			long vecIdxInInputImage = (i * numColsWithPadding) + j;
			outputZeroPaddedImageData[vecIdxInInputImage] = 0;
		}
	}

	for (int i = topLine; i <= bottomLine; i++)
	{
		for (int j = leftLine; j <= rightLine; j++)
		{
			long vecIdxInTemplate = ((i - topLine) * numCols) + (j - leftLine);
			long vecIdxInOutput = (i*numColsWithPadding) + j;
			outputZeroPaddedImageData[vecIdxInOutput] = smallerImage[vecIdxInTemplate];
		}
	}
}

void getSubImageData(short *origImage, int numCols, int numRows, 
					 short* choppedInputData, int numColsTemplate, int numRowsTemplate)
{
	int leftLine = (numCols / 2) - (numColsTemplate / 2);
	int rightLine = (numCols / 2) + (numColsTemplate / 2) - 1;
	
	if ((numColsTemplate % 2) == 0)
	{
		rightLine -= 1;
	}

	int topLine = (numRows / 2) - (numRowsTemplate / 2);
	int bottomLine = (numRows / 2) + (numRowsTemplate / 2) - 1;

	if ((numColsTemplate % 2) == 0)
	{
		bottomLine -= 1;
	}

	for (int i = topLine; i <= bottomLine; i++)
	{
		for (int j = leftLine; j <= rightLine; j++)
		{
			long vecIdxInInputImage = (i * numCols) + j;
			long vecIdxInTemplate = ((i-topLine)*numColsTemplate) + (j - leftLine);
			choppedInputData[vecIdxInTemplate] = origImage[vecIdxInInputImage];
		}
	}
}
*/