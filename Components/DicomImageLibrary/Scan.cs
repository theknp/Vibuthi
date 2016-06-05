using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DicomImageLibrary
{
    public class Scan
    {
        ScanInfo info;
        List<DicomReader> dicomImages = new List<DicomReader>();

        public Scan(string path)
        {
            String[] files = System.IO.Directory.GetFiles(path);
            //dicomImages = new DicomReader[files.Length];

            Parallel.ForEach(files, (currentFile) =>
            {
                DicomReader dr = new DicomReader();
                dr.DicomFileName = currentFile;
                dicomImages.Add(dr);

                // srao test branch changes..

            });
            
            dicomImages.Sort(delegate (DicomReader d1, DicomReader d2) { return d1.DicomFileName.CompareTo(d2.DicomFileName); });
        }
        
        public List<DicomReader> Images { get { return dicomImages; } }


        public void HorizontalReconstruct(ref List<ushort> HCut, int Index,ref int resultWidth,ref int resultHeight)
        {
            if(HCut == null)
            {
                HCut = new List<ushort>();
            }
            
            resultHeight = Images.Count;

            int width = Images[0].width;
            resultWidth = width;
            int height = Images[0].height;

            for (int i = 0; i < Images.Count; i++)
            {
                List<ushort> dcImg = null;
                
                Images[i].GetPixels16(ref dcImg);
                if (dcImg.Count == width * height)
                {
                    for (int col = 0; col < width; col++)
                    {
                        HCut.Add(dcImg[Index * width + col]);
                    }
                }
            }
        }

        public void VerticalReconstruct(ref List<ushort> VCut, int Index,ref int resultWidth,ref int resultHeight)
        {
            if (VCut == null)
            {
                VCut = new List<ushort>();
            }
            
            resultHeight = Images.Count;
            for (int i = 0; i < Images.Count; i++)
            {
                List<ushort> dcImg = null;
                int width = Images[i].width;
                int height = Images[i].height;

                resultWidth = height;
                Images[i].GetPixels16(ref dcImg);
                for (int row = 0; row < height; row++)
                {
                    VCut.Add(dcImg[row * width + Index]);
                }
            }
        }
    }
}
