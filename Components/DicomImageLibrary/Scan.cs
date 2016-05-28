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
    }
}
