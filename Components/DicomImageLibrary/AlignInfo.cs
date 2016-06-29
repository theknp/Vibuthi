using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DicomImageLibrary
{
    public class AlignInfo
    {
        public int parentImageIndex;
        public int matchingImageIndex;
        public int shiftX;
        public int shiftY;
        public double score;
    }
}
