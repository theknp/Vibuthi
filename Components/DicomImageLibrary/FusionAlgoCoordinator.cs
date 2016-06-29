using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ImageProcWrapper;

using System.Runtime.InteropServices;

namespace DicomImageLibrary
{
    public class FusionAlgoCoordinator
    {

        public List<AlignInfo> result;
        public int PerformFusion(ref List<Scan> Scans)
        {
            if(Scans.Count == 2)
            {
                Scan Scan1 = Scans[0];
                Scan Scan2 = Scans[1];
                result = new List<AlignInfo>();
                
                int startIndex = 0;
                int endIndex = 2;

                for (int Sc1Index = 0; Sc1Index < Scan1.Images.Count; Sc1Index++)
                {
                    DicomReader dr = Scan1.Images[Sc1Index];

                    if (result.Count > 0)
                    {
                        startIndex = result[result.Count - 1].matchingImageIndex - 2;
                        if (startIndex < 0)
                            startIndex = 0;
                        endIndex = startIndex + 4;

                        if (endIndex >= Scan2.Images.Count)
                            endIndex = Scan2.Images.Count - 1;
                    }

                    AlignInfo aI = findMatch(Sc1Index, ref dr, ref Scan2, startIndex, endIndex);
                    result.Add(aI);
                }
                

            }
            return 1;
        }
        public AlignInfo findMatch(int scanIndex, ref DicomReader dr, ref Scan scan, int startIndex, int endIndex)
        { 

            List<ushort> localpixels16_ref = new List<ushort>();
            dr.GetPixels16(ref localpixels16_ref);
            int width = dr.width;
            int height = dr.height;

            double maxScore = 0;
            int xShiftFound = 0;
            int yShiftFound = 0;
            int matchedIndex = 0;

            //int scan2Index = 0;
            //foreach (DicomReader drTemp in scan.Images)
            for(int scan2Index = startIndex; scan2Index < endIndex; scan2Index++)
            {
                DicomReader drTemp = scan.Images[scan2Index];
                List<ushort> localpixels16_test = new List<ushort>();
                drTemp.GetPixels16(ref localpixels16_test);


                ImageUtils util = new ImageUtils();
                List<double> corrImage = new List<double>();

                IntPtr refImg = Marshal.AllocHGlobal(2 * localpixels16_ref.Count);
                short [] refarray = new short[localpixels16_ref.Count];

                ushort[] refarayushort = localpixels16_ref.ToArray();
                for (int i = 0; i < refarayushort.Length; i++)
                    refarray[i] = (short)refarayushort[i];

                Marshal.Copy(refarray, 0, refImg, localpixels16_ref.Count);


                IntPtr testImg = Marshal.AllocHGlobal(2 * localpixels16_test.Count);
                short[] testarray = new short[localpixels16_test.Count];

                ushort[] testarrayushort = localpixels16_test.ToArray();
                for ( int i = 0; i< testarrayushort.Length; i++)
                    testarray[i] = (short)testarrayushort[i];

                Marshal.Copy(testarray, 0, testImg, localpixels16_test.Count);

                int xShift = 0, yShift = 0;
                double score = 0;
                util.matchImages1(refImg, width, height, testImg, width, height, ref xShift, ref yShift, ref score);

                if(score > maxScore)
                {

                    xShiftFound = xShift;
                    yShiftFound = yShift;
                    maxScore = score;
                    matchedIndex = scan2Index;
                    if (maxScore > 0.99)
                    {
                        break;
                    }
                }             
            }
            AlignInfo aI = new AlignInfo();


            //aI.matchingImageIndex = scanIndex - (int)(scanIndex /5 + 0.5);
            //aI.shiftX = 0;
            //aI.shiftY = 0;
            aI.parentImageIndex = scanIndex;
            aI.score = maxScore;
            aI.shiftX = xShiftFound;
            aI.shiftY = yShiftFound;
            aI.matchingImageIndex = matchedIndex;

            return aI;
        }

        public AlignInfo GetAlginData(int sourceimgIndex, Boolean IsRef)
        {            
            if(IsRef)
            {
                foreach( AlignInfo r in result)
                {
                    if(r.parentImageIndex == sourceimgIndex)
                    {
                        return r;
                    }
                }
            }
            else
            {
                foreach (AlignInfo r in result)
                {
                    if (r.matchingImageIndex == sourceimgIndex)
                    {
                        return r;
                    }
                }
            }
            return null;
        }
    }
}
