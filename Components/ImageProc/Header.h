#pragma once
 class __declspec(dllexport) ImageProcessingOps
{
public:
	ImageProcessingOps()
	{
	}
	int addTwoNumbers(int a, int b)
	{
		return a + b;		
	}

	int addTwoMats();

	void matchImages(unsigned short *origImage, int numCols, int numRows, unsigned short *templateImage, int numColsTemplate, int numRowsTemplate, double *corrImage);

};