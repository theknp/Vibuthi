#pragma once

#include "stdafx.h"
#include "math.h"
#include "opencv\cv.hpp"

using namespace cv;

class __declspec(dllexport) FloodFill
{
	
public:
	FloodFill();
	
	void initStack(int s);
	void push(int j);
	int pop();
private:

	int maxSize;
	int* stackArray;
	int top;

	Mat finalImage;
	Mat thresholdImg;
	int option = 0;
	int neighborRange = 1;
	int nColors = 15;

	int* regionSize = new int[1000];
	int regionsCount = 0;
	int validRegionsAfterThreshold = 0;
	
	int regionThreshold;
	void floodFillAt(int x, int y, int label);

	//void matchImages(unsigned short *origImage, int numCols, int numRows, unsigned short *templateImage, int numColsTemplate, int numRowsTemplate, double *corrImage);

};