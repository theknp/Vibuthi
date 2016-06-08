using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DicomImageLibrary
{
    public enum ImageBitsPerPixel { Eight, Sixteen, TwentyFour };
    class LookupTable
    {
        private byte[] lookupTbl = new byte[256];

        public LookupTable(int minimum, int maximum)
        {
            ComputeLookupTable(minimum, maximum);

        }
        public void ComputeLookupTable(int minimum, int maximum)
        {
            

            if (maximum == 0)
                maximum = 255;

            int range = maximum - minimum;
            if (range < 1) range = 1;
            double factor = 255.0 / range;

            for (int i = 0; i < 256; ++i)
            {
                if (i <= minimum)
                    lookupTbl[i] = 0;
                else if (i >= maximum)
                    lookupTbl[i] = 255;
                else
                {
                    lookupTbl[i] = (byte)((i - minimum) * factor);
                }
            }
        }

        public byte this[int key]
        {
            get
            {

                return lookupTbl[key];
            }
            
        }
        
    }
}
