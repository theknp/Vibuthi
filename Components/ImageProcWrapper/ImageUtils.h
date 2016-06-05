// ImageProcWrapper.h

#pragma once

//#include "../ImageProc/ImageProcInterface.h"
#include "../ImageProc/Header.h"
using namespace System;

namespace ImageProcWrapper {

	public ref class ImageUtils
	{
	public:
		ImageUtils()
		{

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
		
		void matchImages(unsigned short origImage[], int numCols, int numRows, unsigned short templateImage[], int numColsTemplate, int numRowsTemplate, double corrImage[])
		{
			ImageProcessingOps t;

			t.matchImages(origImage, numCols, numRows, templateImage, numColsTemplate, numRowsTemplate, corrImage);
		}		
	};

}
