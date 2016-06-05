using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DicomImageLibrary
{
    class Mat
    {
        ushort [,] data;
        public int width
        {
            get { return width; }
            set { width = value; }
        }
        public int height
        {
            get { return height; }
            set { height = value; }
        }
        Mat(int w, int h)
        {
            data = new ushort[w, h];
            this.width = w;
            this.height = h;
        }

        public void put(int x, int y, ushort val)
        {
            data[x, y] = val;
        }

        public ushort get(int x, int y)
        {
            return data[x, y];
        }

        public void copyTo(Mat inMat)
        {
            if(inMat.width != this.width || inMat.height != this.height)
            {
                throw new Exception("Can not copy Mat as the image dimentions are not correct");
            }
            for (int x = 0; x < width; x++)
                for (int y = 0; y < height; y++)
                     inMat.put(x, y, data[x, y]);
        }

        public Mat clone()
        {
            Mat newMat = new Mat(width, height);
            copyTo(newMat);
            return newMat;
        }


    }
}
