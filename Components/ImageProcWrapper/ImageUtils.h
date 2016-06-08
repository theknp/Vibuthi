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
		static void matchImages2(array <unsigned short>^ origImage, int numCols, int numRows, array<unsigned short>^ templateImage, int numColsTemplate, int numRowsTemplate, float %xshift, float %yshift)
		{
			unsigned short * origImagePtr;
			pin_ptr<unsigned short> origImagePinPtr = &origImage[0];
			origImagePtr = origImagePinPtr;

			unsigned short *templateImagePtr;
			pin_ptr<unsigned short> templateImagePinPtr = &templateImage[0];
			templateImagePtr = templateImagePinPtr;
			float shiftx, shifty;
			Testing t;
			t.matchImages(origImagePtr, numCols, numRows, templateImagePtr, numColsTemplate, numRowsTemplate, shiftx, shifty);

			xshift = shiftx;
			yshift = shifty;



		}

	private:
		static unsigned short* getPointer(array<unsigned short>^ data)
		{
			unsigned short *dataptr;
			pin_ptr<unsigned short> p = &data[0];
			dataptr = p;
			return dataptr;
		}
		
	};
	
	

}
