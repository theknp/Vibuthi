using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace DicomImageLibrary
{
    public class FloodFill
    {
        private int maxSize;
        private int[] stackArray;
        private int top;

        Mat finalImage;
        Mat inputMat;
        int option = 0;
        int neighborRange = 1;
        int nColors = 15;

        int[] regionSize = new int[1000];
        int regionsCount = 0;
        int validRegionsAfterThreshold = 0;
        //ArrayList<double[]> colorList = new ArrayList<double[]>();

        ArrayList colorList = new ArrayList();

        
        public void initStack(int s)
        {
            maxSize = s;
            stackArray = new int[maxSize];
            top = -1;
        }
        public void push(int j)
        {
            stackArray[++top] = j;
        }
        public int pop()
        {
            if (top < 0)
                return -1;
            return stackArray[top--];
        }

        public FloodFill(Mat inputMat)
        {
            this.inputMat = inputMat;
            populateColors();
            initStack(50000);
        }

        void populateColors()
        {
            
            colorList.Add(new double[] { 0, 0, 0 });
            colorList.Add(new double[] { 255, 0, 0 });
            colorList.Add(new double[] { 0, 255, 0 });
            colorList.Add(new double[] { 0, 0, 255 });
            colorList.Add(new double[] { 255, 255, 0 });
            colorList.Add(new double[] { 0, 255, 255 });
            colorList.Add(new double[] { 255, 0, 255 });
            colorList.Add(new double[] { 192, 192, 192 });
            colorList.Add(new double[] { 128, 128, 128 });
            colorList.Add(new double[] { 128, 0, 0 });
            colorList.Add(new double[] { 128, 128, 0 });
            colorList.Add(new double[] { 0, 128, 0 });
            colorList.Add(new double[] { 128, 0, 128 });
            colorList.Add(new double[] { 0, 128, 128 });
            colorList.Add(new double[] { 0, 0, 128 });
            colorList.Add(new double[] { 244, 164, 96 });
            colorList.Add(new double[] { 240, 255, 240 });
            colorList.Add(new double[] { 255, 240, 245 });
            colorList.Add(new double[] { 75, 0, 130 });
            colorList.Add(new double[] { 255, 255, 255 });
        }

        void floodFillAt(int x, int y, ushort label)
        {
            //finalImage.at<uchar>(y, x) = label;

            finalImage.put(y, x, (ushort)label);

            for (int ky = -neighborRange; ky <= neighborRange; ky++)
            {
                for (int kx = -neighborRange; kx <= neighborRange; kx++)
                {
                    int ix = kx + x;
                    if (ix < 0)
                        ix = 0;
                    if (ix >= finalImage.Width)
                        ix = finalImage.Width - 1;

                    int iy = ky + y;
                    if (iy < 0)
                        iy = 0;
                    if (iy >= finalImage.Height)
                        iy = finalImage.Height - 1;
                    ushort d = finalImage.get(iy, ix);
                    if (d == 255)
                    {
                        finalImage.put(iy, ix, label);
                        push(iy);
                        push(ix);
                    }

                }
            }
            int xl = pop();
            int yl = pop();
            while ((xl != -1) && (yl != -1))
            {

                floodFillAt(xl, yl, label);
                xl = pop();
                yl = pop();
            }

        }

        private void thresholdImputImage(int threshold)
        {
            for(int x = 0; x < inputMat.Width; x++)
            {
                for(int y = 0; y < inputMat.Height; y++)
                {
                    if (inputMat.get(x, y) < threshold)
                        inputMat.put(x, y, 0);
                    else
                        inputMat.put(x, y, 255);
                }
            }
        }

        public Mat floodFill(ushort  intenThreshold, int minbSize)
        {
            
            ushort labele = 0;
            regionsCount = 0;
            thresholdImputImage(intenThreshold);


            finalImage = inputMat.clone();
            inputMat.copyTo(finalImage);
		
		    for (int y = 0; y< inputMat.Height; y++)
		    {
			    for (int x = 0; x< inputMat.Width; x++)
			    {
				    ushort d = finalImage.get(y, x);
				    if (d == 255)
				    {
					    labele ++;
					    regionsCount++;

                        floodFillAt(x, y, labele);
                    }
                }
		    }


            measureReasionSizes();

            thresholdRegions(minbSize);
            //Mat colorImg = constructColorImage();

            //return colorImg;

            return finalImage;
        }

        private void measureReasionSizes()
        {
            for (int y = 0; y < finalImage.Height; y++)
            {
                for (int x = 0; x < finalImage.Width; x++)
                {
                    ushort d = finalImage.get(y, x);
                    if (d != 0)
                        regionSize[(int)d]++;
                }
            }
        }

        void thresholdRegions(int threashold)
        {
            for (int y = 0; y < finalImage.Height; y++)
            {
                for (int x = 0; x < finalImage.Width; x++)
                {
                    ushort d = finalImage.get(y, x);
                    if (regionSize[(int)d] <= threashold)
                    {
                        finalImage.put(y, x, 0);
                    }
                }
            }

            validRegionsAfterThreshold = 0;
            for (int i = 0; i < regionSize.Length; i++)
            {
                if (regionSize[i] > threashold)
                    validRegionsAfterThreshold++;
            }
        }


    }

    
}
