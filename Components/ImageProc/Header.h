#pragma once
 class __declspec(dllexport) Testing
{
public:
	 Testing()
	{
	}
	int addTwoNumbers(int a, int b)
	{
		//return a + b;

		return addTwoMats();
	}

	int addTwoMats();

	void matchImages(unsigned short *origImage, int numCols, int numRows, unsigned short *templateImage, int numColsTemplate, int numRowsTemplate, float &xShift, float &yShift);

};