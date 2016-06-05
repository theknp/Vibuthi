#pragma once
class __declspec(dllexport) ImageProcessingUtility
{
public:
	ImageProcessingUtility()
	{
	}

	void threshodImage(unsigned short *origImage, int numCols, int numRows, int threshold, unsigned short *thresholdedImage);

	

	//void matchImages(unsigned short *origImage, int numCols, int numRows, unsigned short *templateImage, int numColsTemplate, int numRowsTemplate, double *corrImage);

};