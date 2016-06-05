// ImageProcWrapper.h

#pragma once

//#include "../ImageProc/ImageProcInterface.h"
#include "../ImageProc/Header.h"
#include "../ImageProc/ImageProcessUtility.h"
using namespace System;

namespace ImageProcWrapper {

	public ref class ImageUtils
	{
		ImageProcessingUtility *utilty = nullptr;

	public:
		ImageUtils()
		{
			utilty = new ImageProcessingUtility();

		}

		ImageProcessingUtility* getUtility()
		{
			if(utilty == nullptr)
				utilty = new ImageProcessingUtility();

			return utilty;
		}

		int method(int a, int b)
		{
			ImageProcessingOps t;
			return t.addTwoNumbers(a, b);
		}
		int test(int a, int b)
		{
			return a - b;
		}
		
		bool threshodImage(unsigned short *origImage, int imWidht, int imHeight, int threshold,  unsigned short *thresholdedImage)
		{ 
			try
			{
				getUtility()->threshodImage(origImage, imWidht, imHeight, threshold,  thresholdedImage);
			}
			catch (...)
			{
				return false;
			}

			return true;

		}

			
		void matchImages(unsigned short origImage[], int numCols, int numRows, unsigned short templateImage[], int numColsTemplate, int numRowsTemplate, double corrImage[])
		{
			ImageProcessingOps t;

			t.matchImages(origImage, numCols, numRows, templateImage, numColsTemplate, numRowsTemplate, corrImage);
		}		
	};

}
