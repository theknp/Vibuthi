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
			Testing t;
			return t.addTwoNumbers(a, b);
		}
		/*static void matchImages2(unsigned short[] origImage, int numCols, int numRows, unsigned short[] templateImage, int numColsTemplate, int numRowsTemplate, double[] corrImage)
		{

		}*/
		
	};
	
	

}
