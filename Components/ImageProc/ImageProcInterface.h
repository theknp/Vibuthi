static HMODULE hLeafModule = NULL;

extern "C"
{

	//__declspec(dllexport) void matchImages(unsigned short *origImage, int numCols, int numRows, unsigned short *templateImage, int numColsTemplate, int numRowsTemplate, double *corrImage);


	//__declspec(dllexport) void extractStatsFromImage(char* fileName, int numHistBins, double* metrics, int* minMax);
	//__declspec(dllexport) void extractStatsFromImageData(short *imageDataIn, int numCols, int numRows, int numHistBins, double* metrics, int* minMax);

	//__declspec(dllexport) void extractStatsFromHistogram(unsigned int *histData, int numHistBins, double* metrics);
	//__declspec(dllexport) int loadImageData(char* imageFileName, unsigned short* imageData, int numRows, int numCols);
	//__declspec(dllexport) int loadImageDataShort(char* imageFileName, short* imageDataArray, int numRows, int numCols);
	//__declspec(dllexport) void computeImageHistogram(char* imageFileName, int* imageData, int numBins);
	//__declspec(dllexport) void computeImageHistogramFromData(short* imageData, int numRows, int numCols, int* histVectorOut, int numBins);
	//__declspec(dllexport) void extractFocusMetricsFromImage(char* fileName, double* metrics);

	//__declspec(dllexport) int resizeImage(char *imageFileName, float conversionQuotient, short *imageDataOut, int numColsOut, int numRowsOut);
	//__declspec(dllexport) void resizeImageFromData(short *inputImage, int numRowsIn, int numColsIn, float UPSAMPLE_MAG, short *imageDataOut, int numColsOut, int numRowsOut);

	//__declspec(dllexport) void applyLPF(short *imageDataIn, short *imageDataOut, short numRows, short numCols, short kernelSize);
	//__declspec(dllexport) void applyFilter(short *imageDataIn, short *imageDataOut, short numRows, short numCols, short* kernel, short kernelSize);
	//__declspec(dllexport) void sharpenImage(short *imageDataIn, short *imageDataOut, int currentSemWidth, int currentSemHeight, short kernelSize);
	//__declspec(dllexport) void matchTemplate(short *origImage, int numCols, int numRows, short *templateImage, int numColsTemplate, int numRowsTemplate, double *corrImage);


	//__declspec(dllexport) void computeStdDevFromImageData(short* imageDataIn, int numCols, int numRows, double* stdDev);
	//__declspec(dllexport) void computeStdDevFromImageDataWithBorder(short* imageDataIn, int numCols, int numRows, short borderWidth, double* stdDev);
	//__declspec(dllexport) void computeMeanFromImageData(short* imageDataIn, int numCols, int numRows, double* mean);

	//__declspec(dllexport) void scaleAndAddImageData(short* imageDataIn, int numCols, int numRows, double scale, short constToSubtr, short constToAdd, short* imageDataOut);


	//__declspec(dllexport) void matchTemplateV2(short *origImage, int numCols, int numRows,
	//	short *templateImage, int numColsTemplate, int numRowsTemplate, 
	//	double *corrImage,
	//	short *alignedImage);

	//__declspec(dllexport) void extractFocusMetricsFromImageData(short* imageData, int numCols, int numRows, double* metrics);

	//__declspec(dllexport) int matchTemplateWithUpsampledCorr(	short *origImage, int numCols, int numRows,
	//	short *templateImage, int numColsTemplate, int numRowsTemplate,
	//	short *outImage, // Same size as template Image
	//	double CORR_UPSAMPLE_MAG,
	//	double *corrImage); // Pre-allocated array of size [(numCols - numColsTemplate + 1) * CORR_UPSAMPLE_MAG] x [(numRows - numRowsTemplate + 1) * CORR_UPSAMPLE_MAG]

	//__declspec(dllexport) int matchTemplateWithUpsampledCorr2(	short *origImage, int numCols, int numRows,
	//	short *templateImage, int numColsTemplate, int numRowsTemplate,
	//	short *outImage, // Same size as template Image
	//	double CORR_UPSAMPLE_MAG,
	//	double *corrImage); // Pre-allocated array of size [(numCols - numColsTemplate + 1) * CORR_UPSAMPLE_MAG] x [(numRows - numRowsTemplate + 1) * CORR_UPSAMPLE_MAG]

	//__declspec(dllexport) void subtractVectors(short* imageData1, short* imageData2, int numCols, int numRows, short* imageDataOut);

	//__declspec(dllexport) void histoEqualizeImageData(short* inImageData, int numCols, int numRows, short* outImage);

	//__declspec(dllexport) void dynamicallyCompensateImageData(	short* refImage, int numCols, int numRows,
	//	short* currentImage, short kernelSize,
	//	short* outImage);

	//__declspec(dllexport) void rotateImageData(short *inputImageData, int numCols, int numRows, double angle, short *outputImageData);

}